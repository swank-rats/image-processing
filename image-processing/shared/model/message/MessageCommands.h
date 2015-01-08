//============================================================================
// Name        : MessageCommands.h
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
			enum MessageCommandEnum { invalid, init, start, stop, shot, hit };
			typedef HashMap<string, MessageCommandEnum> StringMsgCommandMap;

			class MessageCommands {
			public:
				MessageCommands();
				MessageCommandEnum GetCommandEnum(const string& command);
				string GetCommandEnumString(const MessageCommandEnum command);
			private:
				StringMsgCommandMap map;
			};
		}
	}
}