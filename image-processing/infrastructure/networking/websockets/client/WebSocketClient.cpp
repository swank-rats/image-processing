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
		WebSocketClient::WebSocketClient(URI uri, Context::Ptr context) : connection(new WebSocketClientConnection(uri, context))
		{
		}

		void WebSocketClient::OpenConnection() {
			Logger& logger = Logger::get("WebSocketClient");
			URI uri = connection->GetURI();

			logger.information("Opening websocket connection to " + uri.getHost() + ":" + std::to_string(uri.getPort()) + uri.getPath());
			connection->OpenConnection();
		}

		void WebSocketClient::CloseConnection() {
			Logger& logger = Logger::get("WebSocketClient");

			try {
				URI uri = connection->GetURI();

				logger.information("Closing websocket connection to " + uri.getHost() + ":" + std::to_string(uri.getPort()) + uri.getPath());
				connection->CloseConnection();
			}
			catch (Poco::SyntaxException& e) {
				logger.error(e.displayText());
			}
		}
	}
}