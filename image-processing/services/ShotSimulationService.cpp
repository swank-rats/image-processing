//============================================================================
// Name        : ShotSimulationService.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ShotSimulationService.h"

#include <algorithm>
#include <vector>

using std::max;
using std::vector;
using cv::imread;
using shared::model::ShotEndPointType;

namespace services {
	namespace simulation {
		ShotSimulationService::ShotSimulationService(WebcamServicePtr webcamService) : webcamService(webcamService) {
			gunShotImg = imread("resources/images/gunfire_small.png", CV_LOAD_IMAGE_UNCHANGED);
			bulletLeftImg = imread("resources/images/bullet_small_left.png", CV_LOAD_IMAGE_UNCHANGED);
			bulletRightImg = imread("resources/images/bullet_small_right.png", CV_LOAD_IMAGE_UNCHANGED);
			wallExplosionImg = imread("resources/images/explosion_wall.png", CV_LOAD_IMAGE_UNCHANGED);
			robotExplosionImg = imread("resources/images/explosion_robot.png", CV_LOAD_IMAGE_UNCHANGED);

			webcamService->AddObserver(this);
		}

		ShotSimulationService::~ShotSimulationService()
		{
			webcamService->RemoveObserver(this);
		}

		void ShotSimulationService::SimulateShot(Shot shot) {
			shots.insert(ShotsMapType::ValueType(shot, shot.startPoint));
		}

		void ShotSimulationService::Update(WebcamService* observable) {
			if (shots.empty()) {
				return;
			}

			try {
				Mat frame = observable->GetLastImage();
				ShotsMapType::Iterator iter = shots.begin();
				vector<Shot> deleteShots;

				do {
					if (ArePointsEqual(iter->first.startPoint, iter->second)) {
						//simulate gun shot
						OverlayImage(frame, gunShotImg, iter->first.startPoint);
						iter->second.x += gunShotImg.cols;
						iter->second.y += gunShotImg.rows / 2;
					}
					else if (ArePointsEqual(iter->first.endPoint, iter->second)) {
						//simulate explosion
						if (iter->first.endPointType == ShotEndPointType::Robot) {
							OverlayImage(frame, robotExplosionImg, iter->first.endPoint);
						}
						else {
							OverlayImage(frame, wallExplosionImg, iter->first.endPoint);
						}

						deleteShots.push_back(iter->first);
					}
					else {
						//simulate bullet
						OverlayImage(frame, bulletLeftImg, iter->second);
						iter->second.x += bulletLeftImg.cols;
						iter->second.y += bulletLeftImg.rows;

						if (IsPointBiggerOrEqual(iter->second, iter->first.endPoint)) {
							//simulation is finished after next step
							iter->second = iter->first.endPoint;
						}
					}

					++iter;
				} while (iter != shots.end());

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

		void ShotSimulationService::OverlayImage(cv::Mat &background, const cv::Mat &foreground, cv::Point2i location) {
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

		bool ShotSimulationService::ArePointsEqual(const Point2i &p1, const Point2i &p2) {
			return p1.x == p2.x && p1.y == p2.y;
		}

		bool ShotSimulationService::IsPointBiggerOrEqual(const Point2i &p1, const Point2i &p2) {
			return p1.x >= p2.x && p1.y >= p2.y;
		}
	}
}