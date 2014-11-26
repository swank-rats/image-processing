//============================================================================
// Name        : MessageNotification.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "MessageNotification.h"

namespace shared {
	namespace notifications {
		MessageNotification::MessageNotification(const Message* data) {
			this->data = data;
		}

		MessageNotification::~MessageNotification() {
			delete data;
			data = nullptr;
		}

		const Message* MessageNotification::GetData()
		{
			return data;
		}
	}
}