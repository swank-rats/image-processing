//============================================================================
// Name        : WebSocketClient.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <string>

#include <Poco\Exception.h>
#include <Poco\Net\HTTPSClientSession.h>
#include <Poco\Net\HTTPMessage.h>
#include <Poco\Net\HTTPRequest.h>
#include <Poco\Net\HTTPResponse.h>
#include <Poco\Net\WebSocket.h>

#include "WebSocketClient.h"

using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::WebSocket;


namespace infrastructure {
	namespace websocket {
		WebSocketClient::WebSocketClient()
		{
		}

		void WebSocketClient::OpenConnection() {
			char buffer[1024];
			int flags;
			int n;
			std::string payload;

			try
			{
				HTTPSClientSession session("172.16.50.41", 3001);
				HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, "/", Poco::Net::HTTPMessage::HTTP_1_1);
				HTTPResponse res;
				std::string cmd;

				WebSocket * ws = new WebSocket(session, req, res);

				payload = "{\"to\":\"test\",\"cmd\":\"echo\",\"params\":{\"toUpper\": true},\"data\":\"testdata\"}";
				ws->sendFrame(payload.data(), payload.size(), WebSocket::FRAME_TEXT);
				n = ws->receiveFrame(buffer, sizeof(buffer), flags);

				while (cmd != "exit")
				{
					cmd = "";
					std::cin >> cmd;
					ws->sendFrame(cmd.data(), cmd.size(), WebSocket::FRAME_TEXT);
					n = ws->receiveFrame(buffer, sizeof(buffer), flags);
					if (n > 0)
					{
						std::cout << buffer << std::endl;
					}
				}

				ws->shutdown();
			}
			catch (Poco::Exception& e)
			{
				std::cerr << "Exception: " << e.what() << std::endl;
				std::cerr << "Message: " << e.message() << std::endl;
				std::cerr << e.displayText() << std::endl;
			}
		}

		void WebSocketClient::CloseConnection() {

		}
	}
}