//============================================================================
// Name        : IObserver.h
// Author      : ITM13
// Version     : 1.0

// Description : 
//============================================================================
#pragma once
#include <opencv2\opencv.hpp>

using cv::Point2i;

namespace shared {
	namespace model {
		enum ShotEndPointType { Robot, Wall };

		struct Shot
		{
			Point2i startPoint;
			Point2i endPoint;
			ShotEndPointType endPointType;

			Shot(Point2i start, Point2i end, ShotEndPointType type)
			: startPoint(start), endPoint(end), endPointType(type) {
			}
		};
	}
}
