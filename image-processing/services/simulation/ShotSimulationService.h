//============================================================================
// Name        : ShotSimulationService.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\webcam\WebcamService.h"
#include "..\ObjectDetectionService.h"
#include "..\..\shared\observer\IObserver.h"
#include "..\..\shared\model\Shot.h"

#include <opencv2\core\core.hpp>

#include <Poco\HashMap.h>
#include <Poco\SharedPtr.h>
#include <Poco\NotificationQueue.h>

#include <memory>

using cv::Mat;
using cv::Point2i;
using services::webcam::WebcamService;
using shared::model::Shot;
using Poco::HashMap;
using Poco::SharedPtr;
using Poco::NotificationQueue;

namespace services {
	namespace simulation {
		class ShotSimulationService : public IObserver < WebcamService >
		{
		public:
			ShotSimulationService(SharedPtr<WebcamService> webcamService, NotificationQueue& playerHitQueue);
			~ShotSimulationService();
			void SimulateShot(Shot shot);
			void Update(WebcamService* observable);
		private:
			Mat gunShotImg;
			Mat cheeseImg;
			Mat wallExplosionImg;
			Mat robotExplosionImg;
			SharedPtr<WebcamService> webcamService;
			ObjectDetectionService detectionService;
			NotificationQueue& playerHitQueue;

			typedef HashMap<Shot, Point2i, Shot> ShotsMapType;
			ShotsMapType shots;

			void OverlayImage(Mat &background, const Mat &foreground, Point2i location);
			bool ArePointsEqual(const Point2i &p1, const Point2i &p2);
			bool IsPointBiggerOrEqual(const Point2i &p1, const Point2i &p2);
		};
	}
}