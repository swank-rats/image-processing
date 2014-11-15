//============================================================================
// Name        : WebSocketController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "WebSocketController.h"

namespace controller {
	namespace websocket {
		WebSocketController::WebSocketController(URI uri, Context::Ptr context) : webSocketClient(new WebSocketClient(uri, context))
		{
		}

		WebSocketController::~WebSocketController() {
			delete webSocketClient;
			webSocketClient = nullptr;
		}

		void WebSocketController::StartWebSocketClient() {
			webSocketClient->OpenConnection();
		}

		void WebSocketController::StopWebSocketClient() {
			webSocketClient->CloseConnection();
		}

		void WebSocketController::HandleReceivedMessage(WebSocketMessage* message) {

			
		}
	}
}