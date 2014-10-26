//============================================================================
// Name        : VideoStreamingServer.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <string>

#include "ConnectionManager.h"
#include "RequestHandler.h"
#include "..\..\..\services\videostreaming\webcam\WebcamService.h"

namespace infrastructure {
	namespace video_streaming {
		class VideoStreamingServer
		{
		public:
			VideoStreamingServer(const std::string& address, const std::string& port, 
								 const std::string& uri, services::webcam::WebcamServicePtr webcamService);
			void StartServer();
			bool StopServer();

		private:
			void StartCore();
			void AcceptConnection();

			void WriteHandler(const boost::system::error_code &ec, std::size_t bytes_transferred);
			void AcceptHandler(const boost::system::error_code &ec);

			ConnectionManager connectionManager;
			RequestHandler requestHandler;
			boost::thread* workerThread;
			boost::asio::io_service ioService;
			boost::asio::ip::tcp::acceptor acceptor;
			boost::asio::ip::tcp::socket socket;
		};
	}
}