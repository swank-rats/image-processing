//============================================================================
// Name        : ShotSimulationService.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <memory>

#include "videostreaming\webcam\WebcamService.h"

using cv::Mat;
using cv::Point2i;
using services::webcam::WebcamService;
using services::webcam::WebcamServicePtr;

namespace services {
	namespace simulation {
		class ShotSimulationService
		{
		public:
			ShotSimulationService(WebcamServicePtr webcamService);
			~ShotSimulationService();
			void SimulateShot(); //TODO Parameters
		private:
			Mat gunShotImg;
			Mat bulletLeftImg;
			Mat bulletRightImg;
			Mat wallExplosionImg;
			Mat robotExplosionImg;
			WebcamServicePtr webcamService;
			void OverlayImage(Mat &background, const Mat &foreground, Point2i location);

		};
	}
}