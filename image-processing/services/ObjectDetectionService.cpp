//============================================================================
// Name        : ObjectDetectionService.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================


#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>

#include "ObjectDetectionService.h"

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

void thresh_callback(int, void*);


ObjectDetectionService::ObjectDetectionService()
{

}


ObjectDetectionService::~ObjectDetectionService()
{

}

void ObjectDetectionService::DetectObject(Mat src, int iLowH, int iLowS, int iLowV, int iHighH, int iHighS, int iHighV){


	Mat imgOriginal(src);
	Mat imgHSV;

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	Mat imgThresholded;

	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

	//morphological opening (removes small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (removes small holes from the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	imshow("input", imgThresholded);
	waitKey();


	/// Convert image to gray and blur it
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	///// Create Window
	//const char* source_window = "Source";
	//namedWindow(source_window, WINDOW_AUTOSIZE);
	//imshow(source_window, src);
	//waitKey();
	thresh_callback(0, 0);



	//CvMemStorage *storage;
	//IplImage* imgGrayScale;
	//IplImage* img = src;

	////converting the original image into grayscale
	//imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1);
	//cvCvtColor(img, imgGrayScale, CV_BGR2GRAY);

	////thresholding the grayscale image to get better results
	//cvThreshold(imgGrayScale, imgGrayScale, 128, 255, CV_THRESH_BINARY);

	//CvSeq* contours;  //hold the pointer to a contour in the memory block
	//CvSeq* result;   //hold sequence of points of a contour
	//storage = cvCreateMemStorage(0); //storage area for all contours

	////finding all contours in the image
	//cvFindContours(imgGrayScale, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	////iterating through each contour
	//while (contours)
	//{
	//	//obtain a sequence of points of contour, pointed by the variable 'contour'
	//	result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);

	//	//if there are 3  vertices  in the contour(It should be a triangle)
	//	if (result->total == 3)
	//	{
	//		//iterating through each point
	//		CvPoint *pt[3];
	//		for (int i = 0; i < 3; i++){
	//			pt[i] = (CvPoint*)cvGetSeqElem(result, i);
	//		}

	//		//drawing lines around the triangle
	//		cvLine(img, *pt[0], *pt[1], cvScalar(255, 0, 0), 4);
	//		cvLine(img, *pt[1], *pt[2], cvScalar(255, 0, 0), 4);
	//		cvLine(img, *pt[2], *pt[0], cvScalar(255, 0, 0), 4);

	//	}

	//	//if there are 4 vertices in the contour(It should be a quadrilateral)
	//	else if (result->total == 4)
	//	{
	//		//iterating through each point
	//		CvPoint *pt[4];
	//		for (int i = 0; i < 4; i++){
	//			pt[i] = (CvPoint*)cvGetSeqElem(result, i);
	//		}

	//		//drawing lines around the quadrilateral
	//		cvLine(img, *pt[0], *pt[1], cvScalar(0, 255, 0), 4);
	//		cvLine(img, *pt[1], *pt[2], cvScalar(0, 255, 0), 4);
	//		cvLine(img, *pt[2], *pt[3], cvScalar(0, 255, 0), 4);
	//		cvLine(img, *pt[3], *pt[0], cvScalar(0, 255, 0), 4);
	//	}

	//	//if there are 7  vertices  in the contour(It should be a heptagon)
	//	else if (result->total == 7)
	//	{
	//		//iterating through each point
	//		CvPoint *pt[7];
	//		for (int i = 0; i < 7; i++){
	//			pt[i] = (CvPoint*)cvGetSeqElem(result, i);
	//		}

	//		//drawing lines around the heptagon
	//		cvLine(img, *pt[0], *pt[1], cvScalar(0, 0, 255), 4);
	//		cvLine(img, *pt[1], *pt[2], cvScalar(0, 0, 255), 4);
	//		cvLine(img, *pt[2], *pt[3], cvScalar(0, 0, 255), 4);
	//		cvLine(img, *pt[3], *pt[4], cvScalar(0, 0, 255), 4);
	//		cvLine(img, *pt[4], *pt[5], cvScalar(0, 0, 255), 4);
	//		cvLine(img, *pt[5], *pt[6], cvScalar(0, 0, 255), 4);
	//		cvLine(img, *pt[6], *pt[0], cvScalar(0, 0, 255), 4);
	//	}

	//	//obtain the next contour
	//	contours = contours->h_next;
	//}

	//return img;


	//Mat imgOriginal(src);
	//Mat imgHSV;

	//cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	//Mat imgThresholded;

	//inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

	////morphological opening (removes small objects from the foreground)
	//erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	////morphological closing (removes small holes from the foreground)
	//dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//return imgThresholded;

	//return src;
}



/**
* @function thresh_callback
*/
void thresh_callback(int, void*)
{
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