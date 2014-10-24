//============================================================================
// Name        : VideoStreamingServer.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Representing 
//============================================================================
#include "VideoStreamingServer.h"
#include <boost\log\trivial.hpp>
#include <boost\thread.hpp>

VideoStreamingServer::VideoStreamingServer(const std::string& address, const std::string& port)
	: ioService(), acceptor(ioService), connectionManager(), socket(ioService)
{
	// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
	boost::asio::ip::tcp::resolver resolver(ioService);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({ address, port });
	acceptor.open(endpoint.protocol());
	acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor.bind(endpoint);
	acceptor.listen();

	BOOST_LOG_TRIVIAL(info) << "using ip " << endpoint.address() << ":" << endpoint.port();

	AcceptConnection();
}


VideoStreamingServer::~VideoStreamingServer()
{

}

void VideoStreamingServer::StartServer()
{
	BOOST_LOG_TRIVIAL(info) << "starting video streaming server...";
	workerThread = new boost::thread(boost::bind(&VideoStreamingServer::StartCore, this));
}

void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
}

void VideoStreamingServer::StartCore()
{
	ioService.run();

	//BOOST_LOG_TRIVIAL(info) << "worker thread init";

	//boost::asio::io_service ioService;

	///// Acceptor used to listen for incoming connections.
	//tcp::acceptor acceptor(ioService, tcp::endpoint(tcp::v4(), port));
	//acceptor.set_option(tcp::acceptor::reuse_address(true));

	//while (1) {
	//	try {
	//		/// The next socket to be accepted.
	//		tcp::socket socket(ioService);
	//		acceptor.accept(socket);

	//		BOOST_LOG_TRIVIAL(info) << "accepted connection";

	//		boost::system::error_code ignored_error;
	//		boost::asio::async_write(socket, boost::asio::buffer(data), write_handler);
	//	}
	//	catch (std::exception) {
	//		BOOST_LOG_TRIVIAL(error) << "connection error";

	//	}
	//}

	//acceptor.async_accept(socket, [this](boost::system::error_code ec)
	//{
	//	// Check whether the server was stopped by a signal before this
	//	// completion handler had a chance to run.
	//	if (!acceptor.is_open())
	//	{
	//		return;
	//	}

	//	if (!ec)
	//	{
	//		BOOST_LOG_TRIVIAL(info) << "accepted connection";

	//		boost::asio::async_write(socket, boost::asio::buffer(data), write_handler);
	//		//connectionManager.start(std::make_shared<Connection>(std::move(socket), connectionManager, requestHandler));
	//	}
	//});
}


void VideoStreamingServer::AcceptConnection() {
	acceptor.async_accept(socket,
		[this](boost::system::error_code ec)
	{
		// Check whether the server was stopped by a signal before this
		// completion handler had a chance to run.
		if (!acceptor.is_open())
		{
			return;
		}

		if (!ec)
		{
			connectionManager.Start(std::make_shared<Connection>(std::move(socket), connectionManager));
		}

		AcceptConnection();
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
