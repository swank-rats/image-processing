//============================================================================
// Name        : VideoStreamingHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <Poco\Net\HTTPServerRequest.h>
#include <Poco\Net\HTTPServerResponse.h>
#include <Poco\Net\HTTPRequestHandler.h>
#include "..\..\..\..\services\videostreaming\webcam\WebcamService.h"

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingHandler : public Poco::Net::HTTPRequestHandler
		{
		public:
			VideoStreamingHandler(services::webcam::WebcamServicePtr webcamService);
			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
		private:
			services::webcam::WebcamServicePtr webcamService;
		};

	}
}