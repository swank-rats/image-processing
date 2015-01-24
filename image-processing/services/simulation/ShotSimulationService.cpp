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

#include <Poco\Observer.h>

//#include <Poco\Stopwatch.h>
//using Poco::Stopwatch;

using Poco::Observer;
using std::max;
using std::vector;
using cv::imread;
using shared::notifications::PlayerHitNotification;


namespace services {
	namespace simulation {
		ShotSimulationService::ShotSimulationService(SharedPtr<WebcamService> webcamService, NotificationQueue& playerHitQueue)
			: webcamService(webcamService), detectionService(), playerHitQueue(playerHitQueue), 
			  runnable(*this, &ShotSimulationService::UpdateSimulation) {
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
			simulationThread.setPriority(Thread::Priority::PRIO_HIGHEST);			simulationThread.start(runnable);
		}

		void ShotSimulationService::Stop() {
			shallSimulate = false;
			if (simulationThread.isRunning()) {
				simulationThread.join();
			}
		}

		void ShotSimulationService::SimulateShot(Shot shot) {
			Poco::Mutex::ScopedLock lock(shotsSetLock); //will be released after leaving scop
			shots.insert(ShotsSetType::ValueType(SimulationShot(shot)));
		}
		void ShotSimulationService::Update(WebcamService* observable) {
			//Frames are only interesting if shots must be simulated
			if (!shots.empty()) {
				Poco::Mutex::ScopedLock lock(framesQueueLock); //will be released after leaving scop
				framesQueue.push(observable->GetLastImage().clone());
			}
		}

		void ShotSimulationService::UpdateSimulation() {
			while (shallSimulate) {
				//Stopwatch hole;
				//hole.start();

				if (shots.empty() || framesQueue.empty()) {
					//remove frames since we don't need them
					if (!framesQueue.empty()) framesQueue.pop(); 
					continue;
				}

				try {
					Mat frame;
					{
						Poco::Mutex::ScopedLock lock(framesQueueLock); //will be released after leaving scop
						frame = framesQueue.front();
						framesQueue.pop();
					}

					Poco::Mutex::ScopedLock lock(shotsSetLock); //will be released after leaving scop

					ShotsSetType::Iterator iter = shots.begin();
					vector<Shot> deleteShots;

					while (iter != shots.end()){

						//if (iter->SimulateStartExplosion()) {
						//	//simulate gun explosion
						//	int explosionx = iter->startPoint.x - startExplostionHalfXSize > 0 ? iter->startPoint.x - startExplostionHalfXSize : 0;
						//	int explosionY = iter->startPoint.y - startExplostionHalfYSize > 0 ? iter->startPoint.y - startExplostionHalfYSize : 0;
						//	OverlayImage(frame, gunShotImg, Point2i(explosionx, explosionY));
						//}

						//simulate explosion
						SimulationShot::SimulationHitStatus status = iter->status;
						if (status == SimulationShot::SimulationHitStatus::UNKNOWN) {
							if (detectionService.HasShotHitPlayer(frame, *iter)) {
								iter->status = SimulationShot::HIT_PLAYER;
								iter->SetCurrentPointAsEndoint();

								int id = iter->hitPlayer.playerId;

								//Notify that player was hit
								playerHitQueue.enqueueNotification(new PlayerHitNotification(iter->hitPlayer,1));
							}
						}

						if (iter->SimulateEndExplosion()) {
							//Stopwatch endExplo;
							//endExplo.start();

							if (status == SimulationShot::HIT_PLAYER) {
								int explosionx = iter->endPoint.x - robotExplosionHalfXSize > 0 ? iter->endPoint.x - robotExplosionHalfXSize : 0;
								int explosionY = iter->endPoint.y - robotExplosionHalfYSize > 0 ? iter->endPoint.y - robotExplosionHalfYSize : 0;
								OverlayImage(frame, robotExplosionImg, Point2i(explosionx, explosionY));
							}
							else {
								// status == SimulationShot::HIT_WALL or UNKNOWN
								int explosionx = iter->endPoint.x - wallExplosionHalfXSize > 0 ? iter->endPoint.x - wallExplosionHalfXSize : 0;
								int explosionY = iter->endPoint.y - wallExplosionHalfYSize > 0 ? iter->endPoint.y - wallExplosionHalfYSize : 0;
								OverlayImage(frame, robotExplosionImg, Point2i(explosionx, explosionY));

								if (status != SimulationShot::HIT_WALL) {
									iter->status = SimulationShot::HIT_WALL;
								}
							}

							if (iter->IsSimulationFinished()) {
								deleteShots.push_back(*iter);
							}

							//endExplo.stop();
							//printf("endExplo overlay: %f ms\n", endExplo.elapsed() * 0.001);
						}
						else {
							//Stopwatch bullet;
							//bullet.start();

							//simulate bullet
							OverlayImage(frame, cheeseImg, iter->GetNextShotPoint());

							//bullet.stop();
							//printf("Bullet overlay: %f ms\n", bullet.elapsed() * 0.001);
						}

						++iter;
					}

					//hole.stop();
					//printf("hole overlay: %f ms\n", hole.elapsed() * 0.001);

					//imshow("Test", frame);

					webcamService->SetModifiedImage(frame);


					//delete finished simulations
					for each (Shot shot in deleteShots)
					{
						shots.erase(shot);
					}
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