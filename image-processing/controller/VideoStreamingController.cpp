//============================================================================
// Name        : VideoStreamingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "VideoStreamingController.h"

namespace controller {
	namespace video_streaming {
		VideoStreamingController::VideoStreamingController(WebcamServicePtr webcamService)
		{
			streamingServer = new VideoStreamingServer(4711, "/videostream", webcamService);
		}

		VideoStreamingController::~VideoStreamingController() {
			delete streamingServer;
			streamingServer = nullptr;
		}

		void VideoStreamingController::StartStreamingServer() {
			streamingServer->StartServer();
		}

		bool VideoStreamingController::StopStreamingServer() {
			streamingServer->StopServer();
			return true;
		}
	}
}