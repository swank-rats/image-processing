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
#include <Poco\TaskManager.h>

#include "WebSocketClientConnection.h"

using Poco::Logger;
using Poco::URI;
using Poco::Net::HTTPSClientSession;
using Poco::Net::WebSocket;
using Poco::TaskManager;


namespace infrastructure {
	namespace websocket {
		class WebSocketClient
		{
		public:
			WebSocketClient(URI uri);
			void OpenConnection();
			void CloseConnection();
		private:
			WebSocketClientConnection* connection;
			TaskManager taskManager;
			static Poco::Logger& logger;
		};
	}
}

