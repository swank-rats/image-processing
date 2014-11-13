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
#include <Poco\ActiveMethod.h>
#include <Poco\ActiveResult.h>
#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\Context.h>
#include <Poco\Net\HTTPSClientSession.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Activity.h>
#include <Poco\NotificationQueue.h>

#include "WebSocketMessage.h"

using Poco::Logger;
using Poco::URI;
using Poco::Timespan;
using Poco::ActiveMethod;
using Poco::ActiveResult;
using Poco::Activity;
using Poco::NotificationQueue;
using Poco::Net::Context;
using Poco::Net::HTTPSClientSession;
using Poco::Net::WebSocket;
using infrastructure::websocket::WebSocketMessage;

namespace infrastructure {
	namespace websocket {
		class WebSocketClientConnectionHandler
		{
		public:
			WebSocketClientConnectionHandler(URI uri, Context::Ptr context, NotificationQueue& queue);
			~WebSocketClientConnectionHandler();
			URI GetURI();
			void OpenConnection();
			void CloseConnection();
			bool IsConnected();
			const NotificationQueue& GetReceivedMessagesQueues();
		private:
			URI uri;
			Context::Ptr context;
			Activity<WebSocketClientConnectionHandler> receiveActity;
			Activity<WebSocketClientConnectionHandler> sendActity;
			HTTPSClientSession session;
			WebSocket* webSocket;
			Timespan timeout;
			NotificationQueue& sendingQueue;
			NotificationQueue& receivedQueue;

			void Listen();
			void Send();
		};
	}
}