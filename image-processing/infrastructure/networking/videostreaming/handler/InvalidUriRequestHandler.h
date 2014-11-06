//============================================================================
// Name        : InvalidUriRequestHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\Net\HTTPServerRequest.h>
#include <Poco\Net\HTTPServerResponse.h>
#include <Poco\Net\HTTPRequestHandler.h>
#include <Poco\Logger.h>

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Logger;

namespace infrastructure {
	namespace video_streaming {
		class InvalidUriRequestHandler : public HTTPRequestHandler
		{
		public:
			InvalidUriRequestHandler();
			void handleRequest(HTTPServerRequest& request,HTTPServerResponse& response);
		};
	}
}

