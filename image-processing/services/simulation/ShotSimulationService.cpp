//============================================================================
// Name        : ShotSimulationService.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ShotSimulationService.h"
#include "..\..\shared\notifications\PlayerHitNotification.h"

#include <algorithm>
#include <vector>

#include <Poco\Thread.h>
#include <Poco\ThreadPool.h>
#include <Poco\Observer.h>
#include <Poco\RunnableAdapter.h>

#include <Poco\Stopwatch.h>
using Poco::Stopwatch;

using Poco::Thread;
using Poco::ThreadPool;
using Poco::Observer;
using std::max;
using std::vector;
using cv::imread;
using shared::notifications::PlayerHitNotification;

namespace services {
	namespace simulation {
		ShotSimulationService::ShotSimulationService(SharedPtr<WebcamService> webcamService, NotificationQueue& playerHitQueue)
			: webcamService(webcamService), detectionService(), playerHitQueue(playerHitQueue), maxNeededThreads(1),
			runnable(*this, &ShotSimulationService::UpdateSimulation), threadPool(1, maxNeededThreads, 2 * webcamService->GetDelay()) {
			gunShotImg = imread("resources/images/gunfire2_small.png", CV_LOAD_IMAGE_UNCHANGED);
			cheeseImg = imread("resources/images/cheese_small.png", CV_LOAD_IMAGE_UNCHANGED);
			wallExplosionImg = imread("resources/images/explosion_wall.png", CV_LOAD_IMAGE_UNCHANGED);
			wallExplosionHalfXSize = wallExplosionImg.cols / 2;
			wallExplosionHalfYSize = wallExplosionImg.rows / 2;

			robotExplosionImg = imread("resources/images/explosion_robot.png", CV_LOAD_IMAGE_UNCHANGED);
			robotExplosionHalfXSize = robotExplosionImg.cols / 2;
			robotExplosionHalfYSize = robotExplosionImg.rows / 2;

			startExplostionHalfXSize = gunShotImg.cols / 2;
			startExplostionHalfYSize = gunShotImg.rows / 2;

			//namedWindow("Test", WINDOW_AUTOSIZE);
			threadSleepTime = 1;
			webcamService->AddObserver(this);
		}

		ShotSimulationService::~ShotSimulationService()
		{
			shallSimulate = false;
			//do not delete, since it is a shared pointer
			webcamService = nullptr;
		}

		void ShotSimulationService::Start() {
			shallSimulate = true;
		}

		void ShotSimulationService::Stop() {
			shallSimulate = false;
			threadPool.stopAll();
		}

		void ShotSimulationService::SimulateShot(Shot shot) {
			Poco::Mutex::ScopedLock lock(shotsLock);
			shots.push(new SimulationShot(shot));
		}
		void ShotSimulationService::Update(WebcamService* observable) {
			{
				Poco::Mutex::ScopedLock lock(framesLock); //will be released after leaving scop
				frames.push(observable->GetLastImage().clone());
			}

			if (threadPool.used() < maxNeededThreads) {
				threadPool.startWithPriority(Thread::Priority::PRIO_HIGHEST, runnable);
			}
		}

		void ShotSimulationService::UpdateSimulation() {
			Stopwatch total;

			while (shallSimulate)
			{
				total.restart();
				Mat frame;

				framesLock.lock();
				if (!frames.empty()) {
					frame = frames.top();

					if (frames.size() > 10) {
						frames = stack<Mat>();
					}
					framesLock.unlock();
				}
				else {
					framesLock.unlock();
					Thread::sleep(threadSleepTime);
					continue; //retry - no frames
				}

				SimulationShot* shot = nullptr;

				shotsLock.lock();
				if (shots.empty())
				{
					shotsLock.unlock();

					//set unmodified image as modified
					webcamService->SetModifiedImage(frame);

					Thread::sleep(threadSleepTime);
					continue; //set try again
				}
				else {
					shot = shots.front();
					shots.pop();
				}
				shotsLock.unlock();

				try {
					//if (shot->SimulateStartExplosion()) {
					//	//simulate gun explosion
					//	int explosionx = shot->startPoint.x - startExplostionHalfXSize > 0 ? shot->startPoint.x - startExplostionHalfXSize : 0;
					//	int explosionY = shot->startPoint.y - startExplostionHalfYSize > 0 ? shot->startPoint.y - startExplostionHalfYSize : 0;
					//	OverlayImage(frame, gunShotImg, Point2i(explosionx, explosionY));
					//}

					//simulate explosion
					//Stopwatch swStatus;
					//swStatus.start();
					SimulationShot::SimulationHitStatus status = shot->status;
					if (status == SimulationShot::SimulationHitStatus::UNKNOWN) {
						if (detectionService.HasShotHitPlayer(frame, *shot)) {
							shot->status = SimulationShot::HIT_PLAYER;
							shot->SetCurrentPointAsEndoint();

							int id = shot->hitPlayer.playerId;

							//Notify that player was hit
							playerHitQueue.enqueueNotification(new PlayerHitNotification(shot->hitPlayer, 1));
						}
					}
					//swStatus.stop();
					//printf("swStatus: %f ms\n", swStatus.elapsed() * 0.001);

					if (shot->SimulateEndExplosion()) {
						//	Stopwatch endExplo;
						//	endExplo.start();

						if (status == SimulationShot::HIT_PLAYER) {
							int explosionx = shot->endPoint.x - robotExplosionHalfXSize > 0 ? shot->endPoint.x - robotExplosionHalfXSize : 0;
							int explosionY = shot->endPoint.y - robotExplosionHalfYSize > 0 ? shot->endPoint.y - robotExplosionHalfYSize : 0;
							OverlayImage(frame, robotExplosionImg, Point2i(explosionx, explosionY));
						}
						else {
							// status == SimulationShot::HIT_WALL or UNKNOWN
							int explosionx = shot->endPoint.x - wallExplosionHalfXSize > 0 ? shot->endPoint.x - wallExplosionHalfXSize : 0;
							int explosionY = shot->endPoint.y - wallExplosionHalfYSize > 0 ? shot->endPoint.y - wallExplosionHalfYSize : 0;
							OverlayImage(frame, robotExplosionImg, Point2i(explosionx, explosionY));

							if (status != SimulationShot::HIT_WALL) {
								shot->status = SimulationShot::HIT_WALL;
							}
						}

						if (shot->IsSimulationFinished()) {
							delete shot;
							shot = nullptr;
						}

						//endExplo.stop();
						//printf("endExplo overlay: %f ms\n", endExplo.elapsed() * 0.001);
					}
					else {
						//Stopwatch bullet;
						//bullet.start();

						//simulate bullet
						OverlayImage(frame, cheeseImg, shot->GetNextShotPoint());

						//bullet.stop();
						//printf("Bullet overlay: %f ms\n", bullet.elapsed() * 0.001);
					}

					if (shot != nullptr) {
						Poco::Mutex::ScopedLock lock(shotsLock); //will be released after leaving scop
						shots.push(shot);
					}

					total.stop();
					printf("total overlay: %f ms\n", total.elapsed() * 0.001);

					//imshow("Test", frame);
					webcamService->SetModifiedImage(frame);
				}
				catch (cv::Exception& e) {
					Logger& logger = Logger::get("Test");
					logger.error(e.what());
				}
			}
		}

		void ShotSimulationService::OverlayImage(cv::Mat &background, const cv::Mat &foreground, cv::Point2i &location) {
			/*
			 * http://jepsonsblog.blogspot.co.at/2012/10/overlay-transparent-image-in-opencv.html
			 */

			// start at the row indicated by location, or at row 0 if location.y is negative.
			for (int y = max(location.y, 0); y < background.rows; ++y)
			{
				int fY = y - location.y; // because of the translation

				// we are done of we have processed all rows of the foreground image.
				if (fY >= foreground.rows) {
					break;
				}

				// start at the column indicated by location,
				// or at column 0 if location.x is negative.
				for (int x = max(location.x, 0); x < background.cols; ++x)
				{
					int fX = x - location.x; // because of the translation.

					// we are done with this row if the column is outside of the foreground image.
					if (fX >= foreground.cols) {
						break;
					}

					// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
					double opacity = ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3]) / 255.;

					// and now combine the background and foreground pixel, using the opacity,
					// but only if opacity > 0.
					for (int c = 0; opacity > 0 && c < background.channels(); ++c)
					{
						unsigned char foregroundPx = foreground.data[fY * foreground.step + fX * foreground.channels() + c];
						unsigned char backgroundPx = background.data[y * background.step + x * background.channels() + c];
						background.data[y * background.step + background.channels() * x + c] = backgroundPx * (1. - opacity) + foregroundPx * opacity;
					}
				}
			}
		}
	}
}