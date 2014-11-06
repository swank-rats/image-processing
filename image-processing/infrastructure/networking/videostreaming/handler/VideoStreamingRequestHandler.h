//============================================================================
// Name        : VideoStreamingRequestHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\Logger.h>
#include <Poco\Net\HTTPServerRequest.h>
#include <Poco\Net\HTTPServerResponse.h>
#include <Poco\Net\HTTPRequestHandler.h>
#include "..\..\..\..\services\videostreaming\webcam\WebcamService.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Logger;
using services::webcam::WebcamServicePtr;

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingRequestHandler : public HTTPRequestHandler
		{
		public:
			VideoStreamingRequestHandler(WebcamServicePtr webcamService);
			void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
		private:
			WebcamServicePtr webcamService;
		};

	}
}