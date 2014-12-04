//============================================================================
// Name        : MessageHeaders.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once

#include <string>
#include <Poco\HashMap.h>

using std::string;
using Poco::HashMap;

namespace shared {
	namespace model {
		namespace message {
			enum MessageHeaderEnum { to, cmd, params, data };
			typedef HashMap<string, MessageHeaderEnum> StringMsgHeadersMap;

			class MessageHeaders {
			public:
				MessageHeaders();
				StringMsgHeadersMap& GetMap();
			private:
				StringMsgHeadersMap map;
			};
		}
	}
}