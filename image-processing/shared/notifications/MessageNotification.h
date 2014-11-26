//============================================================================
// Name        : MessageNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\infrastructure\networking\websockets\message\Message.h"

#include <Poco\Notification.h>

using Poco::Notification;
using infrastructure::websocket::Message;

namespace shared {
	namespace notifications {
		class MessageNotification : public Notification
		{
		public:
			MessageNotification(const Message* data);
			~MessageNotification();
			const Message* GetData();
		private:
			const Message* data;
		};
	}
}