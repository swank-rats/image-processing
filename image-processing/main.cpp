//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : Image process start point
//============================================================================

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>

#include "services\WebcamService.h"
#include "shared\Logger.h"

using namespace cv;
using namespace std;

void ShowLena();
int WithoutThread();
int WithThread();

int main(int argc, char** argv)
{
	ShowLena();

	return WithThread();
}

void ShowLena() {
	Mat im = imread("img/lena.bmp", 1);
	if (!im.empty())
	{
		cvNamedWindow("Lena");
		//imshow("Lena", im);
		imshow("Lena", im);
		Logger::addMessage("Hello Leno :)");
	}
	else {
		Logger::addMessage("Lena could not be found! Go and find her!");
	}
}

int WithoutThread() {
	Logger::addMessage("No threads were used");

	cvNamedWindow("Camera stream", CV_WINDOW_AUTOSIZE);

	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

	if (!capture){
		Logger::addError("No camera found");
		return -1;
	}

	IplImage* frame;
	char key;

	//Create infinte loop for live streaming
	//exit by pressing ESC
	while (1) {
		//Create image frames from capture
		frame = cvQueryFrame(capture);
		//Show image frames on created window
		cvShowImage("Camera stream", frame);

		//Capture Keyboard stroke
		key = cvWaitKey(10);
		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	Logger::addMessage("bye bye");

	//Release capture.
	cvReleaseCapture(&capture);
	//Destroy Window
	destroyAllWindows();

	return 0;
}

int WithThread()
{
	Logger::addMessage("Threads were used");

	WebcamService* webcamService = new WebcamService();

	webcamService->StartRecording();

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