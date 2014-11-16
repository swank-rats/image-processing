//============================================================================
// Name        : WebSocketController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\URI.h>
#include <Poco\Net\Context.h>
#include <Poco\NotificationQueue.h>
#include <Poco\Activity.h>

#include "..\infrastructure\networking\websockets\client\WebSocketClient.h"
#include "..\infrastructure\networking\websockets\message\Message.h"
#include "..\infrastructure\networking\websockets\message\MessageNotification.h"

using namespace infrastructure::websocket;
using Poco::URI;
using Poco::Net::Context;
using Poco::NotificationQueue;
using Poco::Activity;

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
			NotificationQueue receivedMessagesQueue;
			Activity<WebSocketController> messageHandlerActivity;

			void HandleReceivedMessages();
		};

	}
}