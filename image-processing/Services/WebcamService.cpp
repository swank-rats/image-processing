/*
 * WebcamService.cpp
 *
 *  Created on: 11.10.2014
 *      Author: Stefan
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "WebcamService.h"

using namespace cv;
using namespace std;

WebcamService::WebcamService(string windowName) : windowName(windowName), capture(cvCaptureFromCAM(CV_CAP_ANY)) {
	isStopRequested = false;
}

WebcamService::~WebcamService() {
	// TODO Auto-generated destructor stub
}

void WebcamService::recording() {
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

bool WebcamService::startRecording() {
	if(!capture){
		cout << "No camera found." << endl;
		return false;
	}

	isStopRequested = false;

	thread recordingThread(&WebcamService::recording, this);

	return true;
}

bool WebcamService::stopRecording() {
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




