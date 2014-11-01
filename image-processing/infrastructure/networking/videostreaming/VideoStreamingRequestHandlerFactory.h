//============================================================================
// Name        : VideoStreamingRequestHandlerFactory.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <string>
#include <Poco\Net\HTTPRequestHandlerFactory.h>
#include <Poco\Net\HTTPServerRequest.h>

#include "..\..\..\services\videostreaming\webcam\WebcamService.h"

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
		{
		public:
			VideoStreamingRequestHandlerFactory(services::webcam::WebcamServicePtr webcamService, const std::string& uri);
			Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);
		private:
			services::webcam::WebcamServicePtr webcamService;
			std::string uri;
		};
	}
}