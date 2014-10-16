//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : Image process start point
//============================================================================

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <thread>

//#include "Services/WebcamService.h"


using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

char key;

int main(int argc, char** argv)
{
	cvNamedWindow("Camera stream", CV_WINDOW_NORMAL);

	Mat im = imread("img/lena.bmp", 1);
	if (!im.empty())
	{
		cvNamedWindow("Lena");
		//imshow("Lena", im);
		imshow("Lena", im);
	}

	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

	if (!capture){
		cout << "No camera found." << endl;
		return -1;
	}

	IplImage* frame;

	//Create infinte loop for live streaming
	//exit by pressing ESC
	while (1){
		//Create image frames from capture
		frame = cvQueryFrame(capture);
		//Show image frames on created window
		cvShowImage("Camera stream", frame);

		//sending data

		//Capture Keyboard stroke
		key = cvWaitKey(10);
		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	//Release capture.
	cvReleaseCapture(&capture);
	//Destroy Window
	destroyAllWindows();

	return 0;
}

//int main(int argc, char** argv)
//{
//	string windowName = "Camera stream";
//
//	cvNamedWindow(windowName.c_str(), CV_WINDOW_NORMAL);
//
//	//Mat im = imread("lena.png", 1);
//
//	//if (!im.empty())
//	//{
//	//	imshow("Lena", im);
//	//}
//
//	WebcamService* webcamService = new WebcamService(windowName);
//
//	cout << "Start recording: " << webcamService->startRecording() << endl;
//
//	char key;
//	while (1) {
//		key = cvWaitKey(10);
//
//		if (char(key) == 27){
//			break; //If you hit ESC key loop will break.
//		}
//	}
//
//	webcamService->stopRecording();
//	destroyAllWindows();
//
//	return 0;
//}