//============================================================================
// Name        : ObjectDetectionService.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ObjectDetectionService.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

RNG rng(12345);

int nextShot = -1;
int shotsSize = 8;
Player p;
Shot shots[] = {
	//top to bottom, right to left
	Shot(p, Point2i(350, 50), Point2i(50, 350)),
	//bottom to top, right to left
	Shot(p, Point2i(350, 350), Point2i(50, 50)),

	//top to bottom, left to right
	Shot(p, Point2i(15, 50), Point2i(380, 240)),
	//bottom to top, left to right
	Shot(p, Point2i(50, 380), Point2i(370, 30)),

	//top to bottom
	Shot(p, Point2i(50, 50), Point2i(50, 350)),
	//bottom to top
	Shot(p, Point2i(50, 350), Point2i(50, 50)),

	//left to right
	Shot(p, Point2i(50, 50), Point2i(400, 50)),
	//right to left
	Shot(p, Point2i(400, 50), Point2i(50, 50))
};

static vector<int> CheckForAllowedPentagons(vector<vector<cv::Point>> pentagons, vector<vector<cv::Point>> triangles, vector<int> pentagonContourPositions, vector<int> triangleContourPositions, vector<int> *allowedTriangles)
{
	int pentagonPos;
	vector<int> allowedPentagonsContourPosition;
	bool found = false;

	if (pentagons.size() == pentagonContourPositions.size()){
		for (size_t i = 0; i < triangles.size(); i++)
		{
			std::vector<cv::Point> triangleSelected = triangles[i];

			for (size_t a = 0; a < pentagons.size(); a++)
			{
				if (triangleSelected.size() != 3)
					break;

				Point2f triPointOne;
				Point2f triPointTwo;
				Point2f triPointThree;

				triPointOne.x = triangleSelected[0].x;
				triPointOne.y = triangleSelected[0].y;

				triPointTwo.x = triangleSelected[1].x;
				triPointTwo.y = triangleSelected[1].y;

				triPointThree.x = triangleSelected[2].x;
				triPointThree.y = triangleSelected[2].y;

				if (pointPolygonTest(Mat(pentagons[a]), triPointOne, true) > 0 && pointPolygonTest(Mat(pentagons[a]), triPointTwo, true) > 0 && pointPolygonTest(Mat(pentagons[a]), triPointThree, true) > 0){
					allowedPentagonsContourPosition.push_back(pentagonContourPositions[a]);
					//allowedTriangles->push_back(triangleContourPositions[i]);
					allowedTriangles->push_back(i);
					found = true;
					break;
				}
			}

			if (found)
				break;
		}
	}

	return  allowedPentagonsContourPosition;
}

static vector<int> CheckForAllowedRectangles(vector<vector<cv::Point>> rectangles, vector<vector<cv::Point>> triangles, vector<int> rectanglesContourPositions, vector<int> triangleContourPositions, vector<int> *allowedTriangles)
{
	int rectPos;
	vector<int> allowedRectanglesContourPositions;
	bool found = false;

	if (rectangles.size() == rectanglesContourPositions.size()){
		for (size_t i = 0; i < triangles.size(); i++)
		{
			std::vector<cv::Point> triSelected = triangles[i];

			for (size_t a = 0; a < rectangles.size(); a++)
			{
				if (triSelected.size() != 3)
					break;

				Point2f triPointOne;
				Point2f triPointTwo;
				Point2f triPointThree;

				triPointOne.x = triSelected[0].x;
				triPointOne.y = triSelected[0].y;

				triPointTwo.x = triSelected[1].x;
				triPointTwo.y = triSelected[1].y;

				triPointThree.x = triSelected[2].x;
				triPointThree.y = triSelected[2].y;

				if (pointPolygonTest(Mat(rectangles[a]), triPointOne, true) > 0 && pointPolygonTest(Mat(rectangles[a]), triPointTwo, true) > 0 && pointPolygonTest(Mat(rectangles[a]), triPointThree, true) > 0){
					allowedRectanglesContourPositions.push_back(rectanglesContourPositions[a]);
					//allowedTriangles->push_back(triangleContourPositions[i]);
					allowedTriangles->push_back(i);
					found = true;
					break;
				}
			}

			if (found)
				break;
		}
	}

	return allowedRectanglesContourPositions;
}

static vector<Point> GetFrontOfTriangle(vector<Point> tri)
{
	Point p1 = tri[0];
	Point p2 = tri[1];
	Point p3 = tri[2];

	Point lP1P2 = p1 - p2;
	Point lP2P3 = p2 - p3;
	Point lP3P1 = p3 - p1;

	double l1 = sqrt((lP1P2.x*lP1P2.x) + (lP1P2.y*lP1P2.y));
	double l2 = sqrt((lP2P3.x*lP2P3.x) + (lP2P3.y*lP2P3.y));
	double l3 = sqrt((lP3P1.x*lP3P1.x) + (lP3P1.y*lP3P1.y));

	double shortes = l1;
	Point toReturn = p3;
	Point dir = Point(lP1P2.x / 2, lP1P2.y / 2) + p2;

	if (l2 < shortes)
	{
		shortes = l2;
		toReturn = p1;
		dir = Point(lP2P3.x / 2, lP2P3.y / 2) + p3;
	}
	if (l3 < shortes)
	{
		shortes = l3;
		toReturn = p2;
		dir = Point(lP3P1.x / 2, lP3P1.y / 2) + p1;
	}

	Point direction = toReturn - dir;
	vector<Point> points;
	points.push_back(direction);
	points.push_back(dir);
	return points;
}

namespace services {
	namespace object_detection {
		ObjectDetectionService::ObjectDetectionService() { }

		ObjectDetectionService::~ObjectDetectionService() { }

		Robot ObjectDetectionService::DetectRobot(Player player, const Mat &frame)
		{
			if (player.playerId == 0)
			{
				return DetectRobotRect(frame);
			}
			if (player.playerId == 1)
			{
				return DetectRobotPent(frame);
			}

			return Robot(); //TODO THOMAS??
		}

		Robot ObjectDetectionService::DetectRobotRect(const Mat &frame){
			Mat srcdetect2;
			Mat src_graydetect2;
			int threshdetect2 = 110;
			int max_threshdetect2 = 255;
			RNG rngdetect2;

			srcdetect2 = frame;

			cvtColor(srcdetect2, src_graydetect2, COLOR_BGR2GRAY);
			blur(src_graydetect2, src_graydetect2, Size(3, 3));

			//Drawing and contours
			Mat canny_output;
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;

			// Finding rects,tris and pentagons
			std::vector<cv::Point> approx;

			vector<vector<cv::Point>> rectangles;
			vector<int> rectanglesContourPositions;
			vector<vector<cv::Point>> triangles;
			vector<int> trianglePositions;

			//Found correct elements
			vector<int> allowedRectanglesContourPositions;
			vector<int> allowedTriangles;

			//dilate(src_graydetect2, src_graydetect2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			/// Detect edges using canny
			Canny(src_graydetect2, canny_output, threshdetect2, threshdetect2 * 2, 3);

			//thresholding the grayscale image to get better results
			threshold(canny_output, canny_output, 128, 255, CV_THRESH_BINARY);

			/// Find contours
			findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

			/// Draw contours
			Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

			/*
			*	Starting detection process
			*/

			for (size_t i = 0; i < contours.size(); i++)
			{
				// Approximate contour with accuracy proportional
				// to the contour perimeter
				// original elipse value 0.02
				cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.1, true);

				// Skip small or non-convex objects
				if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
					continue;

				// Rectangles

				if (approx.size() == 4)
				{
					rectangles.push_back(contours[i]);
					rectanglesContourPositions.push_back(i);
				}

				if (approx.size() == 3)
				{
					triangles.push_back(approx);
					trianglePositions.push_back(i);
				}
			}

			allowedRectanglesContourPositions = CheckForAllowedRectangles(rectangles, triangles, rectanglesContourPositions, trianglePositions, &allowedTriangles);
			vector<Point> pointsTriRect;
			vector<Point> contoursRect;
			if (allowedTriangles.size() > 0){
				pointsTriRect = GetFrontOfTriangle(triangles[allowedTriangles[0]]);
			}

			if (allowedRectanglesContourPositions.size() > 0)
			{
				contoursRect = contours[allowedRectanglesContourPositions[0]];
			}

			if (pointsTriRect.size() == 2)
				return Robot(pointsTriRect[1], pointsTriRect[0], contoursRect);
			else
				return Robot(Point(), Point(), contoursRect);
		}

		Robot ObjectDetectionService::DetectRobotPent(const Mat &frame){
			Mat srcdetect2;
			Mat src_graydetect2;
			int threshdetect2 = 110;
			int max_threshdetect2 = 255;
			RNG rngdetect2;

			srcdetect2 = frame;

			cvtColor(srcdetect2, src_graydetect2, COLOR_BGR2GRAY);
			blur(src_graydetect2, src_graydetect2, Size(3, 3));

			//Drawing and contours
			Mat canny_output;
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;

			// Finding rects,tris and pentagons
			std::vector<cv::Point> approx;

			vector<vector<cv::Point>> pentagons;
			vector<int> pentagonsContourPositions;
			vector<vector<cv::Point>> triangles;
			vector<int> trianglePositions;

			//Found correct elements
			vector<int> allowedPentagonsContourPosition;
			vector<int> allowedTrianglesPoly;

			//dilate(src_graydetect2, src_graydetect2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			/// Detect edges using canny
			Canny(src_graydetect2, canny_output, threshdetect2, threshdetect2 * 2, 3);

			//thresholding the grayscale image to get better results
			threshold(canny_output, canny_output, 128, 255, CV_THRESH_BINARY);

			/// Find contours
			findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

			/// Draw contours
			Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

			/*
			*	Starting detection process
			*/

			for (size_t i = 0; i < contours.size(); i++)
			{
				// Approximate contour with accuracy proportional
				// to the contour perimeter
				// original elipse value 0.02
				cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.1, true);

				// Skip small or non-convex objects
				if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
					continue;

				// Rectangles

				if (approx.size() == 3)
				{
					triangles.push_back(approx);
					trianglePositions.push_back(i);
				}

				if (approx.size() == 5)
				{
					pentagons.push_back(contours[i]);
					pentagonsContourPositions.push_back(i);
				}
			}

			allowedPentagonsContourPosition = CheckForAllowedPentagons(pentagons, triangles, pentagonsContourPositions, trianglePositions, &allowedTrianglesPoly);

			vector<Point> pointsTriRect;
			vector<Point> contoursPent;
			if (allowedTrianglesPoly.size() > 0){
				pointsTriRect = GetFrontOfTriangle(triangles[allowedTrianglesPoly[0]]);
			}

			if (allowedPentagonsContourPosition.size() > 0)
			{
				contoursPent = contours[allowedPentagonsContourPosition[0]];
			}
			if (pointsTriRect.size() == 2)
				return Robot(pointsTriRect[1], pointsTriRect[0], contoursPent);
			else
				return Robot(Point(), Point(), contoursPent);
		}

		Shot ObjectDetectionService::DetectShotRoute(const Mat &frame, Player player) {
			/*Robot robotShootPlayer = DetectRobot(player, frame);

			if (robotShootPlayer.robotForm.size() <= 0)
			return Shot();

			double length = sqrt(pow(robotShootPlayer.shotDirection.x, 2) + pow(robotShootPlayer.shotDirection.y, 2));

			int multiplier = 5;
			Point normDirection = Point(robotShootPlayer.shotDirection.x / length * multiplier, robotShootPlayer.shotDirection.x / length * multiplier);
			bool found = false;
			Point endPoint;
			Point startPoint = robotShootPlayer.shotStartingPoint + normDirection;
			Point currentPoint = startPoint;
			cv::Rect rect(cv::Point(), frame.size());

			while (!found)
			{
			if(!rect.contains(currentPoint))
			{
			endPoint = currentPoint-normDirection;
			found = true;
			}

			currentPoint += normDirection;
			}

			return Shot(player, Point2i(robotShootPlayer.shotStartingPoint.x, robotShootPlayer.shotStartingPoint.y), Point2i(endPoint.x, endPoint.y));*/

			// TODO always calculate beginning at the robot til a wall is hit because we do not know if finally a robot or a wall will be hitten
			++nextShot;

			if (nextShot >= shotsSize) {
				nextShot = 0;
			}

			return shots[nextShot];
		}

		bool ObjectDetectionService::HasShotHitPlayer(const Mat &frame, SimulationShot &shot) {
			// TODO analyse frame and detect if a player is at shot endposition
			//1. find opposite player

			shot.hitPlayer = p;

			Robot robotHitPlayer = DetectRobot(shot.hitPlayer, frame);

			if (robotHitPlayer.robotForm.size() <= 0)
				return false;

			Point2i tmp = shot.GetCurrentShotPoint();
			Point2f currentShotingPoint = Point2f(tmp.x, tmp.y);

			Logger& logger = Logger::get("ObjectDetectionService");

			logger.information("Found Rect");

			logger.information("Point: ");
			logger.information("X: " + std::to_string(tmp.x));
			logger.information("y: " + std::to_string(tmp.y));

			if (pointPolygonTest(Mat(robotHitPlayer.robotForm), currentShotingPoint, true) > 0)
				return true;
			else
				return false;
		}
	}
}