//============================================================================
// Name        : WebSocketController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "WebSocketController.h"

namespace controller {
	namespace websocket {
		WebSocketController::WebSocketController()
		{
			webSocketServer = new WebSocketServer;
		}

		void WebSocketController::StartWebSocketServer() {
			webSocketServer->StartServer();
		}

		void WebSocketController::StopWebSocketServer() {
			webSocketServer->StopServer();
		}

	}
}