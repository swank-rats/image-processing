//============================================================================
// Name        : WebSocketController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "WebSocketController.h"

namespace controller {
	namespace websocket {
		WebSocketController::WebSocketController(URI uri) : webSocketClient(new WebSocketClient(uri))
		{
		}

		void WebSocketController::StartWebSocketClient() {
			webSocketClient->OpenConnection();
		}

		void WebSocketController::StopWebSocketClient() {
			webSocketClient->CloseConnection();
		}
	}
}