//============================================================================
// Name        : WebSocketController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include "..\infrastructure\networking\websockets\server\WebSocketServer.h"

using namespace infrastructure::websocket;
namespace controller {
	namespace websocket {

		class WebSocketController
		{
		public:
			WebSocketController();
			void StartWebSocketServer();
			void StopWebSocketServer();

		private:
			WebSocketServer* webSocketServer;

		};

	}
}