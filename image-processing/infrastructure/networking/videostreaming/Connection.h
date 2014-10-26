//============================================================================
// Name        : Connection.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Represents a single connection from a client.
//
//============================================================================
#pragma once

#include <array>
#include <memory>
#include <boost/asio.hpp>
#include "RequestParser.h"
#include "RequestHandler.h"
#include "Structs.h"

namespace infrastructure {
	namespace video_streaming {
		class ConnectionManager;

		class Connection : public std::enable_shared_from_this < Connection >
		{
		public:
			Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager, RequestHandler& handler);
			/// Start the first asynchronous operation for the connection.
			void Start();
			/// Stop all asynchronous operations associated with the connection.
			void Stop();

		private:
			/// Perform an asynchronous read operation.
			void Receive();
			/// Perform an asynchronous write operation.
			void Response();

			std::array<char, 8192> buffer;
			boost::asio::ip::tcp::socket socket;
			ConnectionManager& connectionManager;
			Request request;
			Reply reply;
			RequestParser requestParser;
			RequestHandler& requestHandler;
		};

		typedef std::shared_ptr<Connection> ConnectionPtr;
	}
}