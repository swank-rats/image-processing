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

using namespace cv;
using namespace std;

char key;

int main(int argc, char** argv)
{
	cvNamedWindow("Camera stream", CV_WINDOW_NORMAL);

	Mat im = imread(argc == 2 ? argv[1] : "lena.png", 1);

	if (!im.empty())
	{
		imshow("Lena", im);
	}

	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

	while(1){ //Create infinte loop for live streaming
		IplImage* frame = cvQueryFrame(capture); //Create image frames from capture
		cvShowImage("Camera stream", frame);   //Show image frames on created window

		key = cvWaitKey(10);     //Capture Keyboard stroke
		if (char(key) == 27){
			break;//If you hit ESC key loop will break.
		}
	}

	cvReleaseCapture(&capture); //Release capture.
	destroyAllWindows(); //Destroy Window

	return 0;
}
