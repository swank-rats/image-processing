//============================================================================
// Name        : VideoStreamingRequestHandlerFactory.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\..\services\webcam\WebcamService.h"

#include "fenix/poco/Net/include/Poco/Net/HTTPRequestHandlerFactory.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPServerRequest.h"
#include "fenix/poco/Foundation/include/Poco/SharedPtr.h"
#include "fenix/poco/Foundation/include/Poco/NotificationQueue.h"

#include <string>

using std::string;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::SharedPtr;
using Poco::NotificationQueue;
using services::webcam::WebcamService;

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingRequestHandlerFactory : public HTTPRequestHandlerFactory
		{
		public:
			VideoStreamingRequestHandlerFactory(SharedPtr<WebcamService> webcamService, const string& uri, NotificationQueue& lostConnectionQueue);
			~VideoStreamingRequestHandlerFactory();
			HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
		private:
			SharedPtr<WebcamService> webcamService;
			NotificationQueue& lostConnectionQueue;
			string uri;
		};
	}
}