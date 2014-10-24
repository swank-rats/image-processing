//============================================================================
// Name        : VideoStreamingServer.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <boost\asio.hpp>
#include <boost\thread.hpp>
#include <string>

//#include "VideoStreamingRequestHandler.h"
#include "ConnectionManager.h"

class VideoStreamingServer
{
public:
	VideoStreamingServer(const std::string& address, const std::string& port);
	~VideoStreamingServer();
	void StartServer();
	bool StopServer();

private:
	void StartCore();
	void AcceptConnection();

	void WriteHandler(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void AcceptHandler(const boost::system::error_code &ec);

	ConnectionManager connectionManager;
	boost::thread* workerThread;
	boost::asio::io_service ioService;
	boost::asio::ip::tcp::acceptor acceptor;
	boost::asio::ip::tcp::socket socket;
};

