//============================================================================
// Name        : MessageNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "Message.h"

#include <Poco\Notification.h>

using Poco::Notification;
using infrastructure::websocket::Message;

namespace infrastructure {
	namespace websocket {
		class MessageNotification : public Notification
		{
		public:
			MessageNotification(Message* data);
			~MessageNotification();
			Message* GetData();
		private:
			Message* data;
		};
	}
}