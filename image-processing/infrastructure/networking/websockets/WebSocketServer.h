//============================================================================
// Name        : WebSocketServer.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\ServerSocket.h>
#include <Poco\ThreadPool.h>
#include <Poco\Logger.h>

namespace infrastructure {
	namespace websocket {
		class WebSocketServer
		{
		public:
			WebSocketServer();
			void StartServer();
			void StopServer();

		private:
			Poco::Net::HTTPServer* server;
			Poco::Net::ServerSocket socket;
			Poco::ThreadPool threadPool;

			static Poco::Logger& logger;
		};
	}
}