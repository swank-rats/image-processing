//============================================================================
// Name        : VideoStreamingServer.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Representing a HTTP streaming server that listens at a
//				 specific port and provides a video stream via MJPEG.
//============================================================================
#include "VideoStreamingServer.h"
#include "VideoStreamingRequestHandlerFactory.h"

#include <Poco\Net\HTTPServerParams.h>

using Poco::Net::HTTPServerParams;

namespace infrastructure {
	namespace video_streaming {
		VideoStreamingServer::VideoStreamingServer(unsigned short port, const std::string& uri,
			SharedPtr<WebcamService> webcamService, NotificationQueue& lostConnectionQueue) : socket(port) {
			Logger& logger = Logger::get("VideoStreamingServer");

			HTTPServerParams* pParams = new HTTPServerParams;
			pParams->setMaxQueued(100);
			pParams->setMaxThreads(16);
			pParams->setKeepAlive(true);

			server = new HTTPServer(new VideoStreamingRequestHandlerFactory(webcamService, uri, lostConnectionQueue), threadPool, socket, pParams);
			logger.information("init videostreaming server at port " + std::to_string(port));

			isRunning = false;
		}

		VideoStreamingServer::~VideoStreamingServer() {
			if (server->currentConnections() > 0) {
				StopServer();
			}

			delete server;
			server = nullptr;
		}

		bool VideoStreamingServer::IsRunning() {
			return isRunning;
		}

		void VideoStreamingServer::StartServer() {
			Logger& logger = Logger::get("VideoStreamingServer");

			logger.information("starting video streaming server...");
			server->start();
			logger.information("video streaming server started - waiting for requests");
			isRunning = true;
		}

		void VideoStreamingServer::StopServer() {
			Logger& logger = Logger::get("VideoStreamingServer");

			logger.information("stopping video streaming server");
			server->stopAll(true);
			threadPool.stopAll();
			logger.information("stopped video streaming server");
			isRunning = false;
		}
	}
}