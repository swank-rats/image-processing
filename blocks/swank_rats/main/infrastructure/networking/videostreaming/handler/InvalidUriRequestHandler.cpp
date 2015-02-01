//============================================================================
// Name        : InvalidUriRequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "InvalidUriRequestHandler.h"

using Poco::Net::HTTPResponse;

namespace infrastructure {
	namespace video_streaming {
		InvalidUriRequestHandler::InvalidUriRequestHandler() { }

		InvalidUriRequestHandler::~InvalidUriRequestHandler() { }

		void InvalidUriRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
			Logger& logger = Logger::get("InvalidUriRequestHandler");

			logger.information("Invalid uri " + request.getURI() + " call by " + request.clientAddress().toString());

			response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
			response.send();
		}
	}
}