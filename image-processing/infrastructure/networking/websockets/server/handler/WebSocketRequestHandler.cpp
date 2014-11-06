//============================================================================
// Name        : WebSocketRequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <Poco\Logger.h>
#include <Poco\Net\WebSocket.h>
#include <Poco\Net\NetException.h>
#include "WebSocketRequestHandler.h"

namespace infrastructure {
	namespace websocket {
		WebSocketRequestHandler::WebSocketRequestHandler()
		{
		}


		void WebSocketRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
			Poco::Logger& logger = Poco::Logger::get("WebSocketRequestHandler");

			try
			{
				Poco::Net::WebSocket ws(request, response);
				logger.information("WebSocket connection established.");
				char buffer[1024];
				int flags;
				int n;
				do
				{
					n = ws.receiveFrame(buffer, sizeof(buffer), flags);
					logger.information(Poco::format("Frame received (length=%d, flags=0x%x).", n, unsigned(flags)));
					ws.sendFrame(buffer, n, flags);
				} while (n > 0 || (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);
				logger.information("WebSocket connection closed.");
			}
			catch (Poco::Net::WebSocketException& exc)
			{
				logger.log(exc);
				switch (exc.code())
				{
				case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
					response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
					// fallthrough
				case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
				case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
				case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
					response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
					response.setContentLength(0);
					response.send();
					break;
				}
			}
		}
	}
}