//============================================================================
// Name        : MessageNotification.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "MessageNotification.h"

namespace infrastructure {
	namespace websocket {
		MessageNotification::MessageNotification(Message* data) : data(data) { }

		MessageNotification::~MessageNotification() {
			delete data;
			data = nullptr;
		}

		Message* MessageNotification::GetData()
		{
			return data;
		}
	}
}