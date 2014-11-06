//============================================================================
// Name        : WebSocketRequestHandlerFactory.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <string>
#include <Poco\Net\HTTPRequestHandlerFactory.h>
#include <Poco\Net\HTTPServerRequest.h>

#include "..\..\..\..\services\videostreaming\webcam\WebcamService.h"

namespace infrastructure {
	namespace websocket {
		class WebSocketRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
		{
		public:
			WebSocketRequestHandlerFactory();
			Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
		private:
		};
	}
}