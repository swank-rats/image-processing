//============================================================================
// Name        : MessageHeaders.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "MessageHeaders.h"

namespace shared {
	namespace model {
		namespace message {
			MessageHeaders::MessageHeaders() {
				map.insert(StringMsgHeadersMap::ValueType("to", MessageHeaderEnum::to));
				map.insert(StringMsgHeadersMap::ValueType("cmd", MessageHeaderEnum::cmd));
				map.insert(StringMsgHeadersMap::ValueType("params", MessageHeaderEnum::params));
				map.insert(StringMsgHeadersMap::ValueType("data", MessageHeaderEnum::data));
			}

			StringMsgHeadersMap& MessageHeaders::GetMap() {
				return map;
			}
		}
	}
}