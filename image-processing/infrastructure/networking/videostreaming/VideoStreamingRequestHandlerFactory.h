//============================================================================
// Name        : VideoStreamingRequestHandlerFactory.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\..\services\webcam\WebcamService.h"

#include <Poco\Net\HTTPRequestHandlerFactory.h>
#include <Poco\Net\HTTPServerRequest.h>

#include <string>

using std::string;
using Poco::Net::HTTPRequestHandlerFactory;
using services::webcam::WebcamServicePtr;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPRequestHandler;

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingRequestHandlerFactory : public HTTPRequestHandlerFactory
		{
		public:
			VideoStreamingRequestHandlerFactory(WebcamServicePtr webcamService, const string& uri);
			~VideoStreamingRequestHandlerFactory();
			HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
		private:
			WebcamServicePtr webcamService;
			string uri;
		};
	}
}