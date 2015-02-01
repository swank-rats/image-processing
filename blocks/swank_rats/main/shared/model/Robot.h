#pragma once

#include <opencv2\core\core.hpp>
#include <math.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

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