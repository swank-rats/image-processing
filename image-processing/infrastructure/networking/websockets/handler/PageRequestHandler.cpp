//============================================================================
// Name        : PageRequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <Poco\Logger.h>

#include "PageRequestHandler.h"

namespace infrastructure {
	namespace websocket {
		PageRequestHandler::PageRequestHandler()
		{
		}


		void PageRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
			Poco::Logger& logger = Poco::Logger::get("PageRequestHandler");
			logger.information("Page requested by " + request.clientAddress().toString());

			response.setChunkedTransferEncoding(true);
			response.setContentType("text/html");
			std::ostream& ostr = response.send();
			ostr << "<html>";
			ostr << "<head>";
			ostr << "<title>WebSocketServer</title>";
			ostr << "<script type=\"text/javascript\">";
			ostr << "function WebSocketTest()";
			ostr << "{";
			ostr << "  if (\"WebSocket\" in window)";
			ostr << "  {";
			ostr << "    var ws = new WebSocket(\"ws://" << request.serverAddress().toString() << "/ws\");";
			ostr << "    ws.onopen = function()";
			ostr << "      {";
			ostr << "        ws.send(\"Hello, world!\");";
			ostr << "      };";
			ostr << "    ws.onmessage = function(evt)";
			ostr << "      { ";
			ostr << "        var msg = evt.data;";
			ostr << "        alert(\"Message received: \" + msg);";
			ostr << "        ws.close();";
			ostr << "      };";
			ostr << "    ws.onclose = function()";
			ostr << "      { ";
			ostr << "        alert(\"WebSocket closed.\");";
			ostr << "      };";
			ostr << "  }";
			ostr << "  else";
			ostr << "  {";
			ostr << "     alert(\"This browser does not support WebSockets.\");";
			ostr << "  }";
			ostr << "}";
			ostr << "</script>";
			ostr << "</head>";
			ostr << "<body>";
			ostr << "  <h1>WebSocket Server</h1>";
			ostr << "  <p><a href=\"javascript:WebSocketTest()\">Run WebSocket Script</a></p>";
			ostr << "</body>";
			ostr << "</html>";
		}
	}
}