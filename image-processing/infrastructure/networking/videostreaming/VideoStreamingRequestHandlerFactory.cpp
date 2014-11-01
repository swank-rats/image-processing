#include "VideoStreamingRequestHandlerFactory.h"
#include "handler\StreamingHandler.h"
#include "handler\InvalidUriHandler.h"

namespace infrastructure {
	namespace video_streaming {
		VideoStreamingRequestHandlerFactory::VideoStreamingRequestHandlerFactory(services::webcam::WebcamServicePtr webcamService,
			const std::string& uri) : webcamService(webcamService), uri(uri)
		{
		}

		Poco::Net::HTTPRequestHandler* VideoStreamingRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
			if (request.getURI() == uri){
				return new StreamingHandler(webcamService);
			}
			else {
				return new InvalidUriHandler();
			}
		}
	}
}