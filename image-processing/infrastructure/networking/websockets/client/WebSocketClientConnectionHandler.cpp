//============================================================================
// Name        : WebSocketClientConnectionHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <string>
#include <Poco\Logger.h>
#include <Poco\Exception.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPResponse.h>
#include <Poco\Net\HTTPCredentials.h>

#include "WebSocketClientConnectionHandler.h"
#include "..\WebSocketMessage.h"

using std::string;
using Poco::Logger;
using Poco::URI;
using Poco::TimeoutException;
using Poco::Exception;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPCredentials;

using infrastructure::websocket::WebSocketMessage;

namespace infrastructure {
	namespace websocket {

		WebSocketClientConnectionHandler::WebSocketClientConnectionHandler(URI uri, Context::Ptr context)
			: uri(uri), context(context), receiveActity(this, &WebSocketClientConnectionHandler::Listen),
			session(uri.getHost(), uri.getPort(), context), webSocket(nullptr), timeout(1000)
		{
		}

		URI WebSocketClientConnectionHandler::GetURI() {
			return uri;
		}

		void WebSocketClientConnectionHandler::OpenConnection() {
			Logger& logger = Logger::get("WebSocketClient");

			try {
				//establish connection
				HTTPRequest req(HTTPRequest::HTTP_GET, uri.getPath(), HTTPMessage::HTTP_1_1);
				HTTPResponse res;
				HTTPCredentials creds("user", "s3cr3t"); //add to websocket ctor as parameter with correct data

				logger.information("Connecting to " + uri.getHost() + ":" + std::to_string(uri.getPort()) + uri.getPath());

				webSocket = new WebSocket(session, req, res);

				logger.information("Response status: " + std::to_string(res.getStatus()) + " reason: " + res.getReason());

				if (res.getStatus() == res.HTTP_SWITCHING_PROTOCOLS) {
					webSocket->setReceiveTimeout(timeout);
					webSocket->setSendTimeout(timeout);

					logger.information("Connection established");
					receiveActity.start();
				}
			}
			catch (Exception& e) {
				logger.error(e.displayText());
			}
		}

		void WebSocketClientConnectionHandler::CloseConnection() {
			Logger& logger = Logger::get("WebSocketClient");

			try {
				if (receiveActity.isRunning()) {
					receiveActity.stop();
					receiveActity.wait();
				}
				else {
					logger.information("websocket activity was not running");
				}

				if (session.connected()) {
					webSocket->shutdown();
				}
			}
			catch (Exception& e) {
				logger.error(e.displayText());
			}
		}

		void WebSocketClientConnectionHandler::Send(WebSocketMessage &message) {
			//TODO since this should be done async and the response should be evaluated 
			//add event,.. for notification of success/failure
			//run in own task
		}

		void WebSocketClientConnectionHandler::Listen() {
			Logger& logger = Logger::get("WebSocketClient");

			char buffer[1024];
			int flags = WebSocket::FRAME_TEXT;
			int length;

			while (!receiveActity.isStopped()) {
				try {
					if (webSocket->poll(timeout, WebSocket::SELECT_READ || WebSocket::SELECT_ERROR)) {
						length = webSocket->receiveFrame(buffer, sizeof(buffer), flags);

						if (length > 0) {
							string receivedMessage = string(buffer, length);
							logger.information(receivedMessage);
							WebSocketMessage* message = WebSocketMessage::Parse(receivedMessage);
							//TODO notify WebSocketClient about new received message
						}
						else {
							logger.error("Connection was closed by server!");
							break;
						}
					}
				}
				catch (TimeoutException) {
					logger.information("nothing received");
				}
				catch (Exception& e)
				{
					logger.error(e.displayText());

					if (!session.connected()) {
						logger.error("Connection was closed!");
						break;
					}
				}
			}
		}
	}
}