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
#include <Poco\NObserver.h>
#include <Poco\NotificationCenter.h>
#include <Poco\AutoPtr.h>

#include "..\infrastructure\networking\websockets\client\WebSocketClient.h"
#include "..\infrastructure\networking\websockets\message\Message.h"
#include "..\infrastructure\networking\websockets\message\MessageNotification.h"

using Poco::URI;
using Poco::Net::Context;
using Poco::AutoPtr;
using Poco::NotificationQueue;
using Poco::NotificationCenter;
using Poco::NObserver;
using Poco::Activity;
using infrastructure::websocket::MessageNotification;
using infrastructure::websocket::WebSocketClient;

namespace controller {
	namespace websocket {
		class WebSocketController
		{
		public:
			WebSocketController(URI uri, Context::Ptr context);
			~WebSocketController();
			void StartWebSocketClient();
			void StopWebSocketClient();

			typedef void(*MessageCallback)(const AutoPtr<MessageNotification>&);
			
			template<class T>
			void AddMessageOberver(T target, void(*callback)(const AutoPtr<MessageNotification>&)) {
				notificationCenter.addObserver(NObserver<T, MessageNotification>(target, callback));
			}

			template<class T>
			void RemoveMessageOberver(T target, void(*callback)(const AutoPtr<MessageNotification>&)) {
				notificationCenter.removeObserver(NObserver<T, MessageNotification>(target, callback));
			}

			NotificationCenter& GetNotificationCenter();
		private:
			WebSocketClient* webSocketClient;
			NotificationQueue receivedMessagesQueue;
			Activity<WebSocketController> messageHandlerActivity;
			NotificationCenter notificationCenter;

			void HandleReceivedMessages();
		};

	}
}