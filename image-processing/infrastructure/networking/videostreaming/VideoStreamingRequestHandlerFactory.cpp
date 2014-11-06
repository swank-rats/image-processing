#include "VideoStreamingRequestHandlerFactory.h"
#include "handler\VideoStreamingRequestHandler.h"
#include "handler\InvalidUriRequestHandler.h"

namespace infrastructure {
	namespace video_streaming {
		VideoStreamingRequestHandlerFactory::VideoStreamingRequestHandlerFactory(services::webcam::WebcamServicePtr webcamService,
			const std::string& uri) : webcamService(webcamService), uri(uri)
		{
		}

		Poco::Net::HTTPRequestHandler* VideoStreamingRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
			if (request.getURI() == uri) {
				return new VideoStreamingRequestHandler(webcamService);
			}
			else {
				return new InvalidUriRequestHandler;
			}
		}
	}
}