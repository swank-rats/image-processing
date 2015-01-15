//============================================================================
// Name        : ImageProcessingController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\services\webcam\WebcamService.h"
#include <Poco\SharedPtr.h>

#include <memory>

using Poco::SharedPtr;
using services::webcam::WebcamService;

namespace controller {
	namespace image_processing {
		class ImageProcessingController
		{
		public:
			ImageProcessingController(SharedPtr<WebcamService> webcamService);
			~ImageProcessingController();
			void StartImageProcessing();
			void StopImageProcessing();
		private:
			SharedPtr<WebcamService> webcamService;
		};
	}
}
