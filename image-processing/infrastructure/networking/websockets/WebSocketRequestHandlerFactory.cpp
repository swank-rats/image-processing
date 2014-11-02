#include "WebSocketRequestHandlerFactory.h"
#include "handler\PageRequestHandler.h"
#include "handler\WebSocketRequestHandler.h"

namespace infrastructure {
	namespace websocket {
		WebSocketRequestHandlerFactory::WebSocketRequestHandlerFactory()
		{
		}

		Poco::Net::HTTPRequestHandler* WebSocketRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
			if (request.find("Upgrade") != request.end() && request["Upgrade"] == "websocket") {
				return new WebSocketRequestHandler;
			}
			else {
				return new PageRequestHandler;
			}
		}
	}
}