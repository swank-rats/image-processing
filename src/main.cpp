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

#include "Services/WebcamService.h"

using namespace cv;
using namespace std;

char key;

int main(int argc, char** argv) {
	string windowName = "Camera stream";

	cvNamedWindow(windowName.c_str(), CV_WINDOW_NORMAL);

	Mat im = imread(argc == 2 ? argv[1] : "lena.png", 1);

	if (!im.empty())
	{
		imshow("Lena", im);
	}

	WebcamService* webcamService = new WebcamService(windowName);

	cout << "Start recording: " << webcamService->startRecording() << endl;

	//Destroy Window
	destroyAllWindows();

	return 0;
}
