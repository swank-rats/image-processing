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

#include <Poco\Stopwatch.h>

using namespace cv;
using namespace std;

using Poco::Stopwatch;

RNG rng(12345);


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
		ObjectDetectionService::ObjectDetectionService() {
			robotRect = nullptr;
			robotCircle = nullptr;
			//namedWindow("test2");
			//namedWindow("test3");
			//namedWindow("test4");
			//namedWindow("test5");
			//namedWindow("test6");
		}

		ObjectDetectionService::~ObjectDetectionService() {
			delete robotCircle;
			robotCircle = nullptr;
			delete robotRect;
			robotRect = nullptr;
		}

		Robot ObjectDetectionService::DetectRobot(Player player, const Mat &frame)
		{
			if (frame.empty()) {
				return Robot();
			}

			if (player.playerId == 0)
			{
				return DetectRobotRect(frame);
			}
			if (player.playerId == 1)
			{
				return DetectRobotPent(frame);
			}

			return Robot();
		}

		Robot ObjectDetectionService::DetectRobotRectInital(const Mat &frame){
			Mat srcdetect2;
			Mat src_graydetect2;
			//int threshdetect2 = 78;
			int threshdetect2 = 100;
			int max_threshdetect2 = 255;
			RNG rngdetect2;

			//Drawing and contours
			Mat canny_output2;

			vector<vector<Point> > contours2;
			vector<Vec4i> hierarchy2;

			std::vector<cv::Point> approx2;

			vector<vector<cv::Point>> triangles;
			vector<int> trianglePositions;
			vector<vector<cv::Point>> rectangles;

			//int iLowH = 0;
			//int iHighH = 179;

			//int iLowS = 0;
			//int iHighS = 129;

			//int iLowV = 0;
			//int iHighV = 255;

			int iLowH = 0;
			int iHighH = 179;

			int iLowS = 0;
			int iHighS = 244;

			int iLowV = 0;
			int iHighV = 245;
			//int iHighV = 253;

			srcdetect2 = frame;
			Mat imgHSV;

			cvtColor(srcdetect2, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

			Mat imgThresholded;

			inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

			//morphological opening (removes small objects from the foreground)
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//morphological closing (removes small holes from the foreground)
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			/// Detect edges using canny
			Canny(imgThresholded, canny_output2, threshdetect2, threshdetect2 * 2, 3);

			//thresholding the grayscale canny_output2 to get better results
			threshold(canny_output2, canny_output2, 128, 255, CV_THRESH_BINARY);

			/// Find contours
			findContours(canny_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

			for (size_t i = 0; i < contours2.size(); i++)
			{
				// Approximate contour with accuracy proportional
				// to the contour perimeter
				// original elipse value 0.02
				cv::approxPolyDP(cv::Mat(contours2[i]), approx2, cv::arcLength(cv::Mat(contours2[i]), true)*0.1, true);

				// Skip small or non-convex objects
				if (std::fabs(cv::contourArea(contours2[i])) < 300 || !cv::isContourConvex(approx2))
					continue;

				if (approx2.size() == 3)
				{
					triangles.push_back(approx2);
					trianglePositions.push_back(i);
				}
				if (approx2.size() == 4)
				{
					rectangles.push_back(approx2);
				}
			}

			vector<Point> pointsTriRect;
			vector<Point> contoursRect;
			bool found = false;

			if (rectangles.size() > 0)
			{
				if (triangles.size() > 0){
					Point dir;
					Point PosTop;

					for (size_t i = 0; i < triangles.size(); i++)
					{
						pointsTriRect = GetFrontOfTriangle(triangles[i]);
						dir = pointsTriRect[0];
						PosTop = pointsTriRect[1];
						if (pointPolygonTest(Mat(rectangles[0]), PosTop, true) > 0)
						{
							found = true;
							break;
						}
					}

					if (found)
					{
						Point centerTri(PosTop.x - dir.x / 2, PosTop.y - dir.y / 2);

						Point x(centerTri.x - 50, centerTri.y - 50);
						Point y(centerTri.x + 50, centerTri.y + 50);
						Point z(centerTri.x - 50, centerTri.y + 50);
						Point v(centerTri.x - 50, centerTri.y + 50);


						contoursRect.push_back(x);
						contoursRect.push_back(y);
						contoursRect.push_back(z);
						contoursRect.push_back(v);
					}
				}
			}

			if (pointsTriRect.size() == 2 && found)
				return Robot(pointsTriRect[1], pointsTriRect[0], contoursRect);
			else
				return Robot(Point(), Point(), contoursRect);
		}

		Robot ObjectDetectionService::DetectRobotPentInital(const Mat &frame){
			Mat srcdetect2;
			Mat src_graydetect2;
			//int threshdetect2 = 78;
			int threshdetect2 = 100;
			int max_threshdetect2 = 255;
			RNG rngdetect2;

			//Drawing and contours
			Mat canny_output2;

			vector<vector<Point> > contours2;
			vector<Vec4i> hierarchy2;

			std::vector<cv::Point> approx2;

			vector<vector<cv::Point>> triangles;
			vector<int> trianglePositions;
			vector<vector<cv::Point>> rectangles;

			//int iLowH = 0;
			//int iHighH = 179;

			//int iLowS = 0;
			//int iHighS = 255;

			//int iLowV = 0;
			//int iHighV = 60;

			int iLowH = 0;
			int iHighH = 179;

			int iLowS = 0;
			int iHighS = 244;

			int iLowV = 0;
			int iHighV = 245;
			//int iHighV = 253;

			srcdetect2 = frame;
			Mat imgHSV;

			cvtColor(srcdetect2, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

			Mat imgThresholded;

			inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

			//morphological opening (removes small objects from the foreground)
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//morphological closing (removes small holes from the foreground)
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			/// Detect edges using canny
			Canny(imgThresholded, canny_output2, threshdetect2, threshdetect2 * 2, 3);

			//thresholding the grayscale canny_output2 to get better results
			threshold(canny_output2, canny_output2, 128, 255, CV_THRESH_BINARY);

			/// Find contours
			findContours(canny_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

			for (size_t i = 0; i < contours2.size(); i++)
			{
				// Approximate contour with accuracy proportional
				// to the contour perimeter
				// original elipse value 0.02
				cv::approxPolyDP(cv::Mat(contours2[i]), approx2, cv::arcLength(cv::Mat(contours2[i]), true)*0.1, true);

				// Skip small or non-convex objects
				if (std::fabs(cv::contourArea(contours2[i])) < 300 || !cv::isContourConvex(approx2))
					continue;

				if (approx2.size() == 3)
				{
					triangles.push_back(approx2);
					trianglePositions.push_back(i);
				}
				if (approx2.size() == 4)
				{
					rectangles.push_back(approx2);
				}

				
			}

			vector<Point> pointsTriRect;
			vector<Point> contoursRect;
			bool found = false;

			if (rectangles.size() > 0)
			{
				if (triangles.size() > 0){
					Point dir;
					Point PosTop;

					for (size_t i = 0; i < triangles.size(); i++)
					{
						pointsTriRect = GetFrontOfTriangle(triangles[i]);
						dir = pointsTriRect[0];
						PosTop = pointsTriRect[1];
						if (pointPolygonTest(Mat(rectangles[0]), PosTop, true) < 0)
						{
							found = true;
							break;
						}
					}

					if (found)
					{
						Point centerTri(PosTop.x - dir.x / 2, PosTop.y - dir.y / 2);

						Point x(centerTri.x - 40, centerTri.y - 40);
						Point y(centerTri.x + 40, centerTri.y + 40);
						Point z(centerTri.x - 40, centerTri.y + 40);
						Point v(centerTri.x - 40, centerTri.y + 40);

						contoursRect.push_back(x);
						contoursRect.push_back(y);
						contoursRect.push_back(z);
						contoursRect.push_back(v);
					}
				}
			}

			for (size_t i = 0; i < contours2.size(); i++)
			{
				Scalar color = Scalar(255, 0, 0);
				drawContours(frame, contours2, (int)i, color, 2, 8, hierarchy2, 0, Point());

			}

			//imshow("test3", frame);

			if (pointsTriRect.size() == 2 && found)
				return Robot(pointsTriRect[1], pointsTriRect[0], contoursRect);
			else
				return Robot(Point(), Point(), contoursRect);
		}


		Robot ObjectDetectionService::DetectRobotRect(const Mat &frame){
			Mat srcdetect2;
			Mat src_graydetect2;
			//int threshdetect2 = 78;
			int threshdetect2 = 100;
			int max_threshdetect2 = 255;
			RNG rngdetect2;

			//Drawing and contours
			Mat canny_output2;

			vector<vector<Point> > contours2;
			vector<Vec4i> hierarchy2;

			std::vector<cv::Point> approx2;

			vector<vector<cv::Point>> triangles;
			vector<int> trianglePositions;
			vector<vector<cv::Point>> rectangles;

			//int iLowH = 0;
			//int iHighH = 179;

			//int iLowS = 0;
			//int iHighS = 129;

			//int iLowV = 0;
			//int iHighV = 255;

			int iLowH = 0;
			int iHighH = 179;

			int iLowS = 0;
			int iHighS = 244;

			int iLowV = 0;
			int iHighV = 245;
			//int iHighV = 253;

			srcdetect2 = frame;
			Mat imgHSV;

			cvtColor(srcdetect2, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

			Mat imgThresholded;

			inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

			//morphological opening (removes small objects from the foreground)
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//morphological closing (removes small holes from the foreground)
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			/// Detect edges using canny
			Canny(imgThresholded, canny_output2, threshdetect2, threshdetect2 * 2, 3);

			//thresholding the grayscale canny_output2 to get better results
			threshold(canny_output2, canny_output2, 128, 255, CV_THRESH_BINARY);

			/// Find contours
			findContours(canny_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

			for (size_t i = 0; i < contours2.size(); i++)
			{
				// Approximate contour with accuracy proportional
				// to the contour perimeter
				// original elipse value 0.02
				cv::approxPolyDP(cv::Mat(contours2[i]), approx2, cv::arcLength(cv::Mat(contours2[i]), true)*0.1, true);

				// Skip small or non-convex objects
				if (std::fabs(cv::contourArea(contours2[i])) < 300 || !cv::isContourConvex(approx2))
					continue;

				if (approx2.size() == 3)
				{
					triangles.push_back(approx2);
					trianglePositions.push_back(i);
				}
				if (approx2.size() == 4)
				{
					rectangles.push_back(approx2);
				}
			}

			vector<Point> pointsTriRect;
			vector<Point> contoursRect;
			bool found = false;

			if (rectangles.size() > 0)
			{
				if (triangles.size() > 0){
					Point dir;
					Point PosTop;

					for (size_t i = 0; i < triangles.size(); i++)
					{
						pointsTriRect = GetFrontOfTriangle(triangles[i]);
						dir = pointsTriRect[0];
						PosTop = pointsTriRect[1];
						if (pointPolygonTest(Mat(rectangles[0]), PosTop, true) > 0)
						{
							found = true;
							break;
						}
					}

					if (found)
					{
						Point centerTri(PosTop.x - dir.x / 2, PosTop.y - dir.y / 2);

						Point x(centerTri.x - 50, centerTri.y - 50);
						Point y(centerTri.x + 50, centerTri.y + 50);
						Point z(centerTri.x - 50, centerTri.y + 50);
						Point v(centerTri.x - 50, centerTri.y + 50);


						contoursRect.push_back(x);
						contoursRect.push_back(y);
						contoursRect.push_back(z);
						contoursRect.push_back(v);
					}
				}
			}

			/*for (size_t i = 0; i < contours2.size(); i++)
			{
				Scalar color = Scalar(255, 0, 0);
				drawContours(frame, contours2, (int)i, color, 2, 8, hierarchy2, 0, Point());

			}

			imshow("test5", frame);*/

			if (pointsTriRect.size() == 2 && found)
				return Robot(pointsTriRect[1], pointsTriRect[0], contoursRect);
			else
				return Robot(Point(), Point(), contoursRect);
		}

		Robot ObjectDetectionService::DetectRobotPent(const Mat &frame){
			Mat srcdetect2;
			Mat src_graydetect2;
			//int threshdetect2 = 78;
			int threshdetect2 = 100;
			int max_threshdetect2 = 255;
			RNG rngdetect2;

			//Drawing and contours
			Mat canny_output2;

			vector<vector<Point> > contours2;
			vector<Vec4i> hierarchy2;

			std::vector<cv::Point> approx2;

			vector<vector<cv::Point>> triangles;
			vector<int> trianglePositions;
			vector<vector<cv::Point>> rectangles;

			//int iLowH = 0;
			//int iHighH = 179;

			//int iLowS = 0;
			//int iHighS = 255;

			//int iLowV = 0;
			//int iHighV = 60;

			int iLowH = 0;
			int iHighH = 179;

			int iLowS = 0;
			int iHighS = 244;

			int iLowV = 0;
			int iHighV = 245;
			//int iHighV = 253;

			srcdetect2 = frame;
			Mat imgHSV;

			cvtColor(srcdetect2, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

			Mat imgThresholded;

			inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

			//morphological opening (removes small objects from the foreground)
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//morphological closing (removes small holes from the foreground)
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			/// Detect edges using canny
			Canny(imgThresholded, canny_output2, threshdetect2, threshdetect2 * 2, 3);

			//thresholding the grayscale canny_output2 to get better results
			threshold(canny_output2, canny_output2, 128, 255, CV_THRESH_BINARY);

			/// Find contours
			findContours(canny_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

			for (size_t i = 0; i < contours2.size(); i++)
			{
				// Approximate contour with accuracy proportional
				// to the contour perimeter
				// original elipse value 0.02
				cv::approxPolyDP(cv::Mat(contours2[i]), approx2, cv::arcLength(cv::Mat(contours2[i]), true)*0.1, true);

				// Skip small or non-convex objects
				if (std::fabs(cv::contourArea(contours2[i])) < 300 || !cv::isContourConvex(approx2))
					continue;

				if (approx2.size() == 3)
				{
					triangles.push_back(approx2);
					trianglePositions.push_back(i);
				}
				if (approx2.size() == 4)
				{
					rectangles.push_back(approx2);
				}


			}

			vector<Point> pointsTriRect;
			vector<Point> contoursRect;
			bool found = false;

			if (rectangles.size() > 0)
			{
				if (triangles.size() > 0){
					Point dir;
					Point PosTop;

					for (size_t i = 0; i < triangles.size(); i++)
					{
						pointsTriRect = GetFrontOfTriangle(triangles[i]);
						dir = pointsTriRect[0];
						PosTop = pointsTriRect[1];
						if (pointPolygonTest(Mat(rectangles[0]), PosTop, true) < 0)
						{
							found = true;
							break;
						}
					}

					if (found)
					{
						Point centerTri(PosTop.x - dir.x / 2, PosTop.y - dir.y / 2);

						Point x(centerTri.x - 40, centerTri.y - 40);
						Point y(centerTri.x + 40, centerTri.y + 40);
						Point z(centerTri.x - 40, centerTri.y + 40);
						Point v(centerTri.x - 40, centerTri.y + 40);

						contoursRect.push_back(x);
						contoursRect.push_back(y);
						contoursRect.push_back(z);
						contoursRect.push_back(v);
					}
				}
			}
			else
			{
				if (triangles.size() > 0){
					Point dir;
					Point PosTop;

					for (size_t i = 0; i < triangles.size(); i++)
					{
						pointsTriRect = GetFrontOfTriangle(triangles[i]);
						dir = pointsTriRect[0];
						PosTop = pointsTriRect[1];
						if (pointsTriRect.size()>0)
						{
							found = true;
							break;
						}
					}

					if (found)
					{
						Point centerTri(PosTop.x - dir.x / 2, PosTop.y - dir.y / 2);

						Point x(centerTri.x - 40, centerTri.y - 40);
						Point y(centerTri.x + 40, centerTri.y + 40);
						Point z(centerTri.x - 40, centerTri.y + 40);
						Point v(centerTri.x - 40, centerTri.y + 40);

						contoursRect.push_back(x);
						contoursRect.push_back(y);
						contoursRect.push_back(z);
						contoursRect.push_back(v);
					}
				}
			}

			/*for (size_t i = 0; i < contours2.size(); i++)
			{
				Scalar color = Scalar(255, 0, 0);
				drawContours(frame, contours2, (int)i, color, 2, 8, hierarchy2, 0, Point());

			}

			imshow("test3", frame);*/

			if (pointsTriRect.size() == 2 && found)
				return Robot(pointsTriRect[1], pointsTriRect[0], contoursRect);
			else
				return Robot(Point(), Point(), contoursRect);
		}


		bool ObjectDetectionService::InitalDetection(const Mat &frame)
		{

			Robot tmpRobotRect = DetectRobotRectInital(frame);
			Robot tmpRobotCircle = DetectRobotPentInital(frame);

			if (tmpRobotRect.robotForm.size() <= 0 || tmpRobotCircle.robotForm.size() <= 0)
			{
				return false;
			}

		

			Point posRobotRect(tmpRobotRect.shotStartingPoint.x - tmpRobotRect.shotDirection.x / 2, tmpRobotRect.shotStartingPoint.y - tmpRobotRect.shotDirection.y / 2);
			Point posRobotCircle(tmpRobotCircle.shotStartingPoint.x - tmpRobotCircle.shotDirection.x / 2, tmpRobotCircle.shotStartingPoint.y - tmpRobotCircle.shotDirection.y / 2);


			robotRect = new Robot(tmpRobotRect.shotStartingPoint, tmpRobotRect.shotDirection, posRobotRect, tmpRobotRect.robotForm);
			robotCircle = new Robot(tmpRobotCircle.shotStartingPoint, tmpRobotCircle.shotDirection, posRobotCircle, tmpRobotCircle.robotForm);

		

			return true;
		}

		bool ObjectDetectionService::UpdateRobotPositions(const Mat &frame)
		{
			return UpdateRobotPosition(0, frame) && UpdateRobotPosition(1, frame);

		}

		bool  ObjectDetectionService::UpdateRobotPosition(int robotId, const Mat &frame)
		{

			Robot* actuelRobot = nullptr;

			if (robotId == 0)
			{
				actuelRobot = robotRect;
			}
			if (robotId == 1)
			{
				actuelRobot = robotCircle;
			}

			if (actuelRobot)
			{
				int x1 = actuelRobot->currentPosition.x <= 100 ? 0 : actuelRobot->currentPosition.x - 100;
				int x2 = actuelRobot->currentPosition.x >= frame.cols - 100 ? frame.cols : actuelRobot->currentPosition.x + 100;
				int y1 = actuelRobot->currentPosition.y <= 100 ? 0 : actuelRobot->currentPosition.y - 100;
				int y2 = actuelRobot->currentPosition.y >= frame.rows - 100 ? frame.rows : actuelRobot->currentPosition.y + 100;


				Point left(x1, y1);
				Point right(x2, y2);

				Mat tmp;
				frame(Rect(left, right)).copyTo(tmp);

				//if (robotId == 0)
				//{
				//	imshow("test2", tmp);
				//}
				//if (robotId == 1)
				//{
				//	imshow("test4", tmp);
				//} 
				
				Player p;
				p.playerId = robotId;

				Robot tmpRobot = DetectRobot(p, tmp);

				if (tmpRobot.robotForm.size() <= 0)
				{
					return false;
				}


				//Point dir(tmpRobot.shotDirection.x + left.x, tmpRobot.shotDirection.y + left.y);
				Point dir(tmpRobot.shotDirection.x, tmpRobot.shotDirection.y);

				Point PosTop(tmpRobot.shotStartingPoint.x + left.x, tmpRobot.shotStartingPoint.y + left.y);
				Point centerTri(PosTop.x - dir.x / 2, PosTop.y - dir.y / 2);




				actuelRobot->currentPosition.x = centerTri.x;
				actuelRobot->currentPosition.y = centerTri.y;

				actuelRobot->shotDirection.x = dir.x;
				actuelRobot->shotDirection.y = dir.y;

				actuelRobot->shotStartingPoint.x = PosTop.x;
				actuelRobot->shotStartingPoint.y = PosTop.y;


				actuelRobot->robotForm[0].x += left.x;
				actuelRobot->robotForm[0].y += left.y;
				actuelRobot->robotForm[1].x += left.x;
				actuelRobot->robotForm[1].y += left.y;
				actuelRobot->robotForm[2].x += left.x;
				actuelRobot->robotForm[2].y += left.y;
				actuelRobot->robotForm[3].x += left.x;
				actuelRobot->robotForm[3].y += left.y;

				return true;
			}

			return false;
		}


		Shot ObjectDetectionService::DetectShotRoute(const Mat &frame, Player player, Player hitPlayer) {

			Robot* actuelRobot = nullptr;

			if (player.playerId == 0)
			{
				actuelRobot = robotRect;
			}
			if (player.playerId == 1)
			{
				actuelRobot = robotCircle;
			}


			if (!actuelRobot)
			{
				return Shot(player, hitPlayer, Point2i(0, 0), Point2i(0, 0));
			}

			double length = sqrt(pow(actuelRobot->shotDirection.x, 2) + pow(actuelRobot->shotDirection.y, 2));

			int multiplier = 5;
			Point normDirection = Point(actuelRobot->shotDirection.x / length * multiplier, actuelRobot->shotDirection.y / length * multiplier);
			bool found = false;
			Point endPoint;
			Point startPoint = actuelRobot->shotStartingPoint + normDirection;
			Point currentPoint = startPoint;
			cv::Rect rect(cv::Point(), frame.size());

			while (!found)
			{
				if (!rect.contains(currentPoint))
				{
					endPoint = currentPoint - 2 * normDirection;
					found = true;
				}

				currentPoint += normDirection;
			}

			return Shot(player, hitPlayer, Point2i(actuelRobot->shotStartingPoint.x, actuelRobot->shotStartingPoint.y), Point2i(endPoint.x, endPoint.y));

		}

		bool ObjectDetectionService::HasShotHitPlayer(const Mat &frame, SimulationShot &shot) {
			//Stopwatch sw;
			//sw.start();

			Point* actuelPosition = nullptr;

			Point2i tmp = shot.GetCurrentShotPoint();

			//sw.stop();
			//printf("GetCurrentShotPoint: %f ms\n", sw.elapsed() * 0.001);

			Point2f currentShotingPoint = Point2f(tmp.x, tmp.y);


			if (shot.hitPlayer.playerId == 0)
			{
				actuelPosition = &robotRect->currentPosition;
			}
			if (shot.hitPlayer.playerId == 1)
			{
				actuelPosition = &robotCircle->currentPosition;
			}


			if (actuelPosition)
			{

				int diffx = abs(actuelPosition->x - currentShotingPoint.x);
				int diffy = abs(actuelPosition->y - currentShotingPoint.y);

				if (diffx > 100 || diffy > 100)
				{
					return false;
				}

				vector<Point> hitArea;

				Point x(actuelPosition->x - 50, actuelPosition->y - 50);
				Point y(actuelPosition->x + 50, actuelPosition->y + 50);
				Point z(actuelPosition->x - 50, actuelPosition->y + 50);
				Point v(actuelPosition->x - 50, actuelPosition->y + 50);


				hitArea.push_back(x);
				hitArea.push_back(y);
				hitArea.push_back(z);
				hitArea.push_back(v);


				if (pointPolygonTest(Mat(hitArea), currentShotingPoint, true) > 0)
				{
					//sw.stop();
					//printf("Has hit player: %f ms\n", sw.elapsed() * 0.001);
					return true;
				}
				else
				{
					//sw.stop();
					//printf("Has not hit player: %f ms\n", sw.elapsed() * 0.001);
					return false;
				}
			}

			return false;

		}
	}
}