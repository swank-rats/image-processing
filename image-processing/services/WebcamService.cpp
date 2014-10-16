//============================================================================
// Name        : web.cpp
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

#include "WebcamService.h"

using namespace cv;
using namespace std;

WebcamService::WebcamService(string windowName) : windowName(windowName), capture(cvCaptureFromCAM(CV_CAP_ANY)) {

	isStopRequested = false;
	cvNamedWindow(windowName.c_str(), CV_WINDOW_NORMAL);
}

WebcamService::~WebcamService() {
	StopRecording();
}

bool WebcamService::StartRecording() {
	if(!capture){
		cout << "No camera found." << endl;
		return false;
	}

	isStopRequested = false;

	thread recordingThread(&WebcamService::Recording, this);

	return true;
}

bool WebcamService::StopRecording() {
	isStopRequested = true;

	//wait till thread has been terminated
	recordingThread.join();

	//release resources
	if(capture) {
		//Release capture.
		cvReleaseCapture(&capture);
	}

	return true;
}

void WebcamService::Recording() {
	cout << "started recording" << endl;

	IplImage* frame;

	//Create infinte loop for live streaming
	while (1){
		//Create image frames from capture
		frame = cvQueryFrame(capture);
		//Show image frames on created window
		cvShowImage("Camera stream", frame);

		if (isStopRequested){
			break; //If you hit ESC key loop will break.
		}
	}
}