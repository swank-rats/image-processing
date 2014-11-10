//============================================================================
// Name        : WebSocketClient.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\URI.h>
#include <Poco\Logger.h>
#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\HTTPSClientSession.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Net\Context.h>
#include <Poco\NotificationQueue.h>

#include "WebSocketClientConnectionHandler.h"

using Poco::Logger;
using Poco::URI;
using Poco::Net::HTTPSClientSession;
using Poco::Net::Context;
using Poco::Net::WebSocket;
using Poco::NotificationQueue;


namespace infrastructure {
	namespace websocket {
		class WebSocketClient
		{
		public:
			WebSocketClient(URI uri, Context::Ptr context);
			void OpenConnection();
			void CloseConnection();
			void Send(WebSocketMessage message);
		private:
			WebSocketClientConnectionHandler* connHandler;
			NotificationQueue queue;
		};
	}
}

