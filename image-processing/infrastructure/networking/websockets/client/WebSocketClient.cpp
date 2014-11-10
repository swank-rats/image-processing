//============================================================================
// Name        : WebSocketClient.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <string>

#include <Poco\Exception.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPResponse.h>

#include "WebSocketClient.h"
#include "WebSocketMessage.h"

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPResponse;

using infrastructure::websocket::StringMap;

namespace infrastructure {
	namespace websocket {
		WebSocketClient::WebSocketClient(URI uri, Context::Ptr context) : 
			connHandler(new WebSocketClientConnectionHandler(uri, context, queue))
		{
		}

		void WebSocketClient::OpenConnection() {
			connHandler->OpenConnection();

			StringMap* map = new StringMap();
			map->insert(StringMap::ValueType("param1", "test"));
			Send(WebSocketMessage("t", "c", map, "d"));
		}

		void WebSocketClient::CloseConnection() {
			connHandler->CloseConnection();
		}

		void WebSocketClient::Send(WebSocketMessage message) {
			queue.enqueueNotification(new WebSocketMessageNotification(message));
		}
	}
}