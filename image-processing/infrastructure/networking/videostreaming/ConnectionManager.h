//============================================================================
// Name        : ConnectionManager.h
// Author      : ITM13
// Version     : 1.0
// Description : Manages all connections
//				 Code adapted, original source see http://www.boost.org/doc/libs/1_56_0/doc/html/boost_asio/examples/cpp11_examples.html
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