//============================================================================
// Name        : IObserver.h
// Author      : ITM13
// Version     : 1.0

// Description :
//============================================================================
#pragma once
#include <opencv2\core\core.hpp>

using cv::Point2i;

namespace shared {
	namespace model {
		enum ShotEndPointType { Robot, Wall };

		struct Shot
		{
			Point2i startPoint;
			Point2i endPoint;
			ShotEndPointType endPointType;

			Shot() { }

			Shot(Point2i start, Point2i end, ShotEndPointType type)
				: startPoint(start), endPoint(end), endPointType(type) { }

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
					&& endPoint.y == s2.endPoint.y
					&& endPointType == s2.endPointType;
			}

			bool operator != (const Shot &s2) const
			{
				return startPoint.x != s2.startPoint.x
					|| startPoint.y != s2.startPoint.y
					|| endPoint.x != s2.endPoint.x
					|| endPoint.y != s2.endPoint.y
					|| endPointType != s2.endPointType;
			}
		};
	}
}
