//============================================================================
// Name        : Connection.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Represents a single connection from a client.
//
//============================================================================
#include <iostream>
#include <boost/log/trivial.hpp>

#include "Connection.h"
#include "ConnectionManager.h"

namespace infrastructure {
	namespace video_streaming {
		Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager,
			RequestHandler& requestHandler, StreamResponseHandler& streamHandler)
			: socket(std::move(socket)), connectionManager(manager), requestHandler(requestHandler),
			  streamReponseHandler(streamHandler)
		{
		}

		void Connection::Start()
		{
			Receive();
		}

		void Connection::Stop()
		{
			BOOST_LOG_TRIVIAL(info) << "closing connection " + socket.remote_endpoint().address().to_string();

			if (streamingThread.joinable()) {
				while (!streamingThread.timed_join(boost::posix_time::seconds(2)))
				{
					// Interupt the thread
					BOOST_LOG_TRIVIAL(info) << "connection thread interrupt request sent";
					streamingThread.interrupt();
				}

				BOOST_LOG_TRIVIAL(info) << "connection thread stopped successfully";

			}
			else {
				BOOST_LOG_TRIVIAL(error) << "connection thread is already disposed!";
			}

			ShutdownConnection();
			socket.close();
		}

		void Connection::Receive()
		{
			auto self(shared_from_this());
			socket.async_read_some(boost::asio::buffer(buffer), [this, self](boost::system::error_code ec, std::size_t bytesTransferred)
			{
				if (!ec)
				{
					RequestParser::ResultType result;
					std::tie(result, std::ignore) = requestParser.Parse(request, buffer.data(), buffer.data() + bytesTransferred);

					if (result == RequestParser::good)
					{
						requestHandler.HandleRequest(request, reply);
						Response();
						streamingThread = boost::thread(boost::bind(&Connection::StartSendingVideoStream, this));
					}
					else if (result == RequestParser::bad)
					{
						reply = Reply::GetStockReply(Reply::bad_request);
						Response();
					}
					else {
						//we are in an intermediate state and more
						//data were needed
						Receive();
					}
				}
				else if (ec != boost::asio::error::operation_aborted)
				{
					connectionManager.Stop(shared_from_this());
				}
			});
		}

		void Connection::Response()
		{
			auto self(shared_from_this());

			boost::asio::async_write(socket, reply.ToBuffers(), [this, self](boost::system::error_code ec, std::size_t)
			{
				if (ec) {
					ShutdownConnection();
				}
			});
		}

		void Connection::ShutdownConnection() {
			BOOST_LOG_TRIVIAL(info) << "shut down socket of connection " << socket.remote_endpoint().address().to_string();

			// Initiate graceful connection closure.
			boost::system::error_code ignored_ec;
			socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
			connectionManager.Stop(shared_from_this());
		}

		void Connection::StartSendingVideoStream() {
			try {
				while (1) {
					if (streamReponseHandler.CreateResponse(reply)) {
						Response();
					}

					//here is the place where the thread can be interrupted with join
					boost::this_thread::interruption_point();
				}
			}
			catch (boost::thread_interrupted) {
				BOOST_LOG_TRIVIAL(warning) << "recording thread stopped by interrupt";
			}
		}
	}
}