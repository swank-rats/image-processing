//============================================================================
// Name        : WebSocketMessage.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <Poco\Dynamic\Var.h>
#include <Poco\JSON\Object.h>

#include "WebSocketMessage.h"

using Poco::Dynamic::Var;
using Poco::JSON::Object;

namespace infrastructure {
	namespace websocket {
		Parser WebSocketMessage::parser;

		WebSocketMessage::WebSocketMessage() {
		}

		WebSocketMessage::WebSocketMessage(const string& to, const string& cmd, StringMap* params, const string& data)
			: to(to), cmd(cmd), data(data) {

			this->params = params == nullptr ? new StringMap() : params;
		}

		const string& WebSocketMessage::GetTo() {
			return to;
		}

		void WebSocketMessage::SetTo(const string& to) {
			this->to = to;
		}

		const string& WebSocketMessage::GetCmd() {
			return cmd;
		}

		void WebSocketMessage::SetCmd(const string& cmd) {
			this->cmd = cmd;
		}

		StringMap* WebSocketMessage::GetParams() {
			return params;
		}

		void WebSocketMessage::SetParams(StringMap& params) {
			this->params = &params;
		}

		const string& WebSocketMessage::GetData() {
			return data;
		}

		void WebSocketMessage::SetData(const string& data) {
			this->data = data;
		}

		void WebSocketMessage::AddParam(const string& key, const string& value) {
			params->insert(StringMap::ValueType(key, value));
		}

		WebSocketMessage* WebSocketMessage::Parse(const string& message) {
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
			std::istringstream istr(message);
			parser.parse(istr);

			Object::Ptr root = parser.result().extract<Object::Ptr>();
			string to = root->getValue<string>("to");
			string cmd = root->getValue<string>("cmd");

			Object::Ptr params = root->getObject("params");
			//TODO parse params
			StringMap* map = new StringMap();

			string data = root->getValue<string>("data");

			return new WebSocketMessage(to, cmd, map, data);
		}
	}
}