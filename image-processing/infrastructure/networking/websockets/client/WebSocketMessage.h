//============================================================================
// Name        : WebSocketMessage.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include <string>
#include <sstream>
#include <Poco\HashMap.h>
#include <Poco\Logger.h>
#include <Poco\Exception.h>
#include <Poco\JSON\Parser.h>
#include <Poco\Dynamic\Var.h>
#include <Poco\JSON\Object.h>
#include <Poco\Notification.h>

using std::string;
using std::stringstream;
using std::vector;
using Poco::Notification;
using Poco::HashMap;
using Poco::Logger;
using Poco::Dynamic::Var;
using Poco::JSON::Parser;
using Poco::JSON::Object;

namespace infrastructure {
	namespace websocket {
		typedef HashMap<string, string> StringMap;

		struct WebSocketMessage
		{
		public:
			WebSocketMessage(const string& cmd, const string& to = "", StringMap *params = new StringMap(), const string& data = "")
				: to(to), cmd(cmd), data(data) {

				this->params = params == nullptr ? new StringMap() : params;
			}

			WebSocketMessage(const WebSocketMessage& msg) :to(msg.to), cmd(msg.cmd), params(msg.params), data(msg.data) {
			}

			~WebSocketMessage() {
				delete params;
				params = nullptr;
			}

			void AddParam(const string& key, const string& value) {
				params->insert(StringMap::ValueType(key, value));
			}

			const string ToString() {
				stringstream stream;

				stream << "{";
				if (!to.empty()) {
					stream << "\"to\":\"" << to << "\",\n";
				}

				if (cmd.empty()) {
					//TODO ERROR!
				}

				stream << "\"cmd\":\"" << cmd << "\",\n";

				if (!params->empty()) {
					stream << "\"params\": {\n";

					StringMap::Iterator iter = params->begin();
					while (iter != params->end())
					{
						stream << "\"" << iter->first << "\":" << "\"" << iter->second << "\"";

						++iter;

						if (iter != params->end()) {
							stream << ",";
						}

						stream << "\n";
					}

					stream << "},\n";
				}

				if (!data.empty()) {
					stream << "\"data\":\"" << data << "\"\n";
				}

				stream << "}";

				return stream.str();
			}

			static WebSocketMessage* Parse(const string& message) {
				Logger& logger = Logger::get("WebSocketMessage");

				try {
					/* Protocol (based on JSON object)
					* {
					* 	 to: 'test',
					* 	 cmd: 'echo',
					*   params: {
					*	     toUpper: true
					*	 },
					*	 data: 'testdata'
					* }
					*/
					Parser parser;

					std::istringstream istr(message);
					parser.parse(istr);

					Object::Ptr root = parser.result().extract<Object::Ptr>();
					vector<string> names;

					root->getNames(names);
					
					//TODO parsing message

					//string to = root->getValue<string>("to");
					//string cmd = root->getValue<string>("cmd");

					//Object::Ptr params = root->getObject("params");
					////TODO parse params
					//StringMap* map = new StringMap();

					//string data = root->getValue<string>("data");

					//return new WebSocketMessage(to, cmd, map, data);

					string cmd = "cmd";
					string to = "to";

					return new WebSocketMessage(cmd, to);
				}
				catch (Poco::Exception& e)  {
					logger.warning(e.displayText());
				}

				return nullptr;
			}
		private:
			string to;
			string cmd;
			StringMap* params;
			string data;
		};

		class WebSocketMessageNotification : public Notification
		{
		public:
			WebSocketMessageNotification(WebSocketMessage* data) : data(data) {
			}

			~WebSocketMessageNotification() {
				delete data;
				data = nullptr;
			}

			WebSocketMessage* GetData()
			{
				return data;
			}

		private:
			WebSocketMessage* data;
		};
	}
}