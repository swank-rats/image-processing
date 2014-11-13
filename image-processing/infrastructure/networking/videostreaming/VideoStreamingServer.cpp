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

using Poco::Net::HTTPServerParams;

namespace infrastructure {
	namespace video_streaming {

		VideoStreamingServer::VideoStreamingServer(unsigned short port, const std::string& uri, 
			services::webcam::WebcamServicePtr webcamService) : socket(port)
		{
			Logger& logger = Logger::get("VideoStreamingServer");

			HTTPServerParams* pParams = new HTTPServerParams;
			pParams->setMaxQueued(100);
			pParams->setMaxThreads(16);
			pParams->setKeepAlive(true);

			server = new HTTPServer(new VideoStreamingRequestHandlerFactory(webcamService, uri), threadPool, socket, pParams);
			logger.information("init videostreaming server at port " + socket.address().port());
		}

		VideoStreamingServer::~VideoStreamingServer() {
			if (server->currentConnections() > 0) {
				//TODO check if this will cause an exception
				server->stopAll();
			}

			delete server;
			server = nullptr;
		}

		void VideoStreamingServer::StartServer()
		{
			Logger& logger = Logger::get("VideoStreamingServer");

			logger.information("starting video streaming server...");
			server->start();
		}

		void VideoStreamingServer::StopServer() {
			Logger& logger = Logger::get("VideoStreamingServer");

			logger.information("stopping video streaming server");
			server->stopAll();
			threadPool.stopAll();
			logger.information("stopped video streaming server");
		}

	}
}

