//============================================================================
// Name        : MessageNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\model\message\Message.h"

#include "fenix/poco/Foundation/include/Poco/Notification.h"
#include "fenix/poco/Foundation/include/Poco/AutoPtr.h"

using Poco::Notification;
using Poco::AutoPtr;
using shared::model::message::Message;

namespace shared {
	namespace notifications {
		class MessageNotification : public Notification
		{
		public:
			typedef AutoPtr<MessageNotification> Ptr;

			MessageNotification(Message *data);
			~MessageNotification();
			Message* GetData();
		private:
			Message *data;
		};
	}
}