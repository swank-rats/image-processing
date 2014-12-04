//============================================================================
// Name        : VideoStreamingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "VideoStreamingController.h"

namespace controller {
	namespace video_streaming {
		VideoStreamingController::VideoStreamingController(SharedPtr<WebcamService> webcamService) {
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

		void VideoStreamingController::HandleMessageNotification(const AutoPtr<MessageNotification>& notification) {
			//TODO handle message - eg simulate shot and so on
		}
	}
}