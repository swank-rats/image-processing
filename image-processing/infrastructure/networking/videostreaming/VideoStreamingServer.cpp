//============================================================================
// Name        : VideoStreamingServer.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Representing a steraming server instance that listens at a 
//				 specific port on a specific address.
//				 Code adapted, original source see http://www.boost.org/doc/libs/1_56_0/doc/html/boost_asio/examples/cpp11_examples.html
//============================================================================
#include "VideoStreamingServer.h"
#include <boost\log\trivial.hpp>
#include <boost\thread.hpp>

namespace infrastructure {
	namespace video_streaming {
		const unsigned int timeoutMilli = 10000;

		VideoStreamingServer::VideoStreamingServer(const std::string& address, const std::string& port,
			const std::string& uri, services::webcam::WebcamServicePtr webcamService)
			: ioService(), acceptor(ioService), connectionManager(), socket(ioService), requestHandler(uri),
			  streamResponseHandler(webcamService)
		{
			// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
			boost::asio::ip::tcp::resolver resolver(ioService);
			boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({ address, port });
			acceptor.open(endpoint.protocol());
			acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
			acceptor.bind(endpoint);
			acceptor.listen();

			BOOST_LOG_TRIVIAL(info) << "init video streaming server: " + address + ":" + port + "/" + uri;
		}

		void VideoStreamingServer::StartServer()
		{
			BOOST_LOG_TRIVIAL(info) << "starting video streaming server...";
			AcceptConnection();

			workerThread = new boost::thread(boost::bind(&VideoStreamingServer::StartCore, this));
		}

		void VideoStreamingServer::StartCore()
		{
			BOOST_LOG_TRIVIAL(info) << "started streaming worker thread";
			ioService.run();
		}

		void VideoStreamingServer::AcceptConnection() {
			acceptor.async_accept(socket,
				[this](boost::system::error_code ec)
			{
				// Check whether the server was stopped by a signal before this
				// completion handler had a chance to run.
				if (!acceptor.is_open())
				{
					BOOST_LOG_TRIVIAL(error) << "Streaming server acceptor closed!";
					return;
				}

				if (!ec)
				{
					BOOST_LOG_TRIVIAL(info) << "accepted connection " + socket.remote_endpoint().address().to_string();

					connectionManager.Start(std::make_shared<Connection>(std::move(socket), 
						connectionManager, requestHandler, streamResponseHandler));
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
	}
}

