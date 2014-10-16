//============================================================================
// Name        : WebcamService.h
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================

#pragma once

#include <windows.h>

using namespace std;
using namespace cv;

class WebcamService {
public:
	WebcamService(string windowName);
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
private:
	//members
    CvCapture* capture;
    bool isStopRequested;
	HANDLE recordingThread;
	DWORD recordingThreadId;
	const char* streamWindowName;

	//methods
	DWORD Recording();
	static DWORD WINAPI StartRecordingThread(LPVOID param);
};
