//============================================================================
// Name        : VideoStreamingServer.h
// Author      : ITM13
// Version     : 1.0
// Description : Representing a HTTP streaming server that listens at a 
//				 specific port and provides a video stream via MJPEG.
//============================================================================
#pragma once
#include <string>

#include <Poco\Net\HTTPServer.h>
#include <Poco\Net\ServerSocket.h>
#include <Poco\ThreadPool.h>
#include <Poco\Logger.h>

#include "..\..\..\services\videostreaming\webcam\WebcamService.h"

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingServer
		{
		public:
			VideoStreamingServer(unsigned short port, const std::string& uri, services::webcam::WebcamServicePtr webcamService);
			void StartServer();
			void StopServer();

		private:
			Poco::Net::HTTPServer* server;
			Poco::Net::ServerSocket socket;
			Poco::ThreadPool threadPool;

			static Poco::Logger& logger;
		};
	}
}