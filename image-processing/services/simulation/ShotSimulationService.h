//============================================================================
// Name        : ShotSimulationService.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\webcam\WebcamService.h"
#include "..\ObjectDetectionService.h"
#include "..\..\shared\model\Shot.h"
#include "..\..\shared\observer\IObserver.h"
#include "..\..\shared\events\PlayerHitArgs.h"

#include <opencv2\core\core.hpp>

#include <Poco\BasicEvent.h>
#include <Poco\ThreadPool.h>
#include <Poco\HashSet.h>
#include <Poco\SharedPtr.h>
#include <Poco\NotificationQueue.h>
#include <Poco\Mutex.h>
#include <Poco\RunnableAdapter.h>

#include <queue>
#include <vector>
#include <memory>

using cv::Mat;
using cv::Point2i;
using services::webcam::WebcamService;
using services::object_detection::ObjectDetectionService;
using shared::model::Shot;
using shared::model::SimulationShot;
using shared::events::PlayerHitArgs;
using Poco::HashSet;
using Poco::BasicEvent;
using Poco::SharedPtr;
using Poco::NotificationQueue;
using Poco::Mutex;
using Poco::ThreadPool;
using Poco::RunnableAdapter;

namespace services {
	namespace simulation {
		class ShotSimulationService : public IObserver < WebcamService >
		{
		public:
			ShotSimulationService(SharedPtr<WebcamService> webcamService);
			~ShotSimulationService();

			BasicEvent<const PlayerHitArgs> PlayerHit;

			void SimulateShot(Shot shot);
			void Start();
			void Stop();
			void Update(WebcamService* observable);
		private:
			typedef HashSet<SimulationShot, SimulationShot> ShotsSetType;

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
			Poco::Mutex shotsSetLock;
			Poco::Mutex framesQueueLock;
			RunnableAdapter<ShotSimulationService> runnable;
			bool shallSimulate;
			int maxNeededThreads;
			int threadSleepTime;
			ThreadPool threadPool;
			queue<Mat> framesQueue;
			SharedPtr<WebcamService> webcamService;
			ObjectDetectionService detectionService;
			ShotsSetType shots;

			void UpdateSimulation();
			void OverlayImage(Mat &background, const Mat &foreground, Point2i &location);
		};
	}
}