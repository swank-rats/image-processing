//============================================================================
// Name        : Connection.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
//#pragma once
//
//class ConnectionManager;
//
///// Represents a single connection from a client.
//class Connection : public std::enable_shared_from_this<Connection>
//{
//public:
//	Connection(boost::asio::ip::tcp::socket socket,	ConnectionManager& manager, RequestHandler& handler);
//
//	/// Start the first asynchronous operation for the connection.
//	void start();
//
//	/// Stop all asynchronous operations associated with the connection.
//	void stop();
//
//private:
//	/// Perform an asynchronous read operation.
//	void do_read();
//
//	/// Perform an asynchronous write operation.
//	void do_write();
//
//	/// Socket for the connection.
//	boost::asio::ip::tcp::socket socket_;
//
//	/// The manager for this connection.
//	connection_manager& connection_manager_;
//
//	/// The handler used to process the incoming request.
//	request_handler& request_handler_;
//
//	/// Buffer for incoming data.
//	std::array<char, 8192> buffer_;
//
//	/// The incoming request.
//	request request_;
//
//	/// The parser for the incoming request.
//	request_parser request_parser_;
//
//	/// The reply to be sent back to the client.
//	reply reply_;
//};
//
//typedef std::shared_ptr<connection> connection_ptr;

