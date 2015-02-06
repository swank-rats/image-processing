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
			: messageHandlerActivity(this, &WebSocketController::HandleReceivedMessages), webSocketClient(new WebSocketClient(uri, receivedMessagesQueue)) {
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

		void WebSocketController::Send(Message* message) {
			webSocketClient->Send(*message);
		}

		void WebSocketController::HandleReceivedMessages() {
			while (!messageHandlerActivity.isStopped()) {
				Notification::Ptr notification(receivedMessagesQueue.waitDequeueNotification());

				if (notification) {
					MessageNotification::Ptr messageNotification = notification.cast<MessageNotification>();
					if (messageNotification)
					{
						MessageReceived(this, *messageNotification->GetData()); //notify others
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