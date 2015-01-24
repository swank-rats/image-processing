//============================================================================
// Name        : PlayerHitNotification.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "PlayerHitNotification.h"

namespace shared {
	namespace notifications {
		PlayerHitNotification::PlayerHitNotification(Player hitPlayer, unsigned short precision)
			: hitPlayer(hitPlayer), precision(precision) {}

		PlayerHitNotification::~PlayerHitNotification() { }

		Player PlayerHitNotification::GetHitPlayer() {
			return hitPlayer;
		}

		unsigned short PlayerHitNotification::GetPrecision() {
			return precision;
		}
	}
}