//============================================================================
// Name        : WebSocketClient.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

namespace infrastructure {
	namespace websocket {
		class WebSocketClient
		{
		public:
			WebSocketClient();
			void OpenConnection();
			void CloseConnection();
		private:
		};
	}
}

