//============================================================================
// Name        : VideoStreamingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "VideoStreamingController.h"
#include "swank_rats/main/shared/model/message/MessageCommands.h"
#include "swank_rats/main/shared/notifications/ClientConnectionLostNotification.h"

using shared::model::message::MessageCommandEnum;
using shared::notifications::ClientConnectionLostNotification;

namespace controller {
	namespace video_streaming {
		VideoStreamingController::VideoStreamingController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> webSocketController)
			:lostConnectionQueue(), lostConnectionHandlerActivity(this, &VideoStreamingController::HandleLostConnections), webSocketController(webSocketController) {
			streamingServer = new VideoStreamingServer(4711, "/videostream", webcamService, lostConnectionQueue);

		}

		VideoStreamingController::~VideoStreamingController() {
			delete streamingServer;
			streamingServer = nullptr;

			webSocketController = nullptr; //do not delete - is shared pointer
		}

		void VideoStreamingController::StartStreamingServer() {
			if (!lostConnectionHandlerActivity.isRunning()) {
				lostConnectionHandlerActivity.start();
			}

			if (!streamingServer->IsRunning()) {
				streamingServer->StartServer();
			}
		}

		bool VideoStreamingController::StopStreamingServer() {
			if (lostConnectionHandlerActivity.isRunning()) {
				lostConnectionHandlerActivity.stop();
				lostConnectionQueue.wakeUpAll();
				lostConnectionHandlerActivity.wait();
			}

			if (streamingServer->IsRunning()) {
				streamingServer->StopServer();
			}

			lostConnectionQueue.clear(); //clear possible lost connection notifications

			return true;
		}

		void VideoStreamingController::HandleMessageNotification(MessageNotification* notification) {
			Message *message = notification->GetData();

			switch (message->GetCmd()) {
			case MessageCommandEnum::start:
				StartStreamingServer();
				break;
			case MessageCommandEnum::stop:
				StopStreamingServer();
				break;
			}

			notification->release();
		}

		void VideoStreamingController::HandleLostConnections() {
			while (!lostConnectionHandlerActivity.isStopped()) {
				Notification::Ptr notification(lostConnectionQueue.waitDequeueNotification());

				if (notification) {
					ClientConnectionLostNotification::Ptr connLostNotification = notification.cast<ClientConnectionLostNotification>();
					if (connLostNotification)
					{
						Message* message = new Message(MessageCommandEnum::connectionlost);
						message->AddParam("ip", connLostNotification->GetSocketAddress().toString());
						webSocketController->Send(message);
					}
				}
				else {
					//null signals that worker should stop polling queue
					lostConnectionHandlerActivity.stop();
					break;
				}
			}
		}
	}
}