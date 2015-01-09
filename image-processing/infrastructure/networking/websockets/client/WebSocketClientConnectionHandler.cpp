//============================================================================
// Name        : WebSocketClientConnectionHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "WebSocketClientConnectionHandler.h"
#include "..\..\..\..\shared\\notifications\MessageNotification.h"

#include <Poco\Logger.h>
#include <Poco\Exception.h>
#include <Poco\AutoPtr.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPResponse.h>
#include <Poco\Net\HTTPCredentials.h>

#include <string>

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

using shared::notifications::MessageNotification;

namespace infrastructure {
	namespace websocket {
		WebSocketClientConnectionHandler::WebSocketClientConnectionHandler(URI uri, NotificationQueue &receivedQueue, NotificationQueue &sendingQueue)
			: uri(uri), session(uri.getHost(), uri.getPort()), webSocket(nullptr), timeout(1000),
			receiveActity(this, &WebSocketClientConnectionHandler::Listen), sendActity(this, &WebSocketClientConnectionHandler::Send),
			receivedQueue(receivedQueue), sendingQueue(sendingQueue) { }

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

		bool WebSocketClientConnectionHandler::OpenConnection() {
			Logger& logger = Logger::get("WebSocketClient");
			HTTPResponse res;

			try {
				//establish connection
				HTTPRequest req(HTTPRequest::HTTP_GET, uri.getPath(), HTTPMessage::HTTP_1_1);
				HTTPCredentials creds("user", "s3cr3t"); //add to websocket ctor as parameter with correct data

				logger.information("Connecting to " + uri.getScheme() + "://" + uri.getHost() + ":" + std::to_string(uri.getPort()) + uri.getPath());

				webSocket = new WebSocket(session, req, res);

				logger.information("Response status: " + std::to_string(res.getStatus()) + " reason: " + res.getReason());

				if (res.getStatus() == res.HTTP_SWITCHING_PROTOCOLS) {
					webSocket->setReceiveTimeout(timeout);
					webSocket->setSendTimeout(timeout);

					logger.information("Connection established");
					receiveActity.start();
					sendActity.start();

					return true;
				}

				return false;
			}
			catch (Exception& e) {
				logger.error(e.displayText());
				logger.error("Error code: " + std::to_string(e.code()));
				return false;
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
				MessageNotification* messageNotification = dynamic_cast<MessageNotification*>(notification.get());
				if (messageNotification)
				{
					try {
						const Message &message = messageNotification->GetData();
						string temp = message.ToString();
						buffer = temp.c_str();

						logger.information("sending " + temp);

						length = webSocket->sendFrame(buffer, temp.length(), flags);

						logger.information((length > 0) ? "message send!" : "message send failed");
						messageNotification = nullptr;
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
							Message* message = Message::Parse(receivedMessage);

							logger.information("Received message: " + message->ToString());

							if (message) {
								receivedQueue.enqueueNotification(new MessageNotification(message));
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