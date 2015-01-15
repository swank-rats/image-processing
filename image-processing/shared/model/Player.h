//============================================================================
// Name        : Player.h
// Author      : ITM13
// Version     : 1.0

// Description :
//============================================================================
#pragma once

#include <iostream>
#include <string>

using namespace std;


namespace shared {
	namespace model {
		struct Player
		{
			unsigned char playerId;
			string form;

			Player() { }

			Player(unsigned char playerId, string form)
				:playerId(playerId), form(form){}
		};
	}
}