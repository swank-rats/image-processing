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

#include "fenix/poco/Foundation/include/Poco/URI.h"
#include "fenix/poco/Foundation/include/Poco/NotificationQueue.h"
#include "fenix/poco/Foundation/include/Poco/Activity.h"
#include "fenix/poco/Foundation/include/Poco/NObserver.h"
#include "fenix/poco/Foundation/include/Poco/NotificationCenter.h"
#include "fenix/poco/Foundation/include/Poco/AutoPtr.h"

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
			WebSocketController(URI uri);
			~WebSocketController();
			void StartWebSocketClient();
			void StopWebSocketClient();
			NotificationCenter& GetNotificationCenter();
			void Send(Message* message);
		private:
			WebSocketClient* webSocketClient;
			NotificationQueue receivedMessagesQueue;
			Activity<WebSocketController> messageHandlerActivity;
			NotificationCenter notificationCenter;

			void HandleReceivedMessages();
		};
	}
}