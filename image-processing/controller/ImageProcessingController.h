//============================================================================
// Name        : ImageProcessingController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <memory>
#include <opencv2\opencv.hpp>
#include "..\services\videostreaming\webcam\WebcamService.h"
#include "..\shared\observer\IObserver.h"
#include "..\services\ObjectDetectionService.h"

using namespace services::webcam;

namespace controller {
	namespace image_processing {
		class ImageProcessingController : public IObserver <WebcamService>
		{
		public:
			ImageProcessingController(WebcamServicePtr webcamService);
			void StartImageProcessing();
			void StopImageProcessing();
			virtual void Update(WebcamService* observable);
			IplImage* GetLastFrame();
		private:
			WebcamServicePtr webcamService;
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
