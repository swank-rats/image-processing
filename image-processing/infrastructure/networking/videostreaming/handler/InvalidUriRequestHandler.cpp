//============================================================================
// Name        : InvalidUriRequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <Poco\Logger.h>

#include "InvalidUriRequestHandler.h"

namespace infrastructure {
	namespace video_streaming {
		InvalidUriRequestHandler::InvalidUriRequestHandler()
		{
		}


		void InvalidUriRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
			Poco::Logger& logger = Poco::Logger::get("InvalidUriRequestHandler");
			logger.information("Invalid uri call by " + request.clientAddress().toString());

			response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.send();
		}
	}
}