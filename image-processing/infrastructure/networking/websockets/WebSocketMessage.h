//============================================================================
// Name        : WebSocketMessage.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include <string>
#include <Poco\HashMap.h>
#include <Poco\JSON\Parser.h>

using std::string;
using std::vector;
using Poco::JSON::Parser;
using Poco::HashMap;

namespace infrastructure {
	namespace websocket {
		typedef HashMap<string, string> StringMap;

		class WebSocketMessage
		{
		public:

			WebSocketMessage();
			WebSocketMessage(const string& to, const string& cmd, StringMap *params = new StringMap(), const string& data = 0);

			const string& GetTo();
			void SetTo(const string& to);
			const string& GetCmd();
			void SetCmd(const string& cmd);
			StringMap* GetParams();
			void SetParams(StringMap& params);
			const string& GetData();
			void SetData(const string& data);

			void AddParam(const string& key, const string& value);

			static WebSocketMessage* Parse(const string& message);
		private:
			string to;
			string cmd;
			StringMap* params;
			string data;

			static Parser parser;
		};

	}
}