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
			enum MessageHeaderEnum { unknown, to, cmd, params, data };
			typedef HashMap<string, MessageHeaderEnum> StringMsgHeadersMap;

			class MessageHeaders {
			public:
				MessageHeaders();
				MessageHeaderEnum GetHeaderEnum(const string& header);
			private:
				StringMsgHeadersMap map;
			};
		}
	}
}