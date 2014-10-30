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

using namespace services::webcam;

namespace controller {
	namespace image_processing {
		class ImageProcessingController : public IObserver <WebcamService>
		{
		public:
			ImageProcessingController(WebcamServicePtr webcamService);
			void StartImageProcessing();
			bool StopImageProcessing();
			virtual void Update(WebcamService* observable);
			IplImage* GetLastFrame();
		private:
			WebcamServicePtr webcamService;
			void CreateTrackBarView();
		};
	}
}