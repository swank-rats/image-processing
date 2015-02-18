//============================================================================
// Name        : ObjectDetectionService.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\shared\model\Shot.h"
#include "..\shared\model\Robot.h"

#include <Poco\Logger.h>


using namespace std;
using namespace cv;
using namespace shared::model;

using shared::model::Shot;
using shared::model::Robot;
using Poco::Logger;

namespace services {
	namespace object_detection {
		class ObjectDetectionService
		{
		public:
			ObjectDetectionService();
			virtual ~ObjectDetectionService();

			Robot ObjectDetectionService::DetectRobot(Player player, const Mat &frame);
			Robot ObjectDetectionService::DetectRobotRect(const Mat &frame);
			Robot ObjectDetectionService::DetectRobotPent(const Mat &frame);

			Shot DetectShotRoute(const Mat &frame, Player player, Player hitPlayer); //TODO player parameter
			bool HasShotHitPlayer(const Mat &frame,bool dirtyFrame, SimulationShot &shot);
		private:
			Mat src;
			Mat src_gray;
			int thresh = 100;
			int max_thresh = 255;
		};
	}
}