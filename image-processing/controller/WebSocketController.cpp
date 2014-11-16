//============================================================================
// Name        : WebSocketController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <Poco\AutoPtr.h>
#include "WebSocketController.h"

using Poco::AutoPtr;

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

		void WebSocketController::HandleReceivedMessages() {
			Logger& logger = Logger::get("WebSocketController");
			AutoPtr<Notification> notification(receivedMessagesQueue.waitDequeueNotification());

			while (!messageHandlerActivity.isStopped() && notification) {
				WebSocketMessageNotification* messageNotification = dynamic_cast<WebSocketMessageNotification*>(notification.get());
				if (messageNotification)
				{
					logger.information("received message");
				}

				notification = receivedMessagesQueue.waitDequeueNotification();
			}
		}
	}
}