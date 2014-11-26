//============================================================================
// Name        : PlayerHitNotification.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "PlayerHitNotification.h"

namespace shared {
	namespace notifications {
		PlayerHitNotification::PlayerHitNotification(Player hittenPlayer) : hittenPlayer(hittenPlayer) { }

		PlayerHitNotification::~PlayerHitNotification() { }

		Player PlayerHitNotification::GetHittenPlayer() {
			return hittenPlayer;
		}
	}
}