//============================================================================
// Name        : VideoStreamingServer.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================

#pragma once
#include <set>
#include "Connection.h"

namespace infrastructure {
	namespace video_streaming {

		class ConnectionManager
		{
		public:
			ConnectionManager();

			// Add the specified connection to the manager and start it.
			void Start(ConnectionPtr c);

			// Stop the specified connection.
			void Stop(ConnectionPtr c);

			// Stop all connections.
			void StopAll();

		private:
			// The managed connections.
			std::set<ConnectionPtr> connections;
		};
	}
}