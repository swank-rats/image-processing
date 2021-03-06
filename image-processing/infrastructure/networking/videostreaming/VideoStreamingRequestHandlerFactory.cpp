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
		VideoStreamingRequestHandlerFactory::VideoStreamingRequestHandlerFactory(SharedPtr<WebcamService> webcamService, VideoStreamingServer& server,
			const string& uri) : webcamService(webcamService), uri(uri), server(server) { }

		VideoStreamingRequestHandlerFactory::~VideoStreamingRequestHandlerFactory() {
			//do not delete, since it is a shared pointer
			webcamService = nullptr;
		}

		HTTPRequestHandler* VideoStreamingRequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request) {
			if (request.getURI() == uri) {
				return new VideoStreamingRequestHandler(webcamService, server);
			}
			else {
				return new InvalidUriRequestHandler;
			}
		}
	}
}