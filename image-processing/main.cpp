//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : Image process start point
//============================================================================

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>

#include "services\WebcamService.h"
#include "services\ObjectDetectionService.h"
#include "shared\Logger.h"

using namespace cv;
using namespace std;

enum Farbe { ROT = 0, GRUEN = 1, BLAU = 2 };

void ShowLena();
int WithoutThread();
int WithThread();
void FilterColor(Farbe color);

int main(int argc, char** argv)
{
	//ShowLena();

	FilterColor(ROT);

	return WithoutThread();
}

void ShowLena() {
	Mat im = imread("img/lena.bmp", 1);
	if (!im.empty())
	{
		cvNamedWindow("Lena");
		//imshow("Lena", im);
		imshow("Lena", im);
		Logger::addMessage("Hello Leno :)");
	}
	else {
		Logger::addMessage("Lena could not be found! Go and find her!");
	}
}

void FilterColor(Farbe color)
{
	Mat input = imread("img/colored_squares.png");

	imshow("input", input);
	waitKey();

	ObjectDetectionService* detectService = new ObjectDetectionService();

	Mat redOnly = detectService->DetectObject(input, color);

	imshow("redOnly", redOnly);
	waitKey();

	// detect squares after filtering...

}

int WithoutThread() {
	Logger::addMessage("No threads were used");

	cvNamedWindow("Camera stream", CV_WINDOW_NORMAL);

	CvCapture* capture = cvCaptureFromCAM(1);

	if (!capture){
		Logger::addError("No camera found");
		return -1;
	}

	IplImage* frame;
	char key;

	//Create infinte loop for live streaming
	//exit by pressing ESC
	while (1) {
		//Create image frames from capture
		frame = cvQueryFrame(capture);
		//Show image frames on created window
		cvShowImage("Camera stream", frame);

		//Capture Keyboard stroke
		key = cvWaitKey(10);
		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	Logger::addMessage("bye bye");

	//Release capture.
	cvReleaseCapture(&capture);
	//Destroy Window
	destroyAllWindows();

	return 0;
}

int WithThread()
{
	Logger::addMessage("Threads were used");

	WebcamService* webcamService = new WebcamService("Webcam stream");

	webcamService->StartRecording();

	char key;
	while (1) {
		key = cvWaitKey(10);

		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	webcamService->StopRecording();
	destroyAllWindows();

	return 0;
}