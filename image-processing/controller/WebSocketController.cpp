//============================================================================
// Name        : WebSocketController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "WebSocketController.h"

namespace controller {
	namespace websocket {
		WebSocketController::WebSocketController(URI uri, Context::Ptr context)
			: messageHandlerActivity(this, &WebSocketController::HandleReceivedMessages),
			webSocketClient(new WebSocketClient(uri, context, receivedMessagesQueue)) {
		}

		WebSocketController::~WebSocketController() {
			if (webSocketClient->IsConnected()) {
				StopWebSocketClient();
			}

			delete webSocketClient;
			webSocketClient = nullptr;
		}

		void WebSocketController::StartWebSocketClient() {
			webSocketClient->OpenConnection();

			messageHandlerActivity.start();
		}

		void WebSocketController::StopWebSocketClient() {
			receivedMessagesQueue.clear();
			receivedMessagesQueue.wakeUpAll();

			webSocketClient->CloseConnection();

			if (messageHandlerActivity.isRunning()) {
				messageHandlerActivity.stop();
				messageHandlerActivity.wait();
			}
		}

		NotificationCenter& WebSocketController::GetNotificationCenter() {
			return notificationCenter;
		}

		void WebSocketController::Send(const Message* message) {
			webSocketClient->Send(message);
		}

		void WebSocketController::HandleReceivedMessages() {
			AutoPtr<Notification> notification(receivedMessagesQueue.waitDequeueNotification());

			while (!messageHandlerActivity.isStopped() && notification) {
				MessageNotification* messageNotification = dynamic_cast<MessageNotification*>(notification.get());
				if (messageNotification)
				{
					notificationCenter.postNotification(messageNotification);
				}

				notification = receivedMessagesQueue.waitDequeueNotification();
			}
		}
	}
}