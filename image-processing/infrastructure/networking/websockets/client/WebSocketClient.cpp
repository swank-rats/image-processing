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

using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

namespace infrastructure {
	namespace websocket {
		WebSocketClient::WebSocketClient(URI uri, Context::Ptr context) : connHandler(new WebSocketClientConnectionHandler(uri, context))
		{
		}

		void WebSocketClient::OpenConnection() {
			connHandler->OpenConnection();
		}

		void WebSocketClient::CloseConnection() {
			connHandler->CloseConnection();
		}
	}
}