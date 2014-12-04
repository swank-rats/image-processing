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

#include <Poco\URI.h>
#include <Poco\Net\Context.h>
#include <Poco\NotificationQueue.h>
#include <Poco\Activity.h>
#include <Poco\NObserver.h>
#include <Poco\NotificationCenter.h>
#include <Poco\AutoPtr.h>

using Poco::URI;
using Poco::Net::Context;
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
			WebSocketController(URI uri, Context::Ptr context);
			~WebSocketController();
			void StartWebSocketClient();
			void StopWebSocketClient();
			NotificationCenter& GetNotificationCenter();
			void Send(const Message* message);

			typedef void(*MessageCallback)(const AutoPtr<MessageNotification>&);

			template<class T>
			void AddMessageOberver(T target, void(*callback)(const AutoPtr<MessageNotification>&)) {
				notificationCenter.addObserver(NObserver<T, MessageNotification>(target, callback));
			}

			template<class T>
			void RemoveMessageOberver(T target, void(*callback)(const AutoPtr<MessageNotification>&)) {
				notificationCenter.removeObserver(NObserver<T, MessageNotification>(target, callback));
			}

		private:
			WebSocketClient* webSocketClient;
			NotificationQueue receivedMessagesQueue;
			Activity<WebSocketController> messageHandlerActivity;
			NotificationCenter notificationCenter;

			void HandleReceivedMessages();
		};
	}
}