//============================================================================
// Name        : InvalidUriRequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================

#include "InvalidUriRequestHandler.h"

namespace infrastructure {
	namespace video_streaming {
		Poco::Logger& InvalidUriRequestHandler::logger = Poco::Logger::get("InvalidUriRequestHandler");

		InvalidUriRequestHandler::InvalidUriRequestHandler()
		{
		}


		void InvalidUriRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
			logger.information("Invalid uri " + request.getURI() + " call by " + request.clientAddress().toString());
			
			response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.send();
		}
	}
}