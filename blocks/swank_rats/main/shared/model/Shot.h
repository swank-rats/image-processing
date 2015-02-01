//============================================================================
// Name        : Shot.h
// Author      : ITM13
// Version     : 1.0

// Description :
//============================================================================
#pragma once
#include "Player.h"

#include "diego/opencv/opencv2/core/core.hpp"
#include <math.h>

using cv::Point2i;

namespace shared {
	namespace model {
		struct Shot
		{
			Player shootingPlayer;
			Player hitPlayer;
			Point2i startPoint;
			Point2i endPoint;

			Shot() { }

			Shot(Player shootingPlayer, Player hitPlayer, Point2i start, Point2i end)
				: shootingPlayer(shootingPlayer), hitPlayer(hitPlayer), startPoint(start), endPoint(end) { }

			std::size_t operator () (Shot value) const
			{
				// Returns the hash for the given value.
				return static_cast<std::size_t>(startPoint.x + startPoint.y + endPoint.x + endPoint.y) * 2654435761U;
			}

			bool operator == (const Shot &s2) const
			{
				return shootingPlayer.playerId == s2.shootingPlayer.playerId
					&& startPoint.x == s2.startPoint.x
					&& startPoint.y == s2.startPoint.y
					&& endPoint.x == s2.endPoint.x
					&& endPoint.y == s2.endPoint.y;
			}

			bool operator != (const Shot &s2) const
			{
				return shootingPlayer.playerId != s2.shootingPlayer.playerId
					|| startPoint.x != s2.startPoint.x
					|| startPoint.y != s2.startPoint.y
					|| endPoint.x != s2.endPoint.x
					|| endPoint.y != s2.endPoint.y;
			}
		};

		struct SimulationShot : public Shot
		{
			enum SimulationHitStatus { UNKNOWN, HIT_PLAYER, HIT_WALL };

			SimulationShot() {}
			SimulationShot(Shot shot) : SimulationShot(shot.shootingPlayer,shot.hitPlayer, shot.startPoint, shot.endPoint) { }
			SimulationShot(Player shootingPlayer,Player hitPlayer, Point2i start, Point2i end) : Shot(shootingPlayer,hitPlayer, start, end) {
				a = end.x - start.x;
				b = end.y - start.y;

				currPercentage = percentageStep;

				currX = start.x;
				currY = start.y;

				startExplosionCtr = 0;
				endExplosionCtr = 0;
				status = UNKNOWN;
			}

			Point2i GetNextShotPoint() {
				currX = startPoint.x + round(a * currPercentage);
				currY = startPoint.y + round(b * currPercentage);
				currPercentage += percentageStep;

				return Point2i(currX, currY);
			}

			Point2i GetCurrentShotPoint() {
				return Point2i(currX, currY);
			}

			bool SimulateStartExplosion() {
				++startExplosionCtr;
				return startExplosionCtr <= maxStartExplosions;
			}

			void SetCurrentPointAsEndoint() {
				endPoint = Point2i(currX, currY);
			}

			bool SimulateEndExplosion() {
				if (status == HIT_PLAYER) {
					currPercentage = 1.0;
					return true;
				}

				if (currPercentage < 1.0) {
					return false;
				}

				++endExplosionCtr;
				return endExplosionCtr <= maxEndExplosions;
			}

			bool IsSimulationFinished() {
				return currPercentage >= 1.0 && endExplosionCtr >= maxEndExplosions;
			}

			SimulationHitStatus status;
		private:
			double percentageStep = 0.03;
			int maxStartExplosions = 5;
			int maxEndExplosions = 5;

			double currPercentage;
			bool hasStarted;
			int xDirection;
			int yDirection;
			int currX;
			int currY;
			int a;
			int b;
			int startExplosionCtr;
			int endExplosionCtr;
		};
	}
}
