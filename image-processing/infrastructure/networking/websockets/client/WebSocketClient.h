//============================================================================
// Name        : WebSocketClient.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\..\..\shared\model\message\Message.h"
#include "WebSocketClientConnectionHandler.h"

#include <Poco\URI.h>
#include <Poco\Logger.h>
#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\HTTPSClientSession.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Net\Context.h>
#include <Poco\NotificationQueue.h>
#include <Poco\Activity.h>

using Poco::Logger;
using Poco::URI;
using Poco::NotificationQueue;
using Poco::Activity;
using Poco::Net::HTTPSClientSession;
using Poco::Net::Context;
using Poco::Net::WebSocket;
using shared::model::message::Message;

namespace infrastructure {
	namespace websocket {
		class WebSocketClient
		{
		public:
			WebSocketClient(URI uri, Context::Ptr context, NotificationQueue &receivedQueue);
			~WebSocketClient();
			void OpenConnection();
			void CloseConnection();
			bool IsConnected();
			void Send(const Message* message);
		private:
			WebSocketClientConnectionHandler* connHandler;
			NotificationQueue sendingQueue;
		};
	}
}
