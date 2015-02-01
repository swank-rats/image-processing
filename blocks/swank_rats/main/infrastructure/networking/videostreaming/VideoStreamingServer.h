//============================================================================
// Name        : VideoStreamingServer.h
// Author      : ITM13
// Version     : 1.0
// Description : Representing a HTTP streaming server that listens at a
//				 specific port and provides a video stream via MJPEG.
//============================================================================
#pragma once
#include "..\..\..\services\webcam\WebcamService.h"

#include "fenix/poco/Net/include/Poco/Net/HTTPServer.h"
#include "fenix/poco/Net/include/Poco/Net/ServerSocket.h"
#include "fenix/poco/Foundation/include/Poco/ThreadPool.h"
#include "fenix/poco/Foundation/include/Poco/Logger.h"
#include "fenix/poco/Foundation/include/Poco/SharedPtr.h"
#include "fenix/poco/Foundation/include/Poco/NotificationQueue.h"

#include <string>

using std::string;
using Poco::Net::HTTPServer;
using Poco::Net::ServerSocket;
using Poco::ThreadPool;
using Poco::Logger;
using Poco::SharedPtr;
using Poco::NotificationQueue;
using services::webcam::WebcamService;

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingServer
		{
		public:
			VideoStreamingServer(unsigned short port, const string& uri, SharedPtr<WebcamService> webcamService, NotificationQueue& lostConnectionQueue);
			~VideoStreamingServer();
			void StartServer();
			void StopServer();
			bool IsRunning();
		private:
			HTTPServer* server;
			ServerSocket socket;
			ThreadPool threadPool;
			bool isRunning;
		};
	}
}