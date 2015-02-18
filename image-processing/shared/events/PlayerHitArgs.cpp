//============================================================================
// Name        : PlayerHitArgs.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "PlayerHitArgs.h"

namespace shared {
	namespace events {
		PlayerHitArgs::PlayerHitArgs(Player hitPlayer, unsigned short precision)
			: hitPlayer(hitPlayer), precision(precision) {}

		PlayerHitArgs::~PlayerHitArgs() { }

		Player PlayerHitArgs::GetHitPlayer() const {
			return hitPlayer;
		}

		unsigned short PlayerHitArgs::GetPrecision() const {
			return precision;
		}
	}
}