////============================================================================
//// Name        : main.cpp
//// Author      : ITM13
//// Version     : 1.0
//// Description : Image process start point
////============================================================================
//
//#include <iostream>
//#include <list>
//
//#undef max 
//#undef min
//
//#include <opencv2\core\core.hpp>
//#include <opencv2\highgui\highgui.hpp>
//#include <opencv2\opencv.hpp>
//#include <opencv2\video\background_segm.hpp>
//
//#include "services\webcam\WebcamService.h"
//#include "services\ObjectDetectionService.h"
//#include "controller\ImageProcessingController.h"
//#include "Poco/HashMap.h"
//#
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/video/background_segm.hpp"
//#include <stdio.h>
//#include <string>
//
//#include <opencv\cv.h>
//#include <opencv2\flann\lsh_table.h>
////#include <limits>
//
//#include "opencv2/imgproc/imgproc.hpp"
//using namespace cv;
//using namespace std;
//
//static Mat srcdetect2;
//static Mat src_graydetect2;
//static int threshdetect2;
//static int max_threshdetect2;
//static RNG rngdetect2;
//
//
//
////our sensitivity value to be used in the threshold() function
//const static int SENSITIVITY_VALUE = 20;
////size of blur used to smooth the image to remove possible noise and
////increase the size of the object we are trying to track. (Much like dilate and erode)
//const static int BLUR_SIZE = 10;
////we'll have just one object to search for
////and keep track of its position.
//static int theObject[2] = { 0, 0 };
////bounding rectangle of the object, we will use the center of this as its position.
//static Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
//
//
//
//
//
//
///// Global Variables
//static Mat img; static Mat templ; static Mat result;
//const static char* image_window = "Source Image";
//const static char* result_window = "Result window";
//const static char* search_window = "Search window";
//
//static int match_method;
//static int max_Trackbar = 5;
//
///// Function Headers
//void MatchingMethod(int, void*);
//
///**
//* @function MatchingMethod
//* @brief Trackbar callback
//*/
//static void MatchingMethod(int, void*)
//{
//	/// Source image to display
//	Mat img_display;
//	img.copyTo(img_display);
//
//	/// Create the result matrix
//	int result_cols = img.cols - templ.cols + 1;
//	int result_rows = img.rows - templ.rows + 1;
//
//	result.create(result_rows, result_cols, CV_32FC1);
//
//	/// Do the Matching and Normalize
//	matchTemplate(img, templ, result, match_method);
//	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
//
//	/// Localizing the best match with minMaxLoc
//	double minVal; double maxVal; Point minLoc; Point maxLoc;
//	Point matchLoc;
//
//	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
//
//
//	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
//	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
//	{
//		matchLoc = minLoc;
//	}
//	else
//	{
//		matchLoc = maxLoc;
//	}
//
//	/// Show me what you got
//	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
//	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
//
//	imshow(image_window, img_display);
//	imshow(result_window, result);
//	imshow(search_window, templ);
//
//	return;
//}
//
//
//
//
////int to string helper function
//static string intToString(int number){
//
//	//this function has a number input and string outpu
//	std::stringstream ss;
//	ss << number;
//	return ss.str();
//}
////
////static void searchForMovement(Mat thresholdImage, Mat &cameraFeed){
////	//notice how we use the '&' operator for the cameraFeed. This is because we wish
////	//to take the values passed into the function and manipulate them, rather than just working with a copy.
////	//eg. we draw to the cameraFeed in this function which is then displayed in the main() function.
////	bool objectDetected = false;
////	Mat temp;
////	thresholdImage.copyTo(temp);
////	//these two vectors needed for output of findContours
////	vector< vector<Point> > contours;
////	vector<Vec4i> hierarchy;
////	//find contours of filtered image using openCV findContours function
////	//findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
////	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);// retrieves external contours
////
////	//if contours vector is not empty, we have found some objects
////	if (contours.size() > 0)objectDetected = true;
////	else objectDetected = false;
////
////	if (objectDetected){
////		//the largest contour is found at the end of the contours vector
////		//we will simply assume that the biggest contour is the object we are looking for.
////		vector< vector<Point> > largestContourVec;
////		largestContourVec.push_back(contours.at(contours.size() - 1));
////		//make a bounding rectangle around the largest contour then find its centroid
////		//this will be the object's final estimated position.
////		objectBoundingRectangle = boundingRect(largestContourVec.at(0));
////		int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
////		int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
////
////		//update the objects positions by changing the 'theObject' array values
////		theObject[0] = xpos, theObject[1] = ypos;
////	}
////	//make some temp x and y variables so we dont have to type out so much
////	int x = theObject[0];
////	int y = theObject[1];
////	//draw some crosshairs on the object
////	circle(cameraFeed, Point(x, y), 20, Scalar(0, 255, 0), 2);
////	line(cameraFeed, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
////	line(cameraFeed, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
////	line(cameraFeed, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
////	line(cameraFeed, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
////	putText(cameraFeed, "Tracking object at (" + intToString(x) + "," + intToString(y) + ")", Point(x, y), 1, 1, Scalar(255, 0, 0), 2);
////
////
////
////}
////
////
////static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
////{
////	int niters = 3;
////
////	vector<vector<Point> > contours;
////	vector<Vec4i> hierarchy;
////
////	Mat temp;
////
////	dilate(mask, temp, Mat(), Point(-1, -1), niters);
////	erode(temp, temp, Mat(), Point(-1, -1), niters * 2);
////	dilate(temp, temp, Mat(), Point(-1, -1), niters);
////
////	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
////
////	dst = Mat::zeros(img.size(), CV_8UC3);
////
////	if (contours.size() == 0)
////		return;
////
////	// iterate through all the top-level contours,
////	// draw each connected component with its own random color
////	int idx = 0, largestComp = 0;
////	double maxArea = 0;
////
////	for (; idx >= 0; idx = hierarchy[idx][0])
////	{
////		const vector<Point>& c = contours[idx];
////		double area = fabs(contourArea(Mat(c)));
////		if (area > maxArea)
////		{
////			maxArea = area;
////			largestComp = idx;
////		}
////	}
////	Scalar color(0, 0, 255);
////	drawContours(dst, contours, largestComp, color, CV_FILLED, 8, hierarchy);
////}
//
//static vector<int> CheckForAllowedPentagons(vector<vector<cv::Point>> pentagons, vector<vector<cv::Point>> triangles, vector<int> pentagonContourPositions, vector<int> triangleContourPositions, vector<int> *allowedTriangles)
//{
//	int pentagonPos;
//	vector<int> allowedPentagonsContourPosition;
//	bool found = false;
//
//	if (pentagons.size() == pentagonContourPositions.size()){
//		for (size_t i = 0; i < triangles.size(); i++)
//		{
//			std::vector<cv::Point> triangleSelected = triangles[i];
//
//			for (size_t a = 0; a < pentagons.size(); a++)
//			{
//				if (triangleSelected.size() != 3)
//					break;
//
//				Point2f triPointOne;
//				Point2f triPointTwo;
//				Point2f triPointThree;
//
//				triPointOne.x = triangleSelected[0].x;
//				triPointOne.y = triangleSelected[0].y;
//
//				triPointTwo.x = triangleSelected[1].x;
//				triPointTwo.y = triangleSelected[1].y;
//
//				triPointThree.x = triangleSelected[2].x;
//				triPointThree.y = triangleSelected[2].y;
//
//				if (pointPolygonTest(Mat(pentagons[a]), triPointOne, true) > 0 && pointPolygonTest(Mat(pentagons[a]), triPointTwo, true) > 0 && pointPolygonTest(Mat(pentagons[a]), triPointThree, true) > 0){
//					allowedPentagonsContourPosition.push_back(pentagonContourPositions[a]);
//					allowedTriangles->push_back(triangleContourPositions[i]);
//					found = true;
//					break;
//				}
//			}
//
//			if (found)
//				break;
//		}
//	}
//
//	return  allowedPentagonsContourPosition;
//}
//
//static vector<int> CheckForAllowedRectangles(vector<vector<cv::Point>> rectangles, vector<vector<cv::Point>> triangles, vector<int> rectanglesContourPositions, vector<int> triangleContourPositions, vector<int> *allowedTriangles)
//{
//	int rectPos;
//	vector<int> allowedRectanglesContourPositions;
//	bool found = false;
//
//	if (rectangles.size() == rectanglesContourPositions.size()){
//		for (size_t i = 0; i < triangles.size(); i++)
//		{
//			std::vector<cv::Point> triSelected = triangles[i];
//
//			for (size_t a = 0; a < rectangles.size(); a++)
//			{
//				if (triSelected.size() != 3)
//					break;
//
//				Point2f triPointOne;
//				Point2f triPointTwo;
//				Point2f triPointThree;
//
//				triPointOne.x = triSelected[0].x;
//				triPointOne.y = triSelected[0].y;
//
//				triPointTwo.x = triSelected[1].x;
//				triPointTwo.y = triSelected[1].y;
//
//				triPointThree.x = triSelected[2].x;
//				triPointThree.y = triSelected[2].y;
//
//				if (pointPolygonTest(Mat(rectangles[a]), triPointOne, true) > 0 && pointPolygonTest(Mat(rectangles[a]), triPointTwo, true) > 0 && pointPolygonTest(Mat(rectangles[a]), triPointThree, true) > 0){
//					allowedRectanglesContourPositions.push_back(rectanglesContourPositions[a]);
//					//allowedTriangles->push_back(triangleContourPositions[i]);
//					allowedTriangles->push_back(i);
//					found = true;
//					break;
//				}
//			}
//
//			if (found)
//				break;
//		}
//	}
//
//	return allowedRectanglesContourPositions;
//}
//
//static vector<Point> GetFrontOfTriangle(vector<Point> tri)
//{
//	Point p1 = tri[0];
//	Point p2 = tri[1];
//	Point p3 = tri[2];
//
//
//	Point lP1P2 =p1 - p2;
//	Point lP2P3 = p2 - p3;
//	Point lP3P1 = p3 - p1;
//
//	double l1 = sqrt((lP1P2.x*lP1P2.x) + (lP1P2.y*lP1P2.y));
//	double l2 = sqrt((lP2P3.x*lP2P3.x) + (lP2P3.y*lP2P3.y));
//	double l3 = sqrt((lP3P1.x*lP3P1.x) + (lP3P1.y*lP3P1.y));
//
//	double shortes = l1;
//	Point toReturn = p3;
//	Point dir = Point(lP1P2.x/ 2,lP1P2.y/2) + p2;
//
//	if (l2 < shortes)
//	{
//		shortes = l2;
//		toReturn = p1;
//		dir = Point(lP2P3.x / 2, lP2P3.y / 2) + p3;
//	}
//	if (l3 < shortes)
//	{
//		shortes = l3;
//		toReturn = p2;
//		dir = Point(lP3P1.x / 2, lP3P1.y / 2) + p1;
//	}
//
//	Point direction = toReturn-dir;
//	vector<Point> points;
//	points.push_back(direction);
//	points.push_back(toReturn);
//	return points;
//
//}
//
//static void setLabel2(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
//{
//	int fontface = cv::FONT_HERSHEY_SIMPLEX;
//	double scale = 0.4;
//	int thickness = 1;
//	int baseline = 0;
//
//	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
//	cv::Rect r = cv::boundingRect(contour);
//
//	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
//	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(0, 0, 0), CV_FILLED);
//	cv::putText(im, label, pt, fontface, scale, CV_RGB(255, 255, 255), thickness, 8);
//}
//
///**
//* @function thresh_callback
//*/
//static void thresh_callbackdetect2(int, void*)
//{
//	Mat canny_output;
//	vector<vector<Point> > contours;
//	vector<Vec4i> hierarchy;
//
//	/// Detect edges using canny
//	Canny(src_graydetect2, canny_output, threshdetect2, threshdetect2 * 2, 3);
//	/// Find contours
//	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//	/// Draw contours
//	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255));
//		drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
//	}
//
//	/// Show in a window
//	namedWindow("Contours", WINDOW_AUTOSIZE);
//	imshow("Contours", drawing);
//}
//
//static void calcPosition()
//{
//}
//
///**
//* @function thresh_callback
//*/
//static void thresh_callbackdetect3(int, void*)
//{
//	//Drawing and contours
//	Mat canny_output;
//	vector<vector<Point> > contours;
//	vector<Vec4i> hierarchy;
//
//	// Finding rects,tris and pentagons
//	std::vector<cv::Point> approx;
//
//	vector<vector<cv::Point>> rectangles;
//	vector<vector<cv::Point>> pentagons;
//	vector<int> rectanglesContourPositions;
//	vector<int> pentagonsContourPositions;
//	vector<vector<cv::Point>> triangles;
//	vector<int> trianglePositions;
//
//	//Found correct elements
//	vector<int> allowedRectanglesContourPositions;
//	vector<int> allowedPentagonsContourPosition;
//	vector<int> allowedTriangles;
//	vector<int> allowedTrianglesPoly;
//
//	//dilate(src_graydetect2, src_graydetect2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//	/// Detect edges using canny
//	Canny(src_graydetect2, canny_output, threshdetect2, threshdetect2 * 2, 3);
//
//	//thresholding the grayscale image to get better results
//	threshold(canny_output, canny_output, 128, 255, CV_THRESH_BINARY);
//
//	/// Find contours
//	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
//
//	/// Draw contours
//	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
//
//	/*
//	*	Starting detection process
//	*/
//
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		// Approximate contour with accuracy proportional
//		// to the contour perimeter
//		// original elipse value 0.02
//		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.1, true);
//
//		// Skip small or non-convex objects
//		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
//			continue;
//
//		// Rectangles
//		if (approx.size() == 4)
//		{
//			rectangles.push_back(contours[i]);
//			rectanglesContourPositions.push_back(i);
//		}
//
//		if (approx.size() == 3)
//		{
//			triangles.push_back(approx);
//			trianglePositions.push_back(i);
//		}
//
//		if (approx.size() == 5)
//		{
//			pentagons.push_back(contours[i]);
//			pentagonsContourPositions.push_back(i);
//		}
//	}
//
//
//
//	allowedRectanglesContourPositions = CheckForAllowedRectangles(rectangles, triangles, rectanglesContourPositions, trianglePositions, &allowedTriangles);
//	allowedPentagonsContourPosition = CheckForAllowedPentagons(pentagons, triangles, pentagonsContourPositions, trianglePositions, &allowedTrianglesPoly);
//
//
//	if (allowedTriangles.size() > 0){
//		Point dir;
//		Point PosTop;
//		vector<Point> pointsTriRect = GetFrontOfTriangle(triangles[allowedTriangles[0]]);
//		dir = pointsTriRect[0];
//		PosTop = pointsTriRect[1];
//
//
//
//		circle(srcdetect2, PosTop, 10.0, Scalar(0, 0, 255), 1, 8);
//		circle(srcdetect2, PosTop-dir, 10.0, Scalar(0, 0, 255), 1, 8);
//
//	}
//
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		for (size_t y = 0; y < allowedRectanglesContourPositions.size(); y++)
//		{
//			if (allowedRectanglesContourPositions[y] == i)
//			{
//				//Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 0));
//				Scalar color = Scalar(255, 0, 0);
//				drawContours(srcdetect2, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
//
//				cv::Moments mom;
//
//				mom = cv::moments(cv::Mat(contours[i]));
//
//				cv::Point point = Point(mom.m10 / mom.m00, mom.m01 / mom.m00);
//				// draw mass center
//				cv::circle(srcdetect2, point
//					// position of mass center converted to integer
//					,
//					2, cv::Scalar(255, 255, 255), 2);// draw white dot
//
//				string xAsString = static_cast<ostringstream*>(&(ostringstream() << point.x))->str();
//				string yAsString = static_cast<ostringstream*>(&(ostringstream() << point.y))->str();
//
//				std::string zeichenkette = "x: " + xAsString + " y: " + yAsString;
//
//				setLabel2(srcdetect2, zeichenkette, contours[i]);
//			}
//		}
//
//		for (size_t y = 0; y < allowedPentagonsContourPosition.size(); y++)
//		{
//			if (allowedPentagonsContourPosition[y] == i)
//			{
//				Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255));
//				drawContours(canny_output, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
//
//				cv::Moments mom;
//
//				mom = cv::moments(cv::Mat(contours[i]));
//
//				cv::Point point = Point(mom.m10 / mom.m00, mom.m01 / mom.m00);
//				// draw mass center
//				cv::circle(canny_output, point
//					// position of mass center converted to integer
//					,
//					2, cv::Scalar(255, 255, 255), 2);// draw white dot
//
//				string xAsString = static_cast<ostringstream*>(&(ostringstream() << point.x))->str();
//				string yAsString = static_cast<ostringstream*>(&(ostringstream() << point.y))->str();
//
//				std::string zeichenkette = "x: " + xAsString + " y: " + yAsString;
//
//				setLabel2(canny_output, zeichenkette, contours[i]);
//			}
//
//			/*for (size_t y = 0; y < allowedTriangles.size(); y++)
//			{
//				if (allowedTriangles[y] == i)
//				{
//					Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255));
//					drawContours(canny_output, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
//				}
//			}
//
//			for (size_t y = 0; y < allowedTrianglesPoly.size(); y++)
//			{
//				if (allowedTrianglesPoly[y] == i)
//				{
//					Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255));
//					drawContours(canny_output, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
//				}
//			}*/
//		}
//	}
//
//	/// Show in a window
//	namedWindow("Contours", WINDOW_AUTOSIZE);
//	imshow("Contours", srcdetect2);
//}
//
///**
//* Helper function to find a cosine of angle between vectors
//* from pt0->pt1 and pt0->pt2
//*/
//static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
//{
//	double dx1 = pt1.x - pt0.x;
//	double dy1 = pt1.y - pt0.y;
//	double dx2 = pt2.x - pt0.x;
//	double dy2 = pt2.y - pt0.y;
//	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
//
//
//
//}
//
//class ThomasTest{
//public:
//	enum Farbe { ROT = 0, GRUEN = 1, BLAU = 2 };
//
//	ThomasTest() {
//		threshdetect2 = 100;
//		max_threshdetect2 = 250;
//		rngdetect2(12345);
//	}
//
//	void DetectWihoutServices(){
//		VideoCapture cap(0); //capture the video from web cam
//
//		if (!cap.isOpened())  // if not success, exit program
//		{
//			return;
//		}
//
//		namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
//
//		int iLowH = 0;
//		int iHighH = 179;
//
//		int iLowS = 0;
//		int iHighS = 255;
//
//		int iLowV = 0;
//		int iHighV = 255;
//
//		//Create trackbars in "Control" window
//		cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
//		cvCreateTrackbar("HighH", "Control", &iHighH, 179);
//
//		cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
//		cvCreateTrackbar("HighS", "Control", &iHighS, 255);
//
//		cvCreateTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
//		cvCreateTrackbar("HighV", "Control", &iHighV, 255);
//
//		while (true)
//		{
//			Mat imgOriginal;
//
//			bool bSuccess = cap.read(imgOriginal); // read a new frame from video
//
//			if (!bSuccess) //if not success, break loop
//			{
//				break;
//			}
//
//			Mat imgHSV;
//
//			cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
//
//			Mat imgThresholded;
//
//			inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
//
//			//morphological opening (removes small objects from the foreground)
//			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//			//morphological closing (removes small holes from the foreground)
//			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//			imshow("Thresholded Image", imgThresholded); //show the thresholded image
//			imshow("Original", imgOriginal); //show the original image
//
//			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//			{
//				break;
//			}
//		}
//	}
//
//	void DetectConture()
//	{
//		CvCapture* capture = cvCaptureFromCAM(0);
//		IplImage* img;
//		CvMemStorage *storage;
//		IplImage* imgGrayScale;
//
//		while (true)
//		{
//			img = cvQueryFrame(capture);
//
//			//show the original image
//			cvNamedWindow("Raw");
//			cvShowImage("Raw", img);
//
//			//converting the original image into grayscale
//			imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1);
//			cvCvtColor(img, imgGrayScale, CV_BGR2GRAY);
//
//			//thresholding the grayscale image to get better results
//			cvThreshold(imgGrayScale, imgGrayScale, 128, 255, CV_THRESH_BINARY);
//
//			CvSeq* contours;  //hold the pointer to a contour in the memory block
//			CvSeq* result;   //hold sequence of points of a contour
//			storage = cvCreateMemStorage(0); //storage area for all contours
//
//			//finding all contours in the image
//			cvFindContours(imgGrayScale, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
//
//			//iterating through each contour
//			while (contours)
//			{
//				//obtain a sequence of points of contour, pointed by the variable 'contour'
//				result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
//
//				//if there are 3  vertices  in the contour(It should be a triangle)
//				if (result->total == 3)
//				{
//					//iterating through each point
//					CvPoint *pt[3];
//					for (int i = 0; i < 3; i++){
//						pt[i] = (CvPoint*)cvGetSeqElem(result, i);
//					}
//
//					//drawing lines around the triangle
//					cvLine(img, *pt[0], *pt[1], cvScalar(255, 0, 0), 4);
//					cvLine(img, *pt[1], *pt[2], cvScalar(255, 0, 0), 4);
//					cvLine(img, *pt[2], *pt[0], cvScalar(255, 0, 0), 4);
//				}
//
//				//if there are 4 vertices in the contour(It should be a quadrilateral)
//				else if (result->total == 4)
//				{
//					//iterating through each point
//					CvPoint *pt[4];
//					for (int i = 0; i < 4; i++){
//						pt[i] = (CvPoint*)cvGetSeqElem(result, i);
//					}
//
//					//drawing lines around the quadrilateral
//					cvLine(img, *pt[0], *pt[1], cvScalar(0, 255, 0), 4);
//					cvLine(img, *pt[1], *pt[2], cvScalar(0, 255, 0), 4);
//					cvLine(img, *pt[2], *pt[3], cvScalar(0, 255, 0), 4);
//					cvLine(img, *pt[3], *pt[0], cvScalar(0, 255, 0), 4);
//				}
//
//				//if there are 7  vertices  in the contour(It should be a heptagon)
//				else if (result->total == 7)
//				{
//					//iterating through each point
//					CvPoint *pt[7];
//					for (int i = 0; i < 7; i++){
//						pt[i] = (CvPoint*)cvGetSeqElem(result, i);
//					}
//
//					//drawing lines around the heptagon
//
//					cvLine(img, *pt[0], *pt[1], cvScalar(0, 0, 255), 4);
//					cvLine(img, *pt[1], *pt[2], cvScalar(0, 0, 255), 4);
//					cvLine(img, *pt[2], *pt[3], cvScalar(0, 0, 255), 4);
//					cvLine(img, *pt[3], *pt[4], cvScalar(0, 0, 255), 4);
//					cvLine(img, *pt[4], *pt[5], cvScalar(0, 0, 255), 4);
//					cvLine(img, *pt[5], *pt[6], cvScalar(0, 0, 255), 4);
//					cvLine(img, *pt[6], *pt[0], cvScalar(0, 0, 255), 4);
//				}
//
//				//obtain the next contour
//				contours = contours->h_next;
//			}
//
//			Mat img2 = img;
//			//morphological opening (removes small objects from the foreground)
//			erode(img2, img2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//			dilate(img2, img2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//			//morphological closing (removes small holes from the foreground)
//			dilate(img2, img2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//			erode(img2, img2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//			//show the image in which identified shapes are marked
//			cvNamedWindow("Tracked");
//			//cvShowImage("Tracked", img2);
//			imshow("Tracked", img2);
//
//			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//			{
//				break;
//			}
//		}
//
//		//cleaning up
//		cvDestroyAllWindows();
//		cvReleaseMemStorage(&storage);
//		cvReleaseImage(&img);
//		cvReleaseImage(&imgGrayScale);
//	}
//
//	void DetectConture2() {
//		VideoCapture capture = VideoCapture();
//
//		capture.open(0);
//
//		////camera settings
//		capture.set(CV_CAP_PROP_FPS, 30);
//		////Possible resolutions : 640x480; 440x330
//		/*	capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
//			capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);*/
//
//		cout << "FPS: " << capture.get(CV_CAP_PROP_FPS) << std::endl;
//		cout << "Resolution: " << capture.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
//
//		/// Create Window
//		const char* source_window = "Source";
//		namedWindow(source_window, WINDOW_AUTOSIZE);
//
//		createTrackbar(" Canny thresh:", "Source", &threshdetect2, max_threshdetect2, thresh_callbackdetect3);
//
//		while (true)
//		{
//			capture >> srcdetect2;
//
//			////Grayscale matrix
//			//cv::Mat grayscaleMat(srcdetect2.size(), CV_8U);
//
//			////Convert BGR to Gray
//			//cv::cvtColor(srcdetect2, grayscaleMat, CV_BGR2GRAY);
//
//			////Binary image
//			//cv::Mat binaryMat(grayscaleMat.size(), grayscaleMat.type());
//
//			////Apply thresholding
//			//cv::threshold(grayscaleMat, binaryMat, 100, 255, cv::THRESH_BINARY);
//
//			//src_graydetect2 = binaryMat;
//
//			///// Convert image to gray and blur it
//			//cvtColor(srcdetect2, src_graydetect2, CV_BGR2GRAY);
//
//			cvtColor(srcdetect2, src_graydetect2, COLOR_BGR2GRAY);
//			blur(src_graydetect2, src_graydetect2, Size(3, 3));
//
//			imshow(source_window, srcdetect2);
//
//			//createTrackbar(" Canny thresh:", "Source", &threshdetect2, max_threshdetect2, thresh_callbackdetect2);
//			//thresh_callbackdetect2(0, 0);
//
//			thresh_callbackdetect3(0, 0);
//
//			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//			{
//				break;
//			}
//		}
//
//		//cleaning up
//		cvDestroyAllWindows();
//	}
//
//	void DetectConture3() {
//		CvCapture* capture = cvCaptureFromCAM(0);
//
//		while (true)
//		{
//			Mat src = cvQueryFrame(capture);
//
//			// Convert to grayscale
//			cv::Mat gray;
//			cv::cvtColor(src, gray, CV_BGR2GRAY);
//
//			// Use Canny instead of threshold to catch squares with gradient shading
//			cv::Mat bw;
//			cv::Canny(gray, bw, 0, 50, 5);
//
//			// Find contours
//			std::vector<std::vector<cv::Point> > contours;
//			cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//
//			std::vector<cv::Point> approx;
//			cv::Mat dst = src.clone();
//
//			for (int i = 0; i < contours.size(); i++)
//			{
//				// Approximate contour with accuracy proportional
//				// to the contour perimeter
//				cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
//
//				// Skip small or non-convex objects
//				if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
//					continue;
//
//				if (approx.size() == 3)
//				{
//					setLabel(dst, "TRI", contours[i]);    // Triangles
//				}
//				else if (approx.size() >= 4 && approx.size() <= 6)
//				{
//					// Number of vertices of polygonal curve
//					int vtc = approx.size();
//
//					// Get the cosines of all corners
//					std::vector<double> cos;
//					for (int j = 2; j < vtc + 1; j++)
//						cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));
//
//					// Sort ascending the cosine values
//					std::sort(cos.begin(), cos.end());
//
//					// Get the lowest and the highest cosine
//					double mincos = cos.front();
//					double maxcos = cos.back();
//
//					// Use the degrees obtained above and the number of vertices
//					// to determine the shape of the contour
//					if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
//						setLabel(dst, "RECT", contours[i]);
//					else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
//						setLabel(dst, "PENTA", contours[i]);
//					else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
//						setLabel(dst, "HEXA", contours[i]);
//				}
//				else
//				{
//					// Detect and label circles
//					double area = cv::contourArea(contours[i]);
//					cv::Rect r = cv::boundingRect(contours[i]);
//					int radius = r.width / 2;
//
//					if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
//						std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
//						setLabel(dst, "CIR", contours[i]);
//				}
//			}
//
//			cv::imshow("src", src);
//			cv::imshow("dst", dst);
//			//cv::waitKey(0);
//
//			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//			{
//				break;
//			}
//		}
//
//		//cleaning up
//		cvDestroyAllWindows();
//	}
//
//	/**
//	* Helper function to display text in the center of a contour
//	*/
//	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
//	{
//		int fontface = cv::FONT_HERSHEY_SIMPLEX;
//		double scale = 0.4;
//		int thickness = 1;
//		int baseline = 0;
//
//		cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
//		cv::Rect r = cv::boundingRect(contour);
//
//		cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
//		cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
//		cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
//	}
//
//	void Test4()
//	{
//		// create a RGB colour image (set it to a black background)
//
//		Mat img = Mat::zeros(400, 400, CV_8UC3);
//
//		// define a polygon (as a vector of points)
//
//		vector<Point> contour;
//		contour.push_back(Point(50, 50));
//		contour.push_back(Point(300, 50));
//		contour.push_back(Point(350, 200));
//		contour.push_back(Point(300, 150));
//		contour.push_back(Point(150, 350));
//		contour.push_back(Point(100, 100));
//
//		// create a pointer to the data as an array of points (via a conversion to
//		// a Mat() object)
//
//		const cv::Point *pts = (const cv::Point*) Mat(contour).data;
//		int npts = Mat(contour).rows;
//
//		std::cout << "Number of polygon vertices: " << npts << std::endl;
//
//		// draw the polygon
//
//		polylines(img, &pts, &npts, 1,
//			true, 			// draw closed contour (i.e. joint end to start)
//			Scalar(0, 255, 0),// colour RGB ordering (here = green)
//			3, 		        // line thickness
//			CV_AA, 0);
//
//		// do point in polygon test (by conversion/cast to a Mat() object)
//		// define and test point one (draw it in red)
//
//		Point2f test_pt;
//		test_pt.x = 150;
//		test_pt.y = 75;
//
//		rectangle(img, test_pt, test_pt, Scalar(0, 0, 255), 3, 8, 0); // RED point
//
//		if (pointPolygonTest(Mat(contour), test_pt, true) > 0){
//			std::cout << "RED {" << test_pt.x << "," << test_pt.y
//				<< "} is in the polygon (dist. "
//				<< pointPolygonTest(Mat(contour), test_pt, 1) << ")"
//				<< std::endl;
//		}
//
//		// define and test point two (draw it in blue)
//
//		test_pt.x = 50;
//		test_pt.y = 350;
//
//		rectangle(img, test_pt, test_pt, Scalar(255, 0, 0), 3, 8, 0); // BLUE point
//
//		if (pointPolygonTest(Mat(contour), test_pt, true) < 0){
//			std::cout << "BLUE {" << test_pt.x << "," << test_pt.y
//				<< "} is NOT in the polygon (dist. "
//				<< pointPolygonTest(Mat(contour), test_pt, 1) << ")"
//				<< std::endl;
//		}
//
//		// pointPolygonTest :-
//		// "The function determines whether the point is inside a contour, outside,
//		// or lies on an edge (or coincides with a vertex). It returns positive
//		// (inside), negative (outside) or zero (on an edge) value, correspondingly.
//		// When measureDist=false , the return value is +1, -1 and 0, respectively.
//		// Otherwise, the return value it is a signed distance between the point
//		// and the nearest contour edge." - OpenCV Manual version 2.1
//
//		// create an image and display the image
//
//		namedWindow("Polygon Test", 0);
//		imshow("Polygon Test", img);
//		waitKey(0);
//	}
//
//	void Test5(){
//		CvCapture* capture = cvCaptureFromCAM(0);
//
//		Mat src;
//
//		while (true)
//		{
//			src = cvQueryFrame(capture);
//
//			// Convert to grayscale
//			cv::Mat gray;
//			cv::cvtColor(src, gray, CV_BGR2GRAY);
//
//			// Use Canny instead of threshold to catch squares with gradient shading
//			cv::Mat bw;
//			cv::Canny(gray, bw, 0, 50, 5);
//
//			// Find contours
//			std::vector<std::vector<cv::Point> > contours;
//			cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
//
//			std::vector<cv::Point> approx;
//			cv::Mat dst = src.clone();
//
//			for (int i = 0; i < contours.size(); i++)
//			{
//				// Approximate contour with accuracy proportional
//				// to the contour perimeter
//				cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
//
//				// Skip small or non-convex objects
//				if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
//					continue;
//
//				if (approx.size() == 3)
//				{
//					setLabel(dst, "TRI", contours[i]);    // Triangles
//				}
//				else if (approx.size() >= 4 && approx.size() <= 6)
//				{
//					// Number of vertices of polygonal curve
//					int vtc = approx.size();
//
//					// Get the cosines of all corners
//					std::vector<double> cos;
//					for (int j = 2; j < vtc + 1; j++)
//						cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));
//
//					// Sort ascending the cosine values
//					std::sort(cos.begin(), cos.end());
//
//					// Get the lowest and the highest cosine
//					double mincos = cos.front();
//					double maxcos = cos.back();
//
//					// Use the degrees obtained above and the number of vertices
//					// to determine the shape of the contour
//					if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
//						setLabel(dst, "RECT", contours[i]);
//					else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
//						setLabel(dst, "PENTA", contours[i]);
//					else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
//						setLabel(dst, "HEXA", contours[i]);
//				}
//				else
//				{
//					// Detect and label circles
//					double area = cv::contourArea(contours[i]);
//					cv::Rect r = cv::boundingRect(contours[i]);
//					int radius = r.width / 2;
//
//					if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
//						std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
//						setLabel(dst, "CIR", contours[i]);
//				}
//			}
//
//			cv::imshow("src", src);
//			cv::imshow("dst", dst);
//
//			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//			{
//				break;
//			}
//		}
//
//		//cleaning up
//		cvDestroyAllWindows();
//	}
//
//	void Test6(){
//
//
//		VideoCapture capture = VideoCapture();
//
//		capture.open(0);
//
//		////camera settings
//		capture.set(CV_CAP_PROP_FPS, 30);
//		////Possible resolutions : 640x480; 440x330
//		//capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
//		//capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
//
//		cout << "FPS: " << capture.get(CV_CAP_PROP_FPS) << std::endl;
//		cout << "Resolution: " << capture.get(CV_CAP_PROP_FRAME_WIDTH) << "x" << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
//
//		//Mat img2 = imread("C:/Users/GIGI/Downloads/img/DetectingContours.jpg", CV_LOAD_IMAGE_COLOR);
//		Mat imgGrayScale2;
//
//		//VideoCapture capture = VideoCapture();
//		IplImage* img;
//		IplImage* imgGrayScale;
//		CvMemStorage *storage;
//
//
//		//Drawing and contours
//		Mat canny_output;
//		vector<vector<Point> > contours;
//		vector<Vec4i> hierarchy;
//
//		// Finding rects,tris and pentagons
//		std::vector<cv::Point> approx;
//
//		Mat img2;
//
//
//		while (true)
//		{
//			capture >> img2;
//
//			//show the original image
//			cvNamedWindow("Original");
//			imshow("Original", img2);
//
//			//smooth the original image using Gaussian kernel to remove noise
//			//cvSmooth(img, img, CV_GAUSSIAN, 3, 3);
//			GaussianBlur(img2, img2, Size(1, 1), 3, 3);
//
//
//
//
//			//////converting the original image into grayscale
//			//imgGrayScale2 = cvCreateImage(cvGetSize(img), 8, 1);
//			//cvCvtColor(img, imgGrayScale, CV_BGR2GRAY);
//
//			cvtColor(img2, imgGrayScale2, CV_RGB2GRAY);
//
//			cvNamedWindow("GrayScale Image");
//			imshow("GrayScale Image", imgGrayScale2);
//
//
//			//thresholding the grayscale image to get better results
//			threshold(imgGrayScale2, imgGrayScale2, 100, 255, CV_THRESH_BINARY_INV);
//
//			cvNamedWindow("Thresholded Image");
//			imshow("Thresholded Image", imgGrayScale2);
//
//
//			//imgGrayScale = cvCloneImage(&(IplImage)imgGrayScale2);
//
//
//			//CvSeq* contour;  //hold the pointer to a contour
//			//CvSeq* result;   //hold sequence of points of a contour
//			//storage = cvCreateMemStorage(0); //storage area for all contours
//
//			////finding all contours in the image
//			//cvFindContours(imgGrayScale, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
//
//			////iterating through each contour
//			//while (contour)
//			//{
//			//	//obtain a sequence of points of the countour, pointed by the variable 'countour'
//			//	result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);
//			//	approxPolyDP(contour)
//
//			//	//if there are 3 vertices  in the contour and the area of the triangle is more than 100 pixels
//			//	if (result->total == 3 && fabs(cvContourArea(result, CV_WHOLE_SEQ))>100)
//			//	{
//			//		//iterating through each point
//			//		CvPoint *pt[3];
//			//		for (int i = 0; i<3; i++){
//			//			pt[i] = (CvPoint*)cvGetSeqElem(result, i);
//			//		}
//
//			//		//drawing lines around the triangle
//			//		line(img2, *pt[0], *pt[1], cvScalar(255, 0, 0), 4);
//			//		line(img2, *pt[1], *pt[2], cvScalar(255, 0, 0), 4);
//			//		line(img2, *pt[2], *pt[0], cvScalar(255, 0, 0), 4);
//
//			//	}
//
//			//	//obtain the next contour
//			//	contour = contour->h_next;
//			//}
//
//
//			/// Find contours
//			findContours(imgGrayScale2, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//			for (size_t i = 0; i < contours.size(); i++)
//			{
//				// Approximate contour with accuracy proportional
//				// to the contour perimeter
//				cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
//
//				// Skip small or non-convex objects
//				if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
//					continue;
//
//
//
//				if (approx.size() == 3 && std::fabs(cv::contourArea(contours[i])) >10)
//				{
//					drawContours(img2, contours, (int)i, Scalar(255, 0, 0), 2, 8, hierarchy, 0, Point());
//				}
//
//
//			}
//
//
//
//
//			//show the image in which identified shapes are marked   
//			cvNamedWindow("Tracked");
//			imshow("Tracked", img2);
//
//			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//			{
//				break;
//			}
//
//		}
//
//		cvWaitKey(0); //wait for a key press
//
//		//cleaning up
//		cvDestroyAllWindows();
//		cvReleaseMemStorage(&storage);
//		cvReleaseImage(&img);
//		cvReleaseImage(&imgGrayScale);
//
//
//	}
//
//	void DetectObject7()
//	{
//		VideoCapture cap;
//		bool update_bg_model = true;
//
//		cap.open(0);
//
//		Mat tmp_frame, bgmask, out_frame;
//
//		cap >> tmp_frame;
//
//
//		namedWindow("video", 1);
//		namedWindow("segmented", 1);
//
//		BackgroundSubtractorMOG bgsubtractor;
//		bgsubtractor.set("noiseSigma", 10);
//
//		for (;;)
//		{
//			cap >> tmp_frame;
//			if (!tmp_frame.data)
//				break;
//			bgsubtractor(tmp_frame, bgmask, update_bg_model ? -1 : 0);
//			//refineSegments(tmp_frame, bgmask, out_frame);
//			imshow("video", tmp_frame);
//			imshow("segmented", out_frame);
//			int keycode = waitKey(30);
//			if (keycode == 27)
//				break;
//			if (keycode == ' ')
//			{
//				update_bg_model = !update_bg_model;
//				printf("Learn background is in state = %d\n", update_bg_model);
//			}
//		}
//
//
//	}
//
//	void MovingDetection(){
//
//
//		const int nmixtures = 3;
//		const bool bShadowDetection = true;
//		const int history = 5;
//
//		cv::Mat frame;
//		cv::Mat back;
//		cv::Mat fore;
//		cv::VideoCapture cap(0);
//
//		cv::BackgroundSubtractorMOG2 bg(history, nmixtures, bShadowDetection);
//
//
//		std::vector<std::vector<cv::Point> > contours;
//
//		cv::namedWindow("Frame");
//		cv::namedWindow("Background");
//
//		int erosion_size = 2; // adjust with you application
//		Mat erode_element = getStructuringElement(MORPH_ELLIPSE,
//			Size(2 * erosion_size + 1, 2 * erosion_size + 1),
//			Point(erosion_size, erosion_size));
//
//
//		for (;;)
//		{
//			cap >> frame;
//			bg.operator ()(frame, fore);
//			bg.getBackgroundImage(back);
//			cv::erode(fore, fore, erode_element);
//			cv::dilate(fore, fore, erode_element);
//			cv::findContours(fore, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//			//cv::findContours(fore, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
//
//			cv::drawContours(frame, contours, -1, cv::Scalar(0, 0, 255), 2);
//			cv::imshow("Frame", frame);
//			cv::imshow("Background", back);
//			if (cv::waitKey(30) >= 0) break;
//		}
//
//
//	}
//
//	int startMotionTracking(){
//
//		//some boolean variables for added functionality
//		bool objectDetected = true;
//		//these two can be toggled by pressing 'd' or 't'
//		bool debugMode = true;
//		bool trackingEnabled = true;
//		//pause and resume code
//		bool pause = false;
//		//set up the matrices that we will need
//		//the two frames we will be comparing
//		Mat frame1, frame2;
//		//their grayscale images (needed for absdiff() function)
//		Mat grayImage1, grayImage2;
//		//resulting difference image
//		Mat differenceImage;
//		//thresholded difference image (for use in findContours() function)
//		Mat thresholdImage;
//		//video capture object.
//		VideoCapture capture;
//
//		while (1){
//
//			//we can loop the video by re-opening the capture every time the video reaches its last frame
//
//			capture.open(0);
//
//			if (!capture.isOpened()){
//				cout << "ERROR ACQUIRING VIDEO FEED\n";
//				getchar();
//				return -1;
//			}
//
//			//check if the video has reach its last frame.
//			//we add '-1' because we are reading two frames from the video at a time.
//			//if this is not included, we get a memory error!
//			//while (capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT) - 1){
//			while (1){
//				//read first frame
//				capture.read(frame1);
//				//convert frame1 to gray scale for frame differencing
//				cv::cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
//				//copy second frame
//				capture.read(frame2);
//				//convert frame2 to gray scale for frame differencing
//				cv::cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);
//				//perform frame differencing with the sequential images. This will output an "intensity image"
//				//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
//				cv::absdiff(grayImage1, grayImage2, differenceImage);
//				//threshold intensity image at a given sensitivity value
//				cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
//				if (debugMode == true){
//					//show the difference image and threshold image
//					cv::imshow("Difference Image", differenceImage);
//					cv::imshow("Threshold Image", thresholdImage);
//				}
//				else{
//					//if not in debug mode, destroy the windows so we don't see them anymore
//					cv::destroyWindow("Difference Image");
//					cv::destroyWindow("Threshold Image");
//				}
//				//blur the image to get rid of the noise. This will output an intensity image
//				cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
//				//threshold again to obtain binary image from blur output
//				cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
//				if (debugMode == true){
//					//show the threshold image after it's been "blurred"
//
//					imshow("Final Threshold Image", thresholdImage);
//
//				}
//				else {
//					//if not in debug mode, destroy the windows so we don't see them anymore
//					cv::destroyWindow("Final Threshold Image");
//				}
//
//				//if tracking enabled, search for contours in our thresholded image
//				if (trackingEnabled){
//
//					//searchForMovement(thresholdImage, frame1);
//				}
//
//				//show our captured frame
//				imshow("Frame1", frame1);
//				//check to see if a button has been pressed.
//				//this 10ms delay is necessary for proper operation of this program
//				//if removed, frames will not have enough time to referesh and a blank 
//				//image will appear.
//				switch (waitKey(10)){
//
//				case 27: //'esc' key has been pressed, exit program.
//					return 0;
//				case 116: //'t' has been pressed. this will toggle tracking
//					trackingEnabled = !trackingEnabled;
//					if (trackingEnabled == false) cout << "Tracking disabled." << endl;
//					else cout << "Tracking enabled." << endl;
//					break;
//				case 100: //'d' has been pressed. this will debug mode
//					debugMode = !debugMode;
//					if (debugMode == false) cout << "Debug mode disabled." << endl;
//					else cout << "Debug mode enabled." << endl;
//					break;
//				case 112: //'p' has been pressed. this will pause/resume the code.
//					pause = !pause;
//					if (pause == true){
//						cout << "Code paused, press 'p' again to resume" << endl;
//						while (pause == true){
//							//stay in this loop until 
//							switch (waitKey()){
//								//a switch statement inside a switch statement? Mind blown.
//							case 112:
//								//change pause back to false
//								pause = false;
//								cout << "Code Resumed" << endl;
//								break;
//							}
//						}
//					}
//
//
//
//				}
//			}
//			//release the capture before re-opening and looping again.
//			capture.release();
//		}
//
//		return 0;
//	}
//
//
//
//	void Test11(){
//
//		//video capture object.
//		VideoCapture capture;
//
//		capture.open(0);
//
//
//
//		/// Load image and template
//		templ = imread("C:/Users/GIGI/Downloads/img/two_2.jpg", 1);
//
//		/// Create windows
//		namedWindow(image_window, WINDOW_AUTOSIZE);
//		namedWindow(result_window, WINDOW_AUTOSIZE);
//		namedWindow(search_window, WINDOW_AUTOSIZE);
//
//		/// Create Trackbar
//		const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
//		createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
//
//		for (;;)
//		{
//			//check if the video has reach its last frame.
//			//we add '-1' because we are reading two frames from the video at a time.
//			//if this is not included, we get a memory error!
//			//read first frame
//			capture.read(img);
//			MatchingMethod(0, 0);
//
//			if (cv::waitKey(30) >= 0) break;
//		}
//
//		waitKey(0);
//
//	}
//
//};
//
//
//
//
//
//
//
//
//
//
//
//
