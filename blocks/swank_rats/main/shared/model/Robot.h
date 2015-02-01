#pragma once

#include <math.h>
#include "diego/opencv/opencv2/core/core.hpp"
#include "diego/opencv/opencv2/opencv.hpp"
#include "diego/opencv/opencv2/highgui/highgui.hpp"

#include <iostream>
#include <string>

using cv::Point;
using cv::vector;

namespace shared {
	namespace model {
		struct Robot
		{

			Point shotStartingPoint;
			Point shotDirection;
			vector<Point> robotForm;

			Robot() { }

			Robot(Point shotStartingPoint, Point shotDirection, vector<Point> robotForm)
				:shotStartingPoint(shotStartingPoint), shotDirection(shotDirection), robotForm(robotForm){}


		};
	}
}