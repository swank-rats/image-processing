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

#include <Poco\HashSet.h>
#include <Poco\SharedPtr.h>
#include <Poco\NotificationQueue.h>
#include <Poco\Mutex.h>

#include <memory>

using cv::Mat;
using cv::Point2i;
using services::webcam::WebcamService;
using services::object_detection::ObjectDetectionService;
using shared::model::Shot;
using shared::model::SimulationShot;
using Poco::HashSet;
using Poco::SharedPtr;
using Poco::NotificationQueue;
using Poco::Mutex;

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
			int robotExplosionHalfXSize;
			int robotExplosionHalfYSize;
			int wallExplosionHalfXSize;
			int wallExplosionHalfYSize;
			int startExplostionHalfXSize;
			int startExplostionHalfYSize;
			Poco::Mutex mutexThreadLock;
			SharedPtr<WebcamService> webcamService;
			ObjectDetectionService detectionService;
			NotificationQueue& playerHitQueue;

			typedef HashSet<SimulationShot, SimulationShot> ShotsSetType;
			ShotsSetType shots;

			void OverlayImage(Mat &background, const Mat &foreground, Point2i &location);
		};
	}
}