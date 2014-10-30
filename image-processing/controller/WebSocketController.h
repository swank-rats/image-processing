//============================================================================
// Name        : WebSocketController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include <websocketpp\config\asio_no_tls.hpp>
#include <websocketpp\server.hpp>
#include <boost\bind.hpp>

using websocketpp::connection_hdl;
using websocketpp::lib::ref;

namespace controller {
	namespace web_sockets {
		typedef websocketpp::server<websocketpp::config::asio> server;

		class WebSocketController
		{
		public:
			WebSocketController();
			void StartWebSocketServer();
			void StopWebSocketServer();

		private:
			void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg);

			server webSocketServer;
		};

	}
}