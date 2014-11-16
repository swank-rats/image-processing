//============================================================================
// Name        : MessageNotification.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include <Poco\Notification.h>
#include "Message.h"

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