//============================================================================
// Name        : WebSocketServer.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <Poco\Net\HTTPServerParams.h>

#include "WebSocketServer.h"
#include "WebSocketRequestHandlerFactory.h"

namespace infrastructure {
	namespace websocket {
		Poco::Logger& WebSocketServer::logger = Poco::Logger::get("VideoStreamingServer");

		WebSocketServer::WebSocketServer()
			: socket(9980)
		{
			Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
			pParams->setMaxQueued(100);
			pParams->setMaxThreads(16);
			pParams->setKeepAlive(true);

			server = new Poco::Net::HTTPServer(new WebSocketRequestHandlerFactory, threadPool, socket, pParams);
			logger.information("init websocket server " + socket.address().toString() + ":9980");
		}

		void WebSocketServer::StartServer()
		{
			logger.information("starting video streaming server...");
			server->start();
		}

		void WebSocketServer::StopServer() {
			logger.information("stopping video streaming server " + socket.address().toString());
			server->stop();
			threadPool.stopAll();
			logger.information("stopped video streaming server " + socket.address().toString());
		}
	}
}