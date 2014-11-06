//============================================================================
// Name        : WebSocketServer.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\HTTPClientSession.h>
#include <Poco\Net\SecureServerSocket.h>
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
			Poco::Net::HTTPClientSession session;
			Poco::ThreadPool threadPool;

			static Poco::Logger& logger;
		};
	}
}