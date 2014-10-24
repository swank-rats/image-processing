//============================================================================
// Name        : VideoStreamingServer.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "VideoStreamingServer.h"
#include <boost\log\trivial.hpp>
#include <boost\thread.hpp>


std::string data = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";

VideoStreamingServer::VideoStreamingServer(std::string addr, std::string port)
: ioService(), acceptor(ioService), socket(ioService)
{
	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	boost::asio::ip::tcp::resolver resolver(ioService);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({ addr, port });
	acceptor.open(endpoint.protocol());
	acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor.bind(endpoint);
	acceptor.listen();

	//DoAcceptingConnections();
	//AwaitStopRequest();
}


VideoStreamingServer::~VideoStreamingServer()
{

}

void VideoStreamingServer::StartVideoStreamingServer()
{
	BOOST_LOG_TRIVIAL(info) << "starting video streaming server...";
	workerThread = new boost::thread(boost::bind(&VideoStreamingServer::HandleConnections, this));
}

void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
}

void VideoStreamingServer::HandleConnections()
{
	while (1) {
		try {
			
		}
		catch (std::exception) {

		}
	}

	acceptor.async_accept(socket, [this](boost::system::error_code ec)
	{
		// Check whether the server was stopped by a signal before this
		// completion handler had a chance to run.
		if (!acceptor.is_open())
		{
			return;
		}

		if (!ec)
		{
			BOOST_LOG_TRIVIAL(info) << "accepted connection";

			boost::asio::async_write(socket, boost::asio::buffer(data), write_handler);
			//connectionManager.start(std::make_shared<Connection>(std::move(socket), connectionManager, requestHandler));
		}
	});
}



//void VideoStreamingServer::AwaitStopRequest()
//{
//	signals.async_wait(
//		[this](boost::system::error_code /*ec*/, int /*signo*/)
//	{
//		// The server is stopped by cancelling all outstanding asynchronous
//		// operations. Once all operations have finished the io_service::run()
//		// call will exit.
//		//acceptor.close();
//		//connectionManager.StopAll();
//	});
//}
