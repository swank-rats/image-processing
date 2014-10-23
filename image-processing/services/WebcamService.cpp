//============================================================================
// Name        : WebcamService.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#include <boost/log/trivial.hpp>

#include "WebcamService.h"

static const char* windowName = "Webcam stream";

WebcamService::WebcamService() {
	capture = cvCaptureFromCAM(1);
	cvNamedWindow(windowName, CV_WINDOW_NORMAL);
}

WebcamService::~WebcamService() {
	StopRecording();
}

bool WebcamService::StartRecording() {
	BOOST_LOG_TRIVIAL(info) << "starting recording...";

	if(!capture){
		BOOST_LOG_TRIVIAL(error) << "No camera found";
		return false;
	}

	recordingThread = new boost::thread(boost::bind(&WebcamService::RecordingCore, this));

	BOOST_LOG_TRIVIAL(info) << "started recording";

	return true;
}

bool WebcamService::StopRecording() {
	BOOST_LOG_TRIVIAL(info) << "stopping recording...";

	recordingThread->join();

	//release resources
	if(capture) {
		//Release capture.
		cvReleaseCapture(&capture);
	}

	BOOST_LOG_TRIVIAL(info) << "stopped recording";

	return true;
}

void WebcamService::RecordingCore() {
	IplImage* frame;

	try {
		do {
			//Create image frames from capture
			frame = cvQueryFrame(capture);
			//Show image frames on created window
			cvShowImage(windowName, frame);
			//Clone image
			lastImage = cvCloneImage(frame);

			Notify();
		} while (!recordingThread->interruption_requested());
	}
	catch (boost::thread_interrupted const& e) {
		BOOST_LOG_TRIVIAL(warning) << "recording thread stopped..";
	}
}

IplImage* WebcamService::GetLastImage() {
	return lastImage;
}