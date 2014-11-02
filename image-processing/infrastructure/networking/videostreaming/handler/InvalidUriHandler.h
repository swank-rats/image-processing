//============================================================================
// Name        : InvalidUriHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\Net\HTTPServerRequest.h>
#include <Poco\Net\HTTPServerResponse.h>
#include <Poco\Net\HTTPRequestHandler.h>

namespace infrastructure {
	namespace video_streaming {
		class InvalidUriHandler : public Poco::Net::HTTPRequestHandler
		{
		public:
			InvalidUriHandler();
			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		};
	}
}

