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
#include "services/WebcamService.h"

using namespace cv;
using namespace std;

void ShowLena();
int WithoutThread();
int WithThread();

int main(int argc, char** argv)
{
	ShowLena();

	return WithoutThread();
}

void ShowLena() {
	Mat im = imread("img/lena.bmp", 1);
	if (!im.empty())
	{
		cvNamedWindow("Lena");
		//imshow("Lena", im);
		imshow("Lena", im);
	}
}

int WithoutThread() {
	cvNamedWindow("Camera stream", CV_WINDOW_NORMAL);

	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

	if (!capture){
		cout << "No camera found." << endl;
		return -1;
	}

	IplImage* frame;
	char key;

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

int WithThread()
{
	WebcamService* webcamService = new WebcamService("Webcam stream");

	cout << "Start recording: " << webcamService->StopRecording() << endl;

	char key;
	while (1) {
		key = cvWaitKey(10);

		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	webcamService->StopRecording();
	destroyAllWindows();

	return 0;
}