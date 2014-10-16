//============================================================================
// Name        : WebcamService.cpp
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

#include "WebcamService.h"
#include "..\shared\Logger.h"

using namespace cv;
using namespace std;

WebcamService::WebcamService(string windowName) : streamWindowName(windowName.c_str()), capture(cvCaptureFromCAM(CV_CAP_ANY)) {
	isStopRequested = false;
	cvNamedWindow(streamWindowName, CV_WINDOW_NORMAL);
}

WebcamService::~WebcamService() {
	StopRecording();
}

bool WebcamService::StartRecording() {
	Logger::addMessage("starting recording...");

	if(!capture){
		Logger::addError("No camera found");
		isStopRequested = false;
		return false;
	}

	isStopRequested = false;
	LPVOID param = this;
	recordingThread = CreateThread(NULL, 0, StartRecordingThread, (void*) this, 0, &recordingThreadId);

	Logger::addMessage("started recording");

	return true;
}

bool WebcamService::StopRecording() {
	Logger::addMessage("stopping recording...");

	isStopRequested = true;

	//wait till thread has been terminated
	DWORD success = SuspendThread(recordingThread);

	if (-1 == success) {
		Logger::addError("Could not stop recording thread: " + GetLastError());
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

DWORD WebcamService::Recording() {
	Logger::addMessage("recording thread started");

	IplImage* frame;

	//Create infinte loop for live streaming
	while (1){
		//Create image frames from capture
		frame = cvQueryFrame(capture);
		//Show image frames on created window
		cvShowImage("Webcam stream", frame);

		if (isStopRequested) {
			break; //If you hit ESC key loop will break.
		}
	}

	return 0;
}

DWORD WINAPI WebcamService::StartRecordingThread(LPVOID param) {
	WebcamService* instance = reinterpret_cast<WebcamService*>(param);

	if (!instance) {
		Logger::addError("webcam streaming thread starting failed");

		return 1;
	}

	instance->Recording();
	return 0;
}