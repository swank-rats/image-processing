//============================================================================
// Name        : WebSocketClientConnectionHandler.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "swank_rats/main/shared/model/message/Message.h"

#include "fenix/poco/Foundation/include/Poco/URI.h"
#include "fenix/poco/Foundation/include/Poco/Logger.h"
#include "fenix/poco/Foundation/include/Poco/Timespan.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPServer.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPClientSession.h"
#include "fenix/poco/Net/include/Poco/Net/HTTPSClientSession.h"
#include "fenix/poco/Net/include/Poco/Net/WebSocket.h"
#include "fenix/poco/Foundation/include/Poco/Activity.h"
#include "fenix/poco/Foundation/include/Poco/Mutex.h"
#include "fenix/poco/Foundation/include/Poco/BasicEvent.h"
#include "fenix/poco/Foundation/include/Poco/NotificationQueue.h"

using Poco::Logger;
using Poco::URI;
using Poco::BasicEvent;
using Poco::Timespan;
using Poco::Activity;
using Poco::Mutex;
using Poco::NotificationQueue;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::WebSocket;
using shared::model::message::Message;

namespace infrastructure {
	namespace websocket {
		class WebSocketClientConnectionHandler
		{
		public:
			BasicEvent<int> LostConnection;

			WebSocketClientConnectionHandler(URI uri, NotificationQueue &receivedQueue, NotificationQueue &sendingQueue);
			~WebSocketClientConnectionHandler();
			URI GetURI();
			bool OpenConnection();
			void CloseConnection();
			bool Reconnect();
			bool IsConnected();
			bool IsReconnecting();
			NotificationQueue* GetReceivedMessagesQueues();
		private:
			URI uri;
			Activity<WebSocketClientConnectionHandler> receiveActity;
			Activity<WebSocketClientConnectionHandler> sendActity;
			HTTPSClientSession* session;
			WebSocket* webSocket;
			Timespan sendTimeout;
			Timespan receiveTimeout;
			Timespan pollTimeout;
			NotificationQueue &sendingQueue;
			NotificationQueue &receivedQueue;
			bool isConnected;
			bool isReconnecting;
			Poco::Mutex mutex;

			void Listen();
			void Send();
			bool EstablishConnection();
			void FireLostConnection();
		};
	}
}