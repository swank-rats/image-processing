//============================================================================
// Name        : WebSocketClient.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "WebSocketClient.h"

#include "..\..\..\..\shared\notifications\MessageNotification.h"
#include "..\..\..\..\shared\model\message\MessageCommands.h"

#include <Poco\Exception.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPResponse.h>
#include <Poco\AutoPtr.h>
#include <Poco\Delegate.h>

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
			: connHandler(new WebSocketClientConnectionHandler(uri, receivedQueue)) {
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
			connHandler->CloseConnection();
		}
		bool WebSocketClient::IsConnected() {
			return connHandler->IsConnected();
		}

		void WebSocketClient::Send(Message& message) {
			connHandler->Send(message);
		}

		void WebSocketClient::OnLostconnection(const void* pSender, int& arg) {
			if (!connHandler->IsReconnecting() && connHandler->Reconnect()) {
				SendInitMessage();
			}
		}

		void WebSocketClient::SendInitMessage() {
			Send(*new Message(MessageCommandEnum::init, "server"));
		}
	}
}