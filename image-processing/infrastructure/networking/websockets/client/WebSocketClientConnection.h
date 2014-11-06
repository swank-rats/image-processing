//============================================================================
// Name        : WebSocketClientConnection.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\URI.h>
#include <Poco\Logger.h>
#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\Context.h>
#include <Poco\Net\HTTPSClientSession.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Activity.h>

using Poco::Logger;
using Poco::URI;
using Poco::Net::Context;
using Poco::Activity;


namespace infrastructure {
	namespace websocket {
		class WebSocketClientConnection
		{
		public:
			WebSocketClientConnection(URI uri, Context::Ptr context);
			URI GetURI();
			void OpenConnection();
			void CloseConnection();
		private:
			URI uri;
			Context::Ptr context;
			Activity<WebSocketClientConnection> connectionActivity;

			static Poco::Logger& logger;

			void HandleConnection();
		};
	}
}