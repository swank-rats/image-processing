//============================================================================
// Name        : WebSocketController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "WebSocketController.h"

namespace controller {
	namespace websocket {
		WebSocketController::WebSocketController(URI uri)
			: messageHandlerActivity(this, &WebSocketController::HandleReceivedMessages),
			webSocketClient(new WebSocketClient(uri, receivedMessagesQueue)), notificationCenter() {
		}

		WebSocketController::~WebSocketController() {
			if (webSocketClient->IsConnected()) {
				StopWebSocketClient();
			}

			delete webSocketClient;
			webSocketClient = nullptr;
		}

		void WebSocketController::StartWebSocketClient() {
			if (!webSocketClient->IsConnected()) {
				webSocketClient->OpenConnection();
				messageHandlerActivity.start();
			}
		}

		void WebSocketController::StopWebSocketClient() {
			if (webSocketClient->IsConnected()) {
				receivedMessagesQueue.clear();
				receivedMessagesQueue.wakeUpAll();

				webSocketClient->CloseConnection();

				if (messageHandlerActivity.isRunning()) {
					messageHandlerActivity.stop();
					messageHandlerActivity.wait();
				}
			}
		}

		NotificationCenter& WebSocketController::GetNotificationCenter() {
			return notificationCenter;
		}

		void WebSocketController::Send(Message* message) {
			webSocketClient->Send(message);
		}

		void WebSocketController::HandleReceivedMessages() {
			while (!messageHandlerActivity.isStopped()) {
				Notification::Ptr notification(receivedMessagesQueue.waitDequeueNotification());

				if (notification) {
					MessageNotification::Ptr messageNotification = notification.cast<MessageNotification>();
					if (messageNotification)
					{
						notificationCenter.postNotification(messageNotification);
					}
				}
				else {
					//null signals that worker should stop polling queue
					messageHandlerActivity.stop();
					break;
				}
			}
		}
	}
}