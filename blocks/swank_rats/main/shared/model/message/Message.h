//============================================================================
// Name        : Message.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "MessageHeaders.h"
#include "MessageCommands.h"

#include "fenix/poco/Foundation/include/Poco/HashMap.h"

#include <string>

using std::string;
using std::vector;
using Poco::HashMap;
using shared::model::message::MessageCommands;

namespace shared {
	namespace model {
		namespace message {
			typedef HashMap<string, string> StringMap;

			class Message
			{
			public:
				Message(const MessageCommandEnum cmd, const string& to = "", StringMap *params = new StringMap(), const string& data = "");
				Message(const Message& msg);
				Message(const Message* msg);

				~Message();
				string GetTo() const;
				MessageCommandEnum GetCmd() const;
				string GetData() const;

				bool Message::GetParam(const string name, string& out) const;
				void AddParam(const string& key, const string& value);
				string ToString() const;

				static Message* Parse(const string& message);
			private:
				Message() : Message(MessageCommandEnum::invalid) {}
				string to;
				MessageCommandEnum cmd;
				StringMap *params;
				string data;
			};
		}
	}
}