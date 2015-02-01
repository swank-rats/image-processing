//============================================================================
// Name        : WebSocketClient.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\..\..\shared\model\message\Message.h"
#include "WebSocketClientConnectionHandler.h"

#include "fenix/poco/Foundation/Poco/include/URI.h"
#include "fenix/poco/Foundation/Poco/include/Logger.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPServer.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPSClientSession.h"
#include "fenix/poco/Net/include/Poco/Net/WebSocket.h"
#include "fenix/poco/Foundation/Poco/include/NotificationQueue.h"
#include "fenix/poco/Foundation/Poco/include/Activity.h"

using Poco::Logger;
using Poco::URI;
using Poco::NotificationQueue;
using Poco::Activity;
using Poco::Net::HTTPSClientSession;
using Poco::Net::WebSocket;
using shared::model::message::Message;

namespace infrastructure {
	namespace websocket {
		class WebSocketClient
		{
		public:
			WebSocketClient(URI uri, NotificationQueue &receivedQueue);
			~WebSocketClient();
			void OpenConnection();
			void CloseConnection();
			bool IsConnected();
			void Send(Message* message);
		private:
			WebSocketClientConnectionHandler* connHandler;
			NotificationQueue sendingQueue;
			void OnLostconnection(const void* pSender, int& arg);
			void SendInitMessage();
		};
	}
}
