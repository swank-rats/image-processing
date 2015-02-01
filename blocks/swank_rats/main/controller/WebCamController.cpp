//============================================================================
// Name        : WebCamController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "WebCamController.h"

#include "fenix/poco/Foundation/include/Poco/Thread.h"
#include "fenix/poco/Foundation/include/Poco/RunnableAdapter.h"

using Poco::Thread;
using Poco::RunnableAdapter;

namespace controller {
	namespace webcam {
		WebCamController::WebCamController(SharedPtr<WebcamService> webcamService) : webcamService(webcamService) {
		}

		WebCamController::~WebCamController()	{
			//do not delete, since it is a shared pointer
			webcamService = nullptr;
		}

		void WebCamController::StartWebCam() {
			if (!webcamService->IsRecording()) {
				webcamService->StartRecording();
			}
		}

		void WebCamController::StopWebCam() {
			if (webcamService->IsRecording()) {
				webcamService->StopRecording();
			}
		}
	}
}