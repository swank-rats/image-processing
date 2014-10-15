//============================================================================
// Name        : image-processing.cpp
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
#include <thread>

#include "Services/WebcamService.h"


using namespace std;

void output() {
	cout << "thread does some work" << endl;
}


//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char** argv)
{
	string windowName = "Camera stream";

	cvNamedWindow(windowName.c_str(), CV_WINDOW_NORMAL);

	//Mat im = imread("lena.png", 1);

	//if (!im.empty())
	//{
	//	imshow("Lena", im);
	//}

	WebcamService* webcamService = new WebcamService(windowName);

	cout << "Start recording: " << webcamService->startRecording() << endl;
	
	char key;
	while (1) {
		key = cvWaitKey(10);

		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	webcamService->stopRecording();
	destroyAllWindows();

	return 0;
}