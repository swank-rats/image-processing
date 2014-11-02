//============================================================================
// Name        : InvalidUriHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <Poco\Logger.h>

#include "InvalidUriHandler.h"

namespace infrastructure {
	namespace video_streaming {
		InvalidUriHandler::InvalidUriHandler()
		{
		}


		void InvalidUriHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
			Poco::Logger& logger = Poco::Logger::get("InvalidUriHandler");
			logger.information("Invalid uri call by " + request.clientAddress().toString());

			response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.send();
		}
	}
}