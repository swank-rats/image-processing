//============================================================================
// Name        : Connection.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Represents a single connection from a client.
//
//============================================================================
#include <iostream>
#include "Connection.h"
#include "ConnectionManager.h"

namespace infrastructure {
	namespace video_streaming {
		Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager, RequestHandler& handler)
			: socket(std::move(socket)), connectionManager(manager), requestHandler(handler)
		{
		}

		void Connection::Start()
		{
			Receive();
		}

		void Connection::Stop()
		{
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
					}
					else if (result == RequestParser::bad)
					{
						reply = Reply::GetStockReply(Reply::bad_request);
					}

					Response();
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
				if (!ec)
				{
					// Initiate graceful connection closure.
					//boost::system::error_code ignored_ec;
					//socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

					// DO NOTHING - KEEP ALIVE since it is a video stream
				}

				if (ec != boost::asio::error::operation_aborted)
				{
					connectionManager.Stop(shared_from_this());
				}
			});
		}
	}
}