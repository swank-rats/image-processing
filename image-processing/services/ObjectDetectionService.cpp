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
Shot shots[] = {
	//top to bottom, right to left
	Shot(Point2i(350, 50), Point2i(50, 350)),
	//bottom to top, right to left
	Shot(Point2i(350, 350), Point2i(50, 50)),

	//top to bottom, left to right
	Shot(Point2i(15, 50), Point2i(380, 240)),
	//bottom to top, left to right
	Shot(Point2i(50, 380), Point2i(370, 30)),

	//top to bottom
	Shot(Point2i(50, 50), Point2i(50, 350)),
	//bottom to top
	Shot(Point2i(50, 350), Point2i(50, 50)),

	//left to right
	Shot(Point2i(50, 50), Point2i(400, 50)),
	//right to left
	Shot(Point2i(400, 50), Point2i(50, 50))
};

ObjectDetectionService::ObjectDetectionService() { }

ObjectDetectionService::~ObjectDetectionService() { }

void ObjectDetectionService::DetectObject(Mat src, int iLowH, int iLowS, int iLowV, int iHighH, int iHighS, int iHighV) {
	Mat imgOriginal(src);
	Mat imgHSV;

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	Mat imgThresholded;

	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));

	//morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));

	imshow("input", imgThresholded);
	waitKey();

	///// Convert image to gray and blur it
	//cvtColor(imgThresholded, src_gray, COLOR_BGR2GRAY);
	//blur(src_gray, src_gray, Size(3, 3));

	ThreshCallback(0, 0, src, imgThresholded, thresh);
}

/**
* @function thresh_callback
*/
void ObjectDetectionService::ThreshCallback(int, void*, Mat src, Mat src_gray, int thresh) {
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	waitKey();
}

Shot ObjectDetectionService::DetectShotRoute(const Mat &frame, int player) {
	// TODO always calculate beginning at the robot til a wall is hit because we do not know if finally a robot or a wall will be hitten
	++nextShot;

	if (nextShot >= shotsSize) {
		nextShot = 0;
	}

	return shots[nextShot];
}

bool ObjectDetectionService::HasShotHitPlayer(const Mat &frame, Shot shot) {
	// TODO analyse frame and detect if a player is at shot endposition
	return true;
}