//============================================================================
// Name        : RequestHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : The common handler for all incoming requests.
//				 Code adapted, original source see http://www.boost.org/doc/libs/1_56_0/doc/html/boost_asio/examples/cpp11_examples.html
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
			RequestHandler(const std::string uri);
			void HandleRequest(const Request& req, Reply& rep);
		private:
			std::string uri;

			static bool UrlDecode(const std::string& in, std::string& out);
		};
	}
}