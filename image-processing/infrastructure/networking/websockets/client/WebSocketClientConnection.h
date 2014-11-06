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
#include <Poco\Task.h>
#include <Poco\TaskManager.h>

using Poco::Logger;
using Poco::URI;
using Poco::Net::Context;
using Poco::Task;

namespace infrastructure {
	namespace websocket {
		class WebSocketClientConnection : public Task
		{
		public:
			WebSocketClientConnection(URI uri, Context::Ptr context);
			void runTask();
			URI GetURI();
		private:
			URI uri;
			Context::Ptr context;
			static Poco::Logger& logger;
		};
	}
}