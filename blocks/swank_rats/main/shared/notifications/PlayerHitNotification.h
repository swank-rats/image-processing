//============================================================================
// Name        : PlayerHitNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "swank_rats/main/shared/model/Player.h"

#include "fenix/poco/Foundation/include/Poco/Notification.h"
#include "fenix/poco/Foundation/include/Poco/AutoPtr.h"

using Poco::AutoPtr;
using shared::model::Player;
using Poco::Notification;

namespace shared {
	namespace notifications {
		class PlayerHitNotification : public Notification
		{
		public:
			typedef AutoPtr<PlayerHitNotification> Ptr;

			PlayerHitNotification(Player hitPlayer, unsigned short precision);
			~PlayerHitNotification();
			Player GetHitPlayer();
			unsigned short GetPrecision();
		private:
			Player hitPlayer;
			unsigned short precision;
		};
	}
}