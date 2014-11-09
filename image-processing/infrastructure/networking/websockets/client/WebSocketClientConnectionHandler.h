//============================================================================
// Name        : WebSocketClientConnectionHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\URI.h>
#include <Poco\Logger.h>
#include <Poco\Timespan.h>
#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\Context.h>
#include <Poco\Net\HTTPSClientSession.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Activity.h>

#include "..\WebSocketMessage.h"

using Poco::Logger;
using Poco::URI;
using Poco::Timespan;
using Poco::Net::Context;
using Poco::Activity;
using Poco::Net::HTTPSClientSession;
using Poco::Net::WebSocket;
using infrastructure::websocket::WebSocketMessage;

namespace infrastructure {
	namespace websocket {
		class WebSocketClientConnectionHandler
		{
		public:
			WebSocketClientConnectionHandler(URI uri, Context::Ptr context);
			URI GetURI();
			void OpenConnection();
			void CloseConnection();
			void Send(WebSocketMessage& message);
		private:
			URI uri;
			Context::Ptr context;
			Activity<WebSocketClientConnectionHandler> receiveActity;
			HTTPSClientSession session;
			WebSocket* webSocket;
			Timespan timeout;


			void Listen();
		};
	}
}