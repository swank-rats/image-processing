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

namespace infrastructure {
	namespace video_streaming {
		class InvalidUriRequestHandler : public Poco::Net::HTTPRequestHandler
		{
		public:
			InvalidUriRequestHandler();
			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		private:
			static Poco::Logger& logger;
		};
	}
}

