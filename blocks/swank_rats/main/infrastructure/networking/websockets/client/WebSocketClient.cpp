//============================================================================
// Name        : WebSocketClient.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "WebSocketClient.h"

#include "swank_rats/main/shared/notifications/MessageNotification.h"
#include "swank_rats/main/shared/model/message/MessageCommands.h"

#include "fenix/poco/Foundation/Poco/include/Exception.h"
#include "fenix/poco/Net/Poco/include/Net/HTTPMessage.h"
#include "fenix/poco/Net/Poco/include/Net/HTTPRequest.h"
#include "fenix/poco/Net/Poco/include/Net/HTTPResponse.h"
#include "fenix/poco/Foundation/Poco/include/AutoPtr.h"
#include "fenix/poco/Foundation/Poco/include/Delegate.h"

#include <string>

using Poco::AutoPtr;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPResponse;

using shared::notifications::MessageNotification;
using shared::model::message::MessageCommandEnum;

namespace infrastructure {
	namespace websocket {
		WebSocketClient::WebSocketClient(URI uri, NotificationQueue &receivedQueue)
			: connHandler(new WebSocketClientConnectionHandler(uri, receivedQueue, sendingQueue)) {
			connHandler->LostConnection += Poco::delegate(this, &WebSocketClient::OnLostconnection);
		}

		WebSocketClient::~WebSocketClient() {
			if (connHandler->IsConnected()) {
				CloseConnection();
			}
		}

		void WebSocketClient::OpenConnection() {
			if (connHandler->OpenConnection()) {
				SendInitMessage();
			}
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

		void WebSocketClient::OnLostconnection(const void* pSender, int& arg) {
			if (!connHandler->IsReconnecting() && connHandler->Reconnect()) {
				SendInitMessage();
			}
		}

		void WebSocketClient::SendInitMessage() {
			Send(new Message(MessageCommandEnum::init, "server"));
		}
	}
}