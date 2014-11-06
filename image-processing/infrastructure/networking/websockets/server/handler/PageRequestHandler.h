//============================================================================
// Name        : PageRequestHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\Net\HTTPServerRequest.h>
#include <Poco\Net\HTTPServerResponse.h>
#include <Poco\Net\HTTPRequestHandler.h>

namespace infrastructure {
	namespace websocket {
		class PageRequestHandler : public Poco::Net::HTTPRequestHandler
		{
		public:
			PageRequestHandler();
			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		};
	}
}

