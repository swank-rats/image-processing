//============================================================================
// Name        : VideoStreamingServer.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Representing a HTTP streaming server that listens at a 
//				 specific port and provides a video stream via MJPEG.
//============================================================================
#include <Poco\Net\HTTPServerParams.h>
#include "VideoStreamingServer.h"
#include "VideoStreamingRequestHandlerFactory.h"

namespace infrastructure {
	namespace video_streaming {
		const unsigned int timeoutMilli = 10000;
		Poco::Logger& VideoStreamingServer::logger = Poco::Logger::get("VideoStreamingServer");

		VideoStreamingServer::VideoStreamingServer(unsigned short port, const std::string& uri, 
			services::webcam::WebcamServicePtr webcamService)
			: socket(port)
		{
			Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
			pParams->setMaxQueued(100);
			pParams->setMaxThreads(16);

			server = new Poco::Net::HTTPServer(new VideoStreamingRequestHandlerFactory(webcamService, uri), threadPool, socket, pParams);
			logger.information("init video streaming server " + socket.address().toString() + ":" + std::to_string(port) + uri);
		}

		void VideoStreamingServer::StartServer()
		{
			logger.information("starting video streaming server...");
			server->start();
		}

		void VideoStreamingServer::StopServer() {
			logger.information("stopping video streaming server " + socket.address().toString());
			server->stop();
			threadPool.stopAll();
			logger.information("stopped video streaming server " + socket.address().toString());
		}

	}
}

