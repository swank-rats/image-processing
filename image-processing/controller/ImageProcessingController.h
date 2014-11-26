//============================================================================
// Name        : ImageProcessingController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\infrastructure\networking\websockets\message\MessageNotification.h"
#include "..\services\webcam\WebcamService.h"
#include "..\services\simulation\ShotSimulationService.h"
#include "..\shared\observer\IObserver.h"
#include "..\services\ObjectDetectionService.h"
#include "WebSocketController.h"

#include <opencv2\opencv.hpp>

#include <Poco\AutoPtr.h>
#include <Poco\Timer.h>
#include <Poco\SharedPtr.h>

#include <memory>

using Poco::AutoPtr;
using Poco::Timer;
using Poco::SharedPtr;
using services::webcam::WebcamService;
using services::webcam::WebcamService;
using services::simulation::ShotSimulationService;
using infrastructure::websocket::MessageNotification;
using controller::websocket::WebSocketController;

namespace controller {
	namespace image_processing {
		class ImageProcessingController : public IObserver < WebcamService >
		{
		public:
			ImageProcessingController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> websocketController);
			~ImageProcessingController();
			void StartImageProcessing();
			void StopImageProcessing();
			void Update(WebcamService* observable);
			IplImage* GetLastFrame();
			void HandleMessageNotification(const AutoPtr<MessageNotification>& notification);
			void OnTimer(Timer& timer);
		private:
			SharedPtr<WebcamService> webcamService;
			SharedPtr<WebSocketController> websocketController;
			ShotSimulationService shotSimulation;
			ObjectDetectionService* detectService;
			int iLowH = 0;
			int iHighH = 179;
			int iLowS = 0;
			int iHighS = 255;
			int iLowV = 0;
			int iHighV = 255;
			void CreateTrackBarView();
		};
	}
}
