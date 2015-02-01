//============================================================================
// Name        : WebCamController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\services\webcam\WebcamService.h"
#include "fenix/poco/Foundation/include/Poco/SharedPtr.h"

#include <memory>

using Poco::SharedPtr;
using services::webcam::WebcamService;

namespace controller {
	namespace webcam {
		class WebCamController
		{
		public:
			WebCamController(SharedPtr<WebcamService> webcamService);
			~WebCamController();
			void StartWebCam();
			void StopWebCam();
		private:
			SharedPtr<WebcamService> webcamService;
		};
	}
}
