//============================================================================
// Name        : ShotSimulationService.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <algorithm>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>


#include "ShotSimulationService.h"

using std::max;
using cv::imread;

namespace services {
	namespace simulation {
		ShotSimulationService::ShotSimulationService(WebcamServicePtr webcamService) : webcamService(webcamService)
		{
			gunShotImg = imread("resources/images/gunfire_small.png", CV_LOAD_IMAGE_UNCHANGED);
			bulletLeftImg = imread("resources/images/bullet_small_left.png", CV_LOAD_IMAGE_UNCHANGED);
			bulletRightImg = imread("resources/images/bullet_small_right.png", CV_LOAD_IMAGE_UNCHANGED);
			wallExplosionImg = imread("resources/images/explosion_wall.png", CV_LOAD_IMAGE_UNCHANGED);
			robotExplosionImg = imread("resources/images/explosion_robot.png", CV_LOAD_IMAGE_UNCHANGED);
		}

		ShotSimulationService::~ShotSimulationService()
		{
		}

		void ShotSimulationService::SimulateShot() {
			try {
				Mat frame = webcamService->GetLastImage();
				OverlayImage(frame, gunShotImg, Point2i(0, 0));
				OverlayImage(frame, wallExplosionImg, Point2i(81, 81));
				OverlayImage(frame, robotExplosionImg, Point2i(200, 200));
				OverlayImage(frame, bulletLeftImg, Point2i(350, 350));
			}
			catch (cv::Exception& e) {
				Logger& logger = Logger::get("Test");
				logger.error(e.what());
			}
		}

		void ShotSimulationService::OverlayImage(cv::Mat &background, const cv::Mat &foreground, cv::Point2i location)
		{
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
						background.data[y * background.step + background.channels()*x + c] = backgroundPx * (1. - opacity) + foregroundPx * opacity;
					}
				}
			}
		}
	}
}