//============================================================================
// Name        : PlayerHitNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include <Poco\Notification.h>
#include <Poco\AutoPtr.h>
#include <Poco\Net\SocketAddress.h>

using Poco::AutoPtr;
using Poco::Notification;
using Poco::Net::SocketAddress;

namespace shared {
	namespace notifications {
		class ClientConnectionLostNotification : public Notification
		{
		public:
			typedef AutoPtr<ClientConnectionLostNotification> Ptr;

			ClientConnectionLostNotification(const SocketAddress socketAddr);
			~ClientConnectionLostNotification();
			const SocketAddress GetSocketAddress() const;
		private:
			const SocketAddress socketAddr;
		};
	}
}