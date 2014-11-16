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
#include "..\message\MessageNotification.h"

using Poco::AutoPtr;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPResponse;

using infrastructure::websocket::MessageNotification;

namespace infrastructure {
	namespace websocket {
		WebSocketClient::WebSocketClient(URI uri, Context::Ptr context, NotificationQueue &receivedQueue)
			: connHandler(new WebSocketClientConnectionHandler(uri, context, receivedQueue, sendingQueue)) {
		}

		WebSocketClient::~WebSocketClient() {
			if (connHandler->IsConnected()) {
				CloseConnection();
			}
		}
		
		void WebSocketClient::OpenConnection() {
			connHandler->OpenConnection();

			Send(new Message("cmd", "to"));
		}

		void WebSocketClient::CloseConnection() {
			sendingQueue.clear();
			sendingQueue.wakeUpAll();

			connHandler->CloseConnection();
		}
		bool WebSocketClient::IsConnected() {
			return connHandler->IsConnected();
		}

		void WebSocketClient::Send(Message* message) {
			sendingQueue.enqueueNotification(new MessageNotification(message));
		}
	}
}