//============================================================================
// Name        : Shot.h
// Author      : ITM13
// Version     : 1.0

// Description :
//============================================================================
#pragma once
#include <opencv2\core\core.hpp>
#include <math.h>

using cv::Point2i;

namespace shared {
	namespace model {
		struct Shot
		{
			Point2i startPoint;
			Point2i endPoint;

			Shot() { }

			Shot(Point2i start, Point2i end) : startPoint(start), endPoint(end) { }

			std::size_t operator () (Shot value) const
				/// Returns the hash for the given value.
			{
				return static_cast<std::size_t>(startPoint.x + startPoint.y + endPoint.x + endPoint.y) * 2654435761U;
			}

			bool operator == (const Shot &s2) const
			{
				return startPoint.x == s2.startPoint.x
					&& startPoint.y == s2.startPoint.y
					&& endPoint.x == s2.endPoint.x
					&& endPoint.y == s2.endPoint.y;
			}

			bool operator != (const Shot &s2) const
			{
				return startPoint.x != s2.startPoint.x
					|| startPoint.y != s2.startPoint.y
					|| endPoint.x != s2.endPoint.x
					|| endPoint.y != s2.endPoint.y;
			}
		};

		struct SimulationShot : public Shot
		{
			enum SimulationHitStatus { UNKNOWN, HIT_PLAYER, HIT_WALL };

			SimulationShot() {}
			SimulationShot(Shot shot) : SimulationShot(shot.startPoint, shot.endPoint) { }
			SimulationShot(Point2i start, Point2i end) : Shot(start, end) {
				a = end.x - start.x;
				b = end.y - start.y;

				currPercentage = percentageStep;

				startExplosionCtr = 0;
				endExplosionCtr = 0;
				status = UNKNOWN;
			}

			Point2i GetNextShotPoint() {
				int nextX = startPoint.x + round(a * currPercentage);
				int nextY = startPoint.y + round(b * currPercentage);
				currPercentage += percentageStep;

				return Point2i(nextX, nextY);
			}

			bool SimulateStartExplosion() {
				++startExplosionCtr;
				return startExplosionCtr <= maxStartExplosions;
			}

			bool SimulateEndExplosion() {
				if (currPercentage < 1.0) {
					return false;
				}

				++endExplosionCtr;
				return endExplosionCtr <= maxEndExplosions;
			}

			SimulationHitStatus GetHitStatus() {
				return status;
			}

			void SetStatusHitPlayer() {
				status = HIT_PLAYER;
			}

			void SetStatusHitWall() {
				status = HIT_WALL;
			}

			bool IsSimulationFinished() {
				return currPercentage >= 1.0 && endExplosionCtr >= maxEndExplosions;
			}
		private:
			double percentageStep = 0.03;
			int maxStartExplosions = 5;
			int maxEndExplosions = 5;

			double currPercentage;
			bool hasStarted;
			int xDirection;
			int yDirection;
			int a;
			int b;
			int startExplosionCtr;
			int endExplosionCtr;
			SimulationHitStatus status;
		};
	}
}
