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
#include "ConnectionManager.h"

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	Connection(boost::asio::ip::tcp::socket socket,	ConnectionManager& manager);

	/// Start the first asynchronous operation for the connection.
	void Start();

	/// Stop all asynchronous operations associated with the connection.
	void Stop();

private:
	/// Perform an asynchronous read operation.
	void DoRead();
	/// Perform an asynchronous write operation.
	void DoWrite();
	/// Socket for the connection.
	boost::asio::ip::tcp::socket socket;

	/// The manager for this connection.
	ConnectionManager& connectionManager;

	/// Buffer for incoming data.
	std::array<char, 8192> buffer;

	///// The incoming request.
	//request request_;

	///// The parser for the incoming request.
	//request_parser request_parser_;

	///// The reply to be sent back to the client.
	//reply reply_;
};

typedef std::shared_ptr<Connection> ConnectionPtr;
