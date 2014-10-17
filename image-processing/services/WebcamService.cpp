//============================================================================
// Name        : WebcamService.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================

#include "WebcamService.h"
#include "..\shared\Logger.h"

static const char* windowName = "Webcam stream";

WebcamService::WebcamService() {
	capture = cvCaptureFromCAM(1);
	cvNamedWindow(windowName, CV_WINDOW_NORMAL);
}

WebcamService::~WebcamService() {
	StopRecording();
}

bool WebcamService::StartRecording() {
	Logger::addMessage("starting recording...");

	if(!capture){
		Logger::addError("No camera found");
		return false;
	}

	Start();

	Logger::addMessage("started recording");

	return true;
}

bool WebcamService::StopRecording() {
	Logger::addMessage("stopping recording...");

	if (!Stop()) {
		Logger::addError("Could not stop recording thread");
		return false;
	}

	//release resources
	if(capture) {
		//Release capture.
		cvReleaseCapture(&capture);
	}

	Logger::addMessage("stopped recording");

	return true;
}

void WebcamService::run() {
	IplImage* frame;
	//Create image frames from capture
	frame = cvQueryFrame(capture);
	//Show image frames on created window
	cvShowImage(windowName, frame);
	//Clone image
	lastImage = cvCloneImage(frame);

	Notify();
}

IplImage* WebcamService::GetLastImage() {
	return lastImage;
}