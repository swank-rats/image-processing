//============================================================================
// Name        : PlayerHitNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\model\Player.h"

#include <Poco\Notification.h>

using shared::model::Player;
using Poco::Notification;

namespace shared {
	namespace notifications {
		class PlayerHitNotification : public Notification
		{
		public:
			PlayerHitNotification(Player hitPlayer, unsigned char precision = 100);
			~PlayerHitNotification();
			Player GetHitPlayer();
			unsigned char GetPrecision();
		private:
			Player hitPlayer;
			unsigned char precision;
		};
	}
}