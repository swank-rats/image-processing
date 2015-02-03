//============================================================================
// Name        : InvalidUriRequestHandler.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "fenix/poco/Net/include/Poco/Net/HTTPServerRequest.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPServerResponse.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPRequestHandler.h"
#include "fenix/poco/Foundation/include/Poco/Logger.h"

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
			~InvalidUriRequestHandler();
			void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
		};
	}
}
