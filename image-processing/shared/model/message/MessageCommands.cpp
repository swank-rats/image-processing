//============================================================================
// Name        : MessageCommands.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "MessageCommands.h"

namespace shared {
	namespace model {
		namespace message {
			MessageCommands::MessageCommands() {
				map.insert(StringMsgCommandMap::ValueType("init", MessageCommandEnum::init));
				map.insert(StringMsgCommandMap::ValueType("start", MessageCommandEnum::start));
				map.insert(StringMsgCommandMap::ValueType("stop", MessageCommandEnum::stop));
				map.insert(StringMsgCommandMap::ValueType("shot", MessageCommandEnum::shot));
				map.insert(StringMsgCommandMap::ValueType("hit", MessageCommandEnum::hit));
				map.insert(StringMsgCommandMap::ValueType("connectionlost", MessageCommandEnum::connectionlost));
			}

			MessageCommandEnum MessageCommands::GetCommandEnum(const string& command) {
				StringMsgCommandMap::Iterator it = map.find(command);

				if (it != map.end()) {
					return it->second;
				}

				return MessageCommandEnum::invalid;
			}

			string MessageCommands::GetCommandEnumString(const MessageCommandEnum command) {
				StringMsgCommandMap::Iterator iter = map.begin();

				while (iter != map.end()) {
					if (iter->second == command) {
						return iter->first;
					}

					++iter;
				}

				return "";
			}

		}
	}
}