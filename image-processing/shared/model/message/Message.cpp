//============================================================================
// Name        : Message.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "Message.h"
#include "MessageHeaders.h"

#include <Poco\HashMap.h>
#include <Poco\Logger.h>
#include <Poco\Exception.h>
#include <Poco\JSON\Parser.h>
#include <Poco\Dynamic\Var.h>
#include <Poco\JSON\Object.h>

#include <string>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;
using Poco::HashMap;
using Poco::Logger;
using Poco::Dynamic::Var;
using Poco::JSON::Parser;
using Poco::JSON::Object;
using shared::model::message::MessageHeaders;
using shared::model::message::MessageHeaderEnum;

namespace shared {
	namespace model {
		namespace message {
			Message::Message(const MessageCommandEnum cmd, const string& to, StringMap *params, const string& data)
				: cmd(cmd), to(to), data(data) {
				this->params = params == nullptr ? new StringMap() : params;
			}

			Message::Message(const Message& msg) : to(msg.to), cmd(msg.cmd), params(msg.params), data(msg.data) { }

			Message::Message(const Message* msg) : to(msg->to), cmd(msg->cmd), params(msg->params), data(msg->data) { }

			Message::~Message() {
				delete params;
				params = nullptr;
			}

			string Message::GetTo() const {
				return to;
			}

			MessageCommandEnum Message::GetCmd() const {
				return cmd;
			}

			string Message::GetData() const {
				return data;
			}

			bool Message::GetParam(const string name, string& out) {
				StringMap::Iterator iter = params->find(name);

				if (iter == params->end()) {
					return false;
				}

				out = iter->second;

				return true;
			}

			void Message::AddParam(const string& key, const string& value) {
				params->insert(StringMap::ValueType(key, value));
			}

			string Message::ToString() const {
				static MessageCommands commands = MessageCommands();

				stringstream stream;
				string comma = ", ";
				bool addComma = false;

				stream << "{ ";

				if (!to.empty()) {
					stream << "\"to\":\"" << to << "\"";
					addComma = true;
				}

				if (addComma) {
					stream << comma;
				}

				stream << "\"cmd\":\"" << commands.GetCommandEnumString(cmd) << "\"";
				addComma = true;

				if (!params->empty()) {
					if (addComma) {
						stream << comma;
					}

					stream << "\"params\": { ";

					StringMap::Iterator iter = params->begin();
					while (iter != params->end())
					{
						stream << "\"" << iter->first << "\":" << "\"" << iter->second << "\"";

						++iter;

						if (iter != params->end()) {
							stream << ",";
						}
					}

					stream << " }";

					addComma = true;
				}

				if (!data.empty()) {
					if (addComma) {
						stream << comma;
					}

					stream << "\"data\":\"" << data << "\"";
				}

				stream << " }";
				return stream.str();
			}

			Message* Message::Parse(const string& message) {
				Logger& logger = Logger::get("WebSocketMessage");
				static MessageHeaders headers = MessageHeaders();
				static MessageCommands commands = MessageCommands();

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
					Message* tempMessage = new Message();

					vector<string> names;
					root->getNames(names);

					Object::Ptr params;
					vector<string> paramsNames;

					for (vector<string>::size_type i = 0; i != names.size(); i++) {
						switch (headers.GetHeaderEnum(names[i]))
						{
						case MessageHeaderEnum::to:
							tempMessage->to = root->getValue<string>(names[i]);
							break;
						case MessageHeaderEnum::cmd:
							tempMessage->cmd = commands.GetCommandEnum(root->getValue<string>(names[i]));
							break;
						case MessageHeaderEnum::params:
							params = root->getObject(names[i]);
							params->getNames(paramsNames);

							for (vector<string>::size_type j = 0; j != paramsNames.size(); j++) {
								tempMessage->AddParam(paramsNames[j], params->getValue<string>(paramsNames[j]));
							}
							break;
						case MessageHeaderEnum::data:
							tempMessage->data = root->getValue<string>(names[i]);
							break;
						default:
							break;
						}
					}

					return tempMessage;
				}
				catch (Poco::Exception& e)  {
					logger.warning(e.displayText());
				}

				return nullptr;
			}
		}
	}
}