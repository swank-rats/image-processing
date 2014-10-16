//============================================================================
// Name        : WebcamService.h
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================

#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <windows.h>
#include "..\shared\Thread.h"

class WebcamService : public Thread {
public:
	WebcamService();
	virtual ~WebcamService();

	/*
	 * This method will start the recording from the webcam
	 * Returns true if starting was successful, false otherwise
	 */
	bool StartRecording();
	/*
	 * This method will stop the recodirng from the webcam
	 */
	bool StopRecording();
protected:
	virtual void run();
private:
    CvCapture* capture;
};
