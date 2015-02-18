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

			Robot DetectRobot(Player player, const Mat &frame);
			Robot DetectRobotRect(const Mat &frame);
			Robot DetectRobotPent(const Mat &frame);
			Robot DetectRobotPentInital(const Mat &frame);
			Robot DetectRobotRectInital(const Mat &frame);

			bool InitalDetection(const Mat &frame);
			bool UpdateRobotPositions(const Mat &frame);

			Shot DetectShotRoute(const Mat &frame, Player player, Player hitPlayer); 
			bool HasShotHitPlayer(const Mat &frame, SimulationShot &shot);
			bool InitalDetection(const Mat &frame, Player player, Player player2);



		private:
			Robot* robotRect;
			Robot* robotCircle;
			Mat src;
			Mat src_gray;
			int thresh = 100;
			int max_thresh = 255;

			bool UpdateRobotPosition(int robotId,const Mat &frame);
		};
	}
}