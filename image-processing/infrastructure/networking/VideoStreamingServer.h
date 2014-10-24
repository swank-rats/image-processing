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
//#include "ConnectionManager.h"

class VideoStreamingServer
{
public:
	VideoStreamingServer(std::string addr, std::string port);
	~VideoStreamingServer();
	void StartVideoStreamingServer();
	bool StopVideoStreamingServer();

private:
	void HandleConnections();
	void WriteHandler(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void AcceptHandler(const boost::system::error_code &ec);

	boost::asio::io_service ioService;
	/// Acceptor used to listen for incoming connections.
	boost::asio::ip::tcp::acceptor acceptor;
	/// The next socket to be accepted.
	boost::asio::ip::tcp::socket socket;
	boost::thread* workerThread;
};

