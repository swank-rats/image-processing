//============================================================================
// Name        : WebSocketController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\URI.h>
#include <Poco\Net\Context.h>
#include "..\infrastructure\networking\websockets\client\WebSocketClient.h"

using namespace infrastructure::websocket;
using Poco::URI;
using Poco::Net::Context;

namespace controller {
	namespace websocket {

		class WebSocketController
		{
		public:
			WebSocketController(URI uri, Context::Ptr context);
			~WebSocketController();
			void StartWebSocketClient();
			void StopWebSocketClient();
		private:
			WebSocketClient* webSocketClient;
		};

	}
}