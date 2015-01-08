//============================================================================
// Name        : MessageNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\model\message\Message.h"

#include <Poco\Notification.h>

using Poco::Notification;
using shared::model::message::Message;

namespace shared {
	namespace notifications {
		class MessageNotification : public Notification
		{
		public:
			MessageNotification(Message *data);
			~MessageNotification();
			Message* GetData();
		private:
			Message *data;
		};
	}
}