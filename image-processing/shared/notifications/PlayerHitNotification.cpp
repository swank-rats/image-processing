//============================================================================
// Name        : PlayerHitNotification.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "PlayerHitNotification.h"

namespace shared {
	namespace notifications {
		PlayerHitNotification::PlayerHitNotification(Player hitPlayer, unsigned char precision)
			: hitPlayer(hitPlayer), precision(precision) {}

		PlayerHitNotification::~PlayerHitNotification() { }

		Player PlayerHitNotification::GetHitPlayer() {
			return hitPlayer;
		}

		unsigned char PlayerHitNotification::GetPrecision() {
			return precision;
		}
	}
}