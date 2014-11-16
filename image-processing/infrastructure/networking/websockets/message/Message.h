//============================================================================
// Name        : Message.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include <string>
#include <Poco\HashMap.h>

using std::string;
using std::vector;
using Poco::HashMap;

namespace infrastructure {
	namespace websocket {
		typedef HashMap<string, string> StringMap;
		
		struct Message
		{
		public:
			Message(const string& cmd, const string& to = "", StringMap *params = new StringMap(), const string& data = "");
			Message(const Message& msg);
			~Message();
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