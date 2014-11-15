//============================================================================
// Name        : WebSocketClient.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <string>

#include <Poco\Exception.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPResponse.h>
#include <Poco\AutoPtr.h>

#include "WebSocketClient.h"
#include "WebSocketMessage.h"

using Poco::AutoPtr;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPResponse;

using infrastructure::websocket::StringMap;

namespace infrastructure {
	namespace websocket {
		WebSocketClient::WebSocketClient(URI uri, Context::Ptr context)
			: messageHandlerActivity(this, &WebSocketClient::HandleMessage)
		{
			sendingQueue = new NotificationQueue();
			connHandler = new WebSocketClientConnectionHandler(uri, context, sendingQueue);
		}

		WebSocketClient::~WebSocketClient() {
			if (connHandler->IsConnected()) {
				CloseConnection();
			}

			delete connHandler;
			connHandler = nullptr;
			delete sendingQueue;
			sendingQueue = nullptr;
			//controller = nullptr;
		}
		
		void WebSocketClient::OpenConnection() {
			messageHandlerActivity.start();

			connHandler->OpenConnection();
		}

		void WebSocketClient::CloseConnection() {
			sendingQueue->clear();
			sendingQueue->wakeUpAll();

			connHandler->CloseConnection();

			if (messageHandlerActivity.isRunning()) {
				messageHandlerActivity.stop();
				messageHandlerActivity.wait();
			}
		}
		bool WebSocketClient::IsConnected() {
			return connHandler->IsConnected();
		}

		void WebSocketClient::Send(WebSocketMessage* message) {
			sendingQueue->enqueueNotification(new WebSocketMessageNotification(message));
		}

		void WebSocketClient::HandleMessage() {
			Logger& logger = Logger::get("WebSocketClient");
			NotificationQueue* queue = connHandler->GetReceivedMessagesQueues();
			AutoPtr<Notification> notification(queue->waitDequeueNotification());

			while (!messageHandlerActivity.isStopped() && notification) {
				WebSocketMessageNotification* messageNotification = dynamic_cast<WebSocketMessageNotification*>(notification.get());
				if (messageNotification)
				{
					logger.information("received message");
					//TODO push message to controller
					//controller->HandleReceivedMessage(messageNotification->GetData());
				}

				notification = queue->waitDequeueNotification();
			}
		}
	}
}