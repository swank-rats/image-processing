//============================================================================
// Name        : ClientConnectionLostNotification.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ClientConnectionLostNotification.h"

namespace shared {
	namespace notifications {
		ClientConnectionLostNotification::ClientConnectionLostNotification(const SocketAddress socketAddr)
			: socketAddr(socketAddr) {}

		ClientConnectionLostNotification::~ClientConnectionLostNotification() { }

		const SocketAddress ClientConnectionLostNotification::GetSocketAddress() const {
			return socketAddr;
		}
	}
}