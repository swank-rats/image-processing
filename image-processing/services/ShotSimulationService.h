//============================================================================
// Name        : ShotSimulationService.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "videostreaming\webcam\WebcamService.h"
#include "..\shared\observer\IObserver.h"
#include "..\shared\model\Shot.h"

#include <opencv2\core\core.hpp>
#include <Poco\HashMap.h>

#include <memory>

using cv::Mat;
using cv::Point2i;
using services::webcam::WebcamService;
using services::webcam::WebcamServicePtr;
using shared::model::Shot;
using Poco::HashMap;

namespace services {
	namespace simulation {
		class ShotSimulationService : public IObserver < WebcamService >
		{
		public:
			ShotSimulationService(WebcamServicePtr webcamService);
			~ShotSimulationService();
			void SimulateShot(Shot shot);
			void Update(WebcamService* observable);
		private:
			Mat gunShotImg;
			Mat bulletLeftImg;
			Mat bulletRightImg;
			Mat wallExplosionImg;
			Mat robotExplosionImg;
			WebcamServicePtr webcamService;

			typedef HashMap<Shot, Point2i> ShotsMapType;
			ShotsMapType shots;

			void OverlayImage(Mat &background, const Mat &foreground, Point2i location);
			bool ArePointsEqual(const Point2i &p1, const Point2i &p2);
			bool IsPointBiggerOrEqual(const Point2i &p1, const Point2i &p2);
		};
	}
}