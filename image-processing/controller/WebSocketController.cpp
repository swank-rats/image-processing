//============================================================================
// Name        : WebSocketController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "WebSocketController.h"

namespace controller {
	namespace web_sockets {
		WebSocketController::WebSocketController()
		{
		}

		void WebSocketController::StartWebSocketServer() {
			try {
				// Set logging settings
				webSocketServer.set_access_channels(websocketpp::log::alevel::all);
				webSocketServer.clear_access_channels(websocketpp::log::alevel::frame_payload);
				// Initialize ASIO
				webSocketServer.init_asio();
				// Register our message handler
				webSocketServer.set_message_handler(boost::bind(&WebSocketController::on_message, this, &webSocketServer, _1, _2));
				// Listen on port 9002
				webSocketServer.listen(9002);
				// Start the server accept loop
				webSocketServer.start_accept();
				// Start the ASIO io_service run loop
				webSocketServer.run();
			}
			catch (const std::exception & e) {
				std::cout << e.what() << std::endl;
			}
			catch (websocketpp::lib::error_code e) {
				std::cout << e.message() << std::endl;
			}
			catch (...) {
				std::cout << "other exception" << std::endl;
			}
		}

		void WebSocketController::StopWebSocketServer() {

		}

		void WebSocketController::on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
			std::cout << "on_message called with hdl: " << hdl.lock().get()
				<< " and message: " << msg->get_payload()
				<< std::endl;

			try {
				s->send(hdl, msg->get_payload(), msg->get_opcode());
			}
			catch (const websocketpp::lib::error_code& e) {
				std::cout << "Echo failed because: " << e
					<< "(" << e.message() << ")" << std::endl;
			}
		}
	}
}