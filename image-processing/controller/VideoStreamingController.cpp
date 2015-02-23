//============================================================================
// Name        : VideoStreamingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "VideoStreamingController.h"
#include "..\shared\model\message\MessageCommands.h"

#include <Poco\Delegate.h>

using shared::model::message::MessageCommandEnum;

using Poco::Delegate;

namespace controller {
	namespace video_streaming {
		VideoStreamingController::VideoStreamingController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> webSocketController)
			: webSocketController(webSocketController) {
			streamingServer = new VideoStreamingServer(4711, "/videostream", webcamService);
			streamingServer->ClientLostConnection += Poco::delegate(this, &VideoStreamingController::HandleLostConnections);
		}

		VideoStreamingController::~VideoStreamingController() {
			streamingServer->ClientLostConnection -= Poco::delegate(this, &VideoStreamingController::HandleLostConnections);
			delete streamingServer;
			streamingServer = nullptr;

			webSocketController = nullptr; //do not delete - is shared pointer
		}

		void VideoStreamingController::StartStreamingServer() {
			if (!streamingServer->IsRunning()) {
				streamingServer->StartServer();
			}
		}

		bool VideoStreamingController::StopStreamingServer() {
			if (streamingServer->IsRunning()) {
				streamingServer->StopServer();
			}

			return true;
		}

		void VideoStreamingController::HandleLostConnections(const void* pSender, const SocketAddress& arg) {
			Message* message = new Message(MessageCommandEnum::connectionlost);
			message->AddParam("ip", arg.toString());
			webSocketController->Send(message);
		}
	}
}