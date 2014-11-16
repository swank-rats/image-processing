//============================================================================
// Name        : MessageParams.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include <string>
#include <Poco\HashMap.h>

using std::string;
using Poco::HashMap;

namespace infrastructure {
	namespace websocket {
		enum MessageParamsEnum { to, cmd, params, data };
		typedef HashMap<string, MessageParamsEnum> StringMsgParamsMap;

		class MessageParams {
		public:
			MessageParams();
			StringMsgParamsMap& GetMap();
		private:
			StringMsgParamsMap map;
		};
	}
}