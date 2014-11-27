//============================================================================
// Name        : Shot.h
// Author      : ITM13
// Version     : 1.0

// Description :
//============================================================================
#pragma once
#include <opencv2\core\core.hpp>

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
			SimulationShot() {}
			SimulationShot(Shot shot) : SimulationShot(shot.startPoint, shot.endPoint) { }
			SimulationShot(Point2i start, Point2i end) : Shot(start, end) {
				hasStarted = false;

				currX = start.x;
				currY = start.y;

				xDirection = LeftToRight;
				yDirection = TopToBottom;

				CalculateSimulation();
			}

			Point2i GetNextShotPoint() {
				switch (xDirection) {
				case LeftToRight:
					currX = currX + xSummand <= endPoint.x ? currX + xSummand : endPoint.x;
					break;
				case RightToLeft:
					currX = currX - xSummand >= endPoint.x ? currX - xSummand : endPoint.x;
					break;
				}

				switch (yDirection) {
				case TopToBottom:
					currY = currY + ySummand <= endPoint.y ? currY + ySummand : endPoint.y;
					break;
				case BottomToTop:
					currY = currY - ySummand >= endPoint.y ? currY - ySummand : endPoint.y;
					break;
				}

				return Point2i(currX, currY);
			}

			void StartSimulation() {
				hasStarted = true;
			}

			bool HasSimulationStarted() {
				return hasStarted;
			}

			bool IsEndPointReached() {
				return currX - endPoint.x == 0 && currY - endPoint.y == 0;
			}
		private:
			enum XDirections { LeftToRight, RightToLeft };
			enum YDirections { TopToBottom, BottomToTop };

			bool hasStarted;
			int xSummand;
			int ySummand;
			int currX;
			int currY;
			XDirections xDirection;
			YDirections yDirection;

			void CalculateSimulation() {
				xSummand = startPoint.x >= endPoint.x ? startPoint.x / endPoint.x : endPoint.x / startPoint.x;
				if (startPoint.x > endPoint.x) {
					//we need to simulate shot from right to left
					xDirection = RightToLeft;
				}

				ySummand = startPoint.y >= endPoint.y ? startPoint.y / endPoint.y : endPoint.y / startPoint.y;
				if (startPoint.y > endPoint.y) {
					//we need to simulate shot from bottom to top
					yDirection = BottomToTop;
				}
			}
		};
	}
}
