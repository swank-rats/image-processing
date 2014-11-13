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

using std::string;
using Poco::Net::HTTPServer;
using Poco::Net::ServerSocket;
using Poco::ThreadPool;
using Poco::Logger;
using services::webcam::WebcamServicePtr;

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingServer
		{
		public:
			VideoStreamingServer(unsigned short port, const string& uri, WebcamServicePtr webcamService);
			~VideoStreamingServer();
			void StartServer();
			void StopServer();
		private:
			HTTPServer* server;
			ServerSocket socket;
			ThreadPool threadPool;
		};
	}
}