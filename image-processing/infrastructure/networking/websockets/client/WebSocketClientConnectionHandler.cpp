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
#include <Poco\Thread.h>

#include <string>

using std::string;
using Poco::Logger;
using Poco::URI;
using Poco::TimeoutException;
using Poco::Exception;
using Poco::AutoPtr;
using Poco::Thread;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::ConnectionAbortedException;
using Poco::Net::ConnectionResetException;

using shared::notifications::MessageNotification;

namespace infrastructure {
	namespace websocket {
		WebSocketClientConnectionHandler::WebSocketClientConnectionHandler(URI uri, NotificationQueue &receivedQueue)
			: uri(uri), sendTimeout(100), receiveTimeout(5000), pollTimeout(100),
			receiveActity(this, &WebSocketClientConnectionHandler::Listen), receivedQueue(receivedQueue){
			session = nullptr;
			webSocket = nullptr;
			isConnected = false;
			isReconnecting = false;
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
			if (EstablishConnection()) {
				receiveActity.start();
				return true;
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

		bool WebSocketClientConnectionHandler::IsReconnecting() {
			return isReconnecting;
		}

		bool WebSocketClientConnectionHandler::Send(Message& message) {
			Logger& logger = Logger::get("WebSocketClient");
			int flags = WebSocket::FRAME_TEXT;
			int length;
			const char* buffer;

			while (isReconnecting) {
				//wait for reconnecting to be finished to avoid loosing message
				logger.warning("Cannot send message while reconnecting ... waiting for new connection...");

				Thread::sleep(50);
			}

			try {
				string temp = message.ToString();

				buffer = temp.c_str();

				logger.information("try sending " + temp);

				if (!isConnected || isReconnecting) {
					logger.warning("Message was not sent cause of missing connection: " + temp);
					return false;
				}

				length = webSocket->sendFrame(buffer, temp.length(), flags);

				if (length > 0) {
					logger.information("message send!");
					return true;
				}
				else {
					logger.error("message send failed!");
					return false;
				}
			}
			catch (TimeoutException& e) {
				logger.warning("Sending timeout: " + e.displayText());
			}
			catch (ConnectionAbortedException& e) {
				logger.error("Connection aborted: " + e.displayText());
				FireLostConnection();
			}
			catch (ConnectionResetException& e) {
				logger.error("Connection reset: " + e.displayText());
				FireLostConnection();
			}
			catch (Exception& e)
			{
				logger.error("General exception: " + e.displayText());
				FireLostConnection();
			}

			//exception occurred
			return false;
		}

		void WebSocketClientConnectionHandler::Listen() {
			Logger& logger = Logger::get("WebSocketClient");

			char buffer[1024];
			int flags = WebSocket::FRAME_TEXT;
			int length;

			while (!receiveActity.isStopped()) {
				if (isReconnecting) continue;

				try {
					if (webSocket->poll(pollTimeout, WebSocket::SELECT_READ || WebSocket::SELECT_ERROR)) {
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
							FireLostConnection();
						}
					}
				}
				catch (TimeoutException& e) {
					logger.warning("Receiving timeout: " + e.displayText());
				}
				catch (ConnectionAbortedException& e) {
					logger.error("Connection aborted: " + e.displayText());
					FireLostConnection();
				}
				catch (ConnectionResetException& e) {
					logger.error("Connection reset: " + e.displayText());
					FireLostConnection();
				}
				catch (Exception& e)
				{
					logger.error("General exception: " + e.displayText());
					FireLostConnection();
				}
			}
		}

		bool WebSocketClientConnectionHandler::Reconnect() {
			Logger& logger = Logger::get("WebSocketClient");

			isConnected = false;
			isReconnecting = true;

			for (size_t i = 1; i <= 5; i++)
			{
				logger.warning("Try reconnect");

				if (EstablishConnection()) {
					isConnected = true;
					break; //reconnected
				}
				else {
					logger.warning("Reconnect failed; retry in 50ms");
					Thread::sleep(50);
				}
			}

			isReconnecting = false;

			return isConnected;
		}

		bool WebSocketClientConnectionHandler::EstablishConnection() {
			Logger& logger = Logger::get("WebSocketClient");
			HTTPResponse res;

			logger.information("Connecting to " + uri.getScheme() + "://" + uri.getHost() + ":" + std::to_string(uri.getPort()) + uri.getPath());

			try {
				session = new HTTPSClientSession(uri.getHost(), uri.getPort());

				HTTPRequest req(HTTPRequest::HTTP_GET, uri.getPath(), HTTPMessage::HTTP_1_1);
				req.setKeepAlive(true);
				webSocket = new WebSocket(*session, req, res);

				logger.information("Response status: " + std::to_string(res.getStatus()) + " reason: " + res.getReason());

				if (res.getStatus() == res.HTTP_SWITCHING_PROTOCOLS) {
					webSocket->setReceiveTimeout(receiveTimeout);
					webSocket->setSendTimeout(sendTimeout);
					webSocket->setKeepAlive(true);

					logger.information("Connection established");

					isConnected = true;
					return true;
				}
			}
			catch (Exception& e) {
				logger.error(e.displayText());
				logger.error("Error code: " + std::to_string(e.code()));
				isConnected = false;
			}

			return false;
		}

		void WebSocketClientConnectionHandler::FireLostConnection() {
			static int counter = 0;
			LostConnection(this, ++counter);
		}
	}
}