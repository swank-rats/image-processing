//============================================================================
// Name        : WebcamService.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <boost/log/trivial.hpp>
#include "WebcamService.h"

static const char* windowName = "Webcam stream";

WebcamService::WebcamService() {
	capture = cvCaptureFromCAM(CV_CAP_ANY);
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

	if (recordingThread != NULL) {
		while (recordingThread->timed_join(boost::posix_time::seconds(2)) == false)
		{
			// Interupt the thread
			BOOST_LOG_TRIVIAL(info) << "recording thread interrupt request sent";
			recordingThread->interrupt();
		}

		BOOST_LOG_TRIVIAL(info) << "recording thread stopped successfully";

	}
	else {
		BOOST_LOG_TRIVIAL(error) << "recording thread is already disposed!";
	}

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
		while(1) {
			//Create image frames from capture
			frame = cvQueryFrame(capture);
			//Show image frames on created window
			cvShowImage(windowName, frame);
			//Clone image
			lastImage = cvCloneImage(frame);

			//Notify();

			//here is the place where the thread can be interrupted with join
			boost::this_thread::interruption_point();
		}
	}
	catch (boost::thread_interrupted) {
		BOOST_LOG_TRIVIAL(warning) << "recording thread stopped by interrupt";
	}
}

IplImage* WebcamService::GetLastImage() {
	return lastImage;
}