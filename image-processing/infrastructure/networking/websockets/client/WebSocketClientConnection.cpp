//============================================================================
// Name        : WebSocketClientConnection.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <string>
#include <Poco\Logger.h>
#include <Poco\Net\HTTPSClientSession.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPResponse.h>

#include "WebSocketClientConnection.h"

using Poco::Logger;
using Poco::URI;
using Poco::Net::HTTPSClientSession;
using Poco::Net::WebSocket;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Exception;

namespace infrastructure {
	namespace websocket {
		Poco::Logger& WebSocketClientConnection::logger = Poco::Logger::get("WebSocketClientConnection");

		WebSocketClientConnection::WebSocketClientConnection(URI uri, Context::Ptr context)
			: Task("WebSocketClientConnection"), uri(uri), context(context)
		{
		}

		URI WebSocketClientConnection::GetURI() {
			return uri;
		}

		void WebSocketClientConnection::runTask() {
			WebSocket* webSocket = nullptr;

			try {
				HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
				HTTPRequest req(HTTPRequest::HTTP_GET, uri.getPath(), HTTPMessage::HTTP_1_1);
				HTTPResponse res;

				webSocket = new WebSocket(session, req, res);
				
				char buffer[1024];
				int flags;
				int n;
				std::string payload = "{\"to\":\"test\",\"cmd\":\"echo\",\"params\":{\"toUpper\": true},\"data\":\"testdata\"}";
				
				webSocket->sendFrame(payload.data(), payload.size(), WebSocket::FRAME_TEXT);


				while (!isCancelled()) {
					n = webSocket->receiveFrame(buffer, sizeof(buffer), flags);

					if (n > 0) {
						logger.information(buffer);
					}
				}
			}
			catch (Exception& e) {
				logger.error("WebSocket Client exception: " + e.displayText());
			}

			if (webSocket != nullptr) {
				webSocket->close();
			}
		}
	}
}