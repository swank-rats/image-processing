//============================================================================
// Name        : RequestHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : The common handler for all incoming requests.
//============================================================================
#pragma once
#include <string>

#include "..\..\..\services\videostreaming\webcam\WebcamService.h"

namespace infrastructure {
	namespace video_streaming {
		struct Reply;
		struct Request;

		class RequestHandler
		{
		public:
			RequestHandler(const std::string uri, services::webcam::WebcamServicePtr webcamService);
			void HandleRequest(const Request& req, Reply& rep);
		private:
			services::webcam::WebcamServicePtr webcamService;
			std::string uri;

			static bool UrlDecode(const std::string& in, std::string& out);
		};
	}
}