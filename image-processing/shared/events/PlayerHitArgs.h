//============================================================================
// Name        : PlayerHitArgs.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\model\Player.h"

#include <Poco\Notification.h>
#include <Poco\AutoPtr.h>

using Poco::AutoPtr;
using shared::model::Player;
using Poco::Notification;

namespace shared {
	namespace events {
		class PlayerHitArgs
		{
		public:
			PlayerHitArgs(Player hitPlayer, unsigned short precision);
			~PlayerHitArgs();
			Player GetHitPlayer() const;
			unsigned short GetPrecision() const;
		private:
			Player hitPlayer;
			unsigned short precision;
		};
	}
}