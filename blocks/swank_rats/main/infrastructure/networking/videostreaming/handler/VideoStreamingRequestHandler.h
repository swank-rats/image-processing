//============================================================================
// Name        : VideoStreamingRequestHandler.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\..\..\services\webcam\WebcamService.h"

#include "fenix/poco/Foundation/include/Poco/Logger.h"
#include "fenix/poco/Foundation/include/Poco/SharedPtr.h"
#include "fenix/poco/Foundation/include/Poco/BasicEvent.h"
#include "fenix/poco/Foundation/include/Poco/NotificationQueue.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPServerRequest.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPServerResponse.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPRequestHandler.h"

#include <vector>
#include <string>

using std::vector;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Logger;
using Poco::BasicEvent;
using Poco::SharedPtr;
using Poco::NotificationQueue;
using services::webcam::WebcamService;

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingRequestHandler : public HTTPRequestHandler
		{
		public:
			BasicEvent<int> LostConnection;

			VideoStreamingRequestHandler(SharedPtr<WebcamService> webcamService, NotificationQueue& lostConnectionQueue);
			~VideoStreamingRequestHandler();
			void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
		private:
			SharedPtr<WebcamService> webcamService;
			string boundary;
			NotificationQueue& lostConnectionQueue;

			void FireLostConnection();
		};
	}
}