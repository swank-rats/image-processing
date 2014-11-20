//============================================================================
// Name        : VideoStreamingRequestHandlerFactory.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "VideoStreamingRequestHandlerFactory.h"
#include "handler\VideoStreamingRequestHandler.h"
#include "handler\InvalidUriRequestHandler.h"

namespace infrastructure {
	namespace video_streaming {
		VideoStreamingRequestHandlerFactory::VideoStreamingRequestHandlerFactory(WebcamServicePtr webcamService,
			const string& uri) : webcamService(webcamService), uri(uri)	{ }

		VideoStreamingRequestHandlerFactory::~VideoStreamingRequestHandlerFactory() { }

		HTTPRequestHandler* VideoStreamingRequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request) {
			if (request.getURI() == uri) {
				return new VideoStreamingRequestHandler(webcamService);
			}
			else {
				return new InvalidUriRequestHandler;
			}
		}
	}
}