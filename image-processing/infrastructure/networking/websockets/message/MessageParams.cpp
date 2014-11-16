//============================================================================
// Name        : MessageParams.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include <string>
#include "MessageParams.h"

using std::string;

namespace infrastructure {
	namespace websocket {
		MessageParams::MessageParams() {
			map.insert(StringMsgParamsMap::ValueType("to", MessageParamsEnum::to));
			map.insert(StringMsgParamsMap::ValueType("cmd", MessageParamsEnum::cmd));
			map.insert(StringMsgParamsMap::ValueType("params", MessageParamsEnum::params));
			map.insert(StringMsgParamsMap::ValueType("data", MessageParamsEnum::data));
		}

		StringMsgParamsMap& MessageParams::GetMap() {
			return map;
		}
	}
}