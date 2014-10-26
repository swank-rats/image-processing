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
			streamingServer = new VideoStreamingServer("127.0.0.1", "4711", "videostream", webcamService);
		}


		VideoStreamingController::~VideoStreamingController()
		{
			StopNetworkService();
		}

		void VideoStreamingController::StartStreamingServer() {
			streamingServer->StartServer();
		}

		bool VideoStreamingController::StopNetworkService() {
			return true;
		}
	}
}