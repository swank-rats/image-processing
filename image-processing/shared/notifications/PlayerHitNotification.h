//============================================================================
// Name        : PlayerHitNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\model\Player.h"

#include <Poco\Notification.h>

using Poco::Notification;
using shared::model::Player;

namespace shared {
	namespace notifications {
		class PlayerHitNotification : public Notification
		{
		public:
			PlayerHitNotification(Player hittenPlayer);
			~PlayerHitNotification();
			Player GetHittenPlayer();
		private:
			Player hittenPlayer;
		};
	}
}