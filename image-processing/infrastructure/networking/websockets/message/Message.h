//============================================================================
// Name        : Message.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include <string>
#include <Poco\HashMap.h>
#include "MessageHeaders.h"

using std::string;
using std::vector;
using Poco::HashMap;

namespace infrastructure {
	namespace websocket {
		typedef HashMap<string, string> StringMap;
		
		class Message
		{
		public:
			Message(const string& cmd, const string& to = "", StringMap *params = new StringMap(), const string& data = "");
			Message(const Message& msg);
			~Message();
			string GetTo();
			string GetCmd();
			string GetData();
			
			bool Message::GetParam(const string name, string& out);
			void AddParam(const string& key, const string& value);
			string ToString();
			
			static Message* Parse(const string& message);
		private:
			Message() : Message("") {}
			string to;
			string cmd;
			StringMap* params;
			string data;
		};
	}
}