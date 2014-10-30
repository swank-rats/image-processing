//============================================================================
// Name        : VideoStreamingServer.h
// Author      : ITM13
// Version     : 1.0
// Description : Representing a steraming server instance that listens at a 
//				 specific port on a specific address.
//				 Code adapted, original source see http://www.boost.org/doc/libs/1_56_0/doc/html/boost_asio/examples/cpp11_examples.html
//============================================================================
#pragma once
#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <string>

#include "ConnectionManager.h"
#include "RequestHandler.h"
#include "StreamResponseHandler.h"
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
			StreamResponseHandler streamResponseHandler;
			boost::thread* workerThread;
			boost::asio::io_service ioService;
			boost::asio::ip::tcp::acceptor acceptor;
			boost::asio::ip::tcp::socket socket;
		};
	}
}