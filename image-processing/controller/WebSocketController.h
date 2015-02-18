//============================================================================
// Name        : WebSocketController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\infrastructure\networking\websockets\client\WebSocketClient.h"
#include "..\shared\notifications\MessageNotification.h"
#include "..\shared\model\message\Message.h"

#include <Poco\BasicEvent.h>
#include <Poco\URI.h>
#include <Poco\NotificationQueue.h>
#include <Poco\Activity.h>
#include <Poco\NObserver.h>
#include <Poco\NotificationCenter.h>
#include <Poco\AutoPtr.h>

using Poco::BasicEvent;
using Poco::URI;
using Poco::AutoPtr;
using Poco::NotificationQueue;
using Poco::NotificationCenter;
using Poco::NObserver;
using Poco::Activity;
using shared::notifications::MessageNotification;
using infrastructure::websocket::WebSocketClient;
using shared::model::message::Message;

namespace controller {
	namespace websocket {
		class WebSocketController
		{
		public:
			BasicEvent<const Message> MessageReceived;

			WebSocketController(URI uri);
			~WebSocketController();
			void StartWebSocketClient();
			void StopWebSocketClient();
			void Send(Message* message);
		private:
			WebSocketClient* webSocketClient;
			NotificationQueue receivedMessagesQueue;
			Activity<WebSocketController> messageHandlerActivity;

			void HandleReceivedMessages();
		};
	}
}