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
#include <Poco\AutoPtr.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPResponse.h>
#include <Poco\Net\HTTPCredentials.h>

#include "WebSocketClientConnectionHandler.h"

using std::string;
using Poco::Logger;
using Poco::URI;
using Poco::TimeoutException;
using Poco::Exception;
using Poco::AutoPtr;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPCredentials;

using infrastructure::websocket::WebSocketMessage;

namespace infrastructure {
	namespace websocket {

		WebSocketClientConnectionHandler::WebSocketClientConnectionHandler(URI uri, Context::Ptr context, NotificationQueue &receivedQueue, NotificationQueue &sendingQueue)
			: uri(uri), context(context), session(uri.getHost(), uri.getPort(), context), webSocket(nullptr), timeout(1000),
			receiveActity(this, &WebSocketClientConnectionHandler::Listen), sendActity(this, &WebSocketClientConnectionHandler::Send),
			receivedQueue(receivedQueue), sendingQueue(sendingQueue) {
		}

		WebSocketClientConnectionHandler::~WebSocketClientConnectionHandler() {
			if (IsConnected()) {
				CloseConnection();
			}

			delete webSocket;
			webSocket = nullptr;
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
					sendActity.start();
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

				if (sendActity.isRunning()) {
					sendActity.stop();
					sendActity.wait();
				}

				if (session.connected()) {
					webSocket->shutdown();
				}				
			}
			catch (Exception& e) {
				logger.error(e.displayText());
			}
		}

		bool WebSocketClientConnectionHandler::IsConnected() {
			return session.connected();
		}

		void WebSocketClientConnectionHandler::Send() {
			Logger& logger = Logger::get("WebSocketClient");
			int flags = WebSocket::FRAME_TEXT;
			int length;
			const char* buffer;
			AutoPtr<Notification> notification(sendingQueue.waitDequeueNotification());

			while (!sendActity.isStopped() && notification) {
				WebSocketMessageNotification* messageNotification = dynamic_cast<WebSocketMessageNotification*>(notification.get());
				if (messageNotification)
				{
					try {
						//TODO WTF WHY DOES buffer = messageNotification->GetData().ToString().c_str() NOT WORK??
						WebSocketMessage* message = messageNotification->GetData(); //take ownership
						string temp2 = message->ToString().c_str();
						string temp = message->ToString();
						buffer = temp.c_str();

						length = webSocket->sendFrame(buffer, temp.length(), flags);

						logger.information((length > 0) ? "message send!" : "message send failed");

						//cleanup
						delete message;
						message = nullptr;
					}
					catch (TimeoutException) {
						logger.error("send failed cause of timeout");
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

				notification = sendingQueue.waitDequeueNotification();
			}
		}

		void WebSocketClientConnectionHandler::Listen() {
			Logger& logger = Logger::get("WebSocketClient");

			char buffer[1024]; //TODO set correct buffer size
			int flags = WebSocket::FRAME_TEXT;
			int length;

			while (!receiveActity.isStopped()) {
				try {
					if (webSocket->poll(timeout, WebSocket::SELECT_READ || WebSocket::SELECT_ERROR)) {
						length = webSocket->receiveFrame(buffer, sizeof(buffer), flags);

						if (length > 0) {
							string receivedMessage = string(buffer, length);
							WebSocketMessage* message = WebSocketMessage::Parse(receivedMessage);

							if (message) {
								receivedQueue.enqueueNotification(new WebSocketMessageNotification(message));
							}
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