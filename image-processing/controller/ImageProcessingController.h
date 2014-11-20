//============================================================================
// Name        : ImageProcessingController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <memory>
#include <opencv2\opencv.hpp>
#include <Poco\AutoPtr.h>
#include "..\infrastructure\networking\websockets\message\MessageNotification.h"
#include "..\services\videostreaming\webcam\WebcamService.h"
#include "..\services\ShotSimulationService.h"
#include "..\shared\observer\IObserver.h"
#include "..\services\ObjectDetectionService.h"

using Poco::AutoPtr;
using services::webcam::WebcamService;
using services::webcam::WebcamServicePtr;
using services::simulation::ShotSimulationService;
using infrastructure::websocket::MessageNotification;

namespace controller {
	namespace image_processing {
		class ImageProcessingController : public IObserver <WebcamService>
		{
		public:
			ImageProcessingController(WebcamServicePtr webcamService);
			~ImageProcessingController();
			void StartImageProcessing();
			void StopImageProcessing();
			virtual void Update(WebcamService* observable);
			IplImage* GetLastFrame();
			void HandleMessageNotification(const AutoPtr<MessageNotification>& notification);
		private:
			WebcamServicePtr webcamService;
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
