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
#include <Poco\Net\NetException.h>
#include <Poco\AutoPtr.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPResponse.h>

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
using Poco::Net::ConnectionAbortedException;
using Poco::Net::ConnectionResetException;

using shared::notifications::MessageNotification;

namespace infrastructure {
	namespace websocket {
		WebSocketClientConnectionHandler::WebSocketClientConnectionHandler(URI uri, NotificationQueue &receivedQueue, NotificationQueue &sendingQueue)
			: uri(uri), session(uri.getHost(), uri.getPort()), webSocket(nullptr), timeout(100),
			receiveActity(this, &WebSocketClientConnectionHandler::Listen), sendActity(this, &WebSocketClientConnectionHandler::Send),
			receivedQueue(receivedQueue), sendingQueue(sendingQueue) {
			isConnected = false;
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

		bool WebSocketClientConnectionHandler::OpenConnection() {
			Logger& logger = Logger::get("WebSocketClient");
			HTTPResponse res;
			res.setKeepAlive(true);

			logger.information("Connecting to " + uri.getScheme() + "://" + uri.getHost() + ":" + std::to_string(uri.getPort()) + uri.getPath());

			try {
				//establish connection
				HTTPRequest req(HTTPRequest::HTTP_GET, uri.getPath(), HTTPMessage::HTTP_1_1);
				webSocket = new WebSocket(session, req, res);

				logger.information("Response status: " + std::to_string(res.getStatus()) + " reason: " + res.getReason());

				if (res.getStatus() == res.HTTP_SWITCHING_PROTOCOLS) {
					webSocket->setReceiveTimeout(timeout);
					webSocket->setSendTimeout(timeout);
					webSocket->setKeepAlive(true);

					logger.information("Connection established");

					isConnected = true;

					receiveActity.start();
					sendActity.start();
					return true;
				}
			}
			catch (Exception& e) {
				logger.error(e.displayText());
				logger.error("Error code: " + std::to_string(e.code()));
				isConnected = false;
				return false;
			}

			return false;
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

				if (isConnected) {
					webSocket->shutdown();
					isConnected = false;
				}
			}
			catch (Exception& e) {
				logger.error(e.displayText());
			}
		}

		bool WebSocketClientConnectionHandler::IsConnected() {
			return isConnected;
		}

		void WebSocketClientConnectionHandler::Send() {
			Logger& logger = Logger::get("WebSocketClient");
			int flags = WebSocket::FRAME_TEXT;
			int length;
			const char* buffer;

			while (!sendActity.isStopped()) {
				Notification::Ptr notification(sendingQueue.waitDequeueNotification());

				if (notification) {
					MessageNotification::Ptr messageNotification = notification.cast<MessageNotification>();

					if (messageNotification && isConnected)
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
						catch (TimeoutException& e) {
							logger.error("Websocket connection timeout: " + e.displayText());
						}
						catch (ConnectionAbortedException& e) {
							logger.error("Websocket connection Aborted: " + e.displayText());
						}
						catch (ConnectionResetException& e) {
							logger.error("Websocket connection Reset: " + e.displayText());
						}
						catch (Exception& e)
						{
							logger.error("Websocket general exception: " + e.displayText());
						}
					}
				}
				else {
					//null signals that worker should stop polling queue
					sendActity.stop();
					break;
				}
			}
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
							Message* message = Message::Parse(receivedMessage);

							logger.information("Received message: " + message->ToString());

							if (message) {
								receivedQueue.enqueueNotification(new MessageNotification(message));
							}
						}
						else {
							logger.error("Connection was closed by server!");
						}
					}
				}
				catch (TimeoutException& e) {
					logger.error("Websocket connection timeout: " + e.displayText());
				}
				catch (ConnectionAbortedException& e) {
					logger.error("Websocket connection Aborted: " + e.displayText());
				}
				catch (ConnectionResetException& e) {
					logger.error("Websocket connection Reset: " + e.displayText());
				}
				catch (Exception& e)
				{
					logger.error("Websocket general exception: " + e.displayText());
				}
			}
		}
	}
}