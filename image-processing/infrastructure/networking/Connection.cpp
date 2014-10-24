//============================================================================
// Name        : Connection.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Represents a single connection from a client.
//
//============================================================================
#include "Connection.h"


Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager)
	: socket(std::move(socket)), connectionManager(manager)
{
}

void Connection::Start()
{

}

void Connection::Stop()
{
}

void Connection::DoRead()
{
}

void Connection::DoWrite()
{
}