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

WebcamService::WebcamService(string windowName) : windowName(windowName), capture(cvCaptureFromCAM(CV_CAP_ANY)) {

}

WebcamService::~WebcamService() {
	// TODO Auto-generated destructor stub
}

bool WebcamService::startRecording() {
	if(!capture){
		cout << "No camera found." << endl;
		return false;
	}

	isStopRequested = false;

	recording();

	return true;
}

bool WebcamService::stopRecording() {
	isStopRequested = true;

	//wait till thread has been terminated


	//release resources
	if(capture) {
		//Release capture.
		cvReleaseCapture(&capture);
	}

	return true;
}

void WebcamService::recording() {
	IplImage* frame;

	//Create infinte loop for live streaming
	while(1){
		//Create image frames from capture
		frame = cvQueryFrame(capture);
		//Show image frames on created window
		cvShowImage("Camera stream", frame);

		if (isStopRequested){
			break; //If you hit ESC key loop will break.
		}
	}
}


