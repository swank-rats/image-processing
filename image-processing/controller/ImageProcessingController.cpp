//============================================================================
// Name        : ImageProcessingController.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>

#include "ImageProcessingController.h"
#include "..\shared\Logger.h"
#include "..\services\ObjectDetectionService.h"



int iLowH = 0;
int iHighH = 179;

int iLowS = 0;
int iHighS = 255;

int iLowV = 0;
int iHighV = 255;

ImageProcessingController::ImageProcessingController()
{
	webcamService = new WebcamService();
}


ImageProcessingController::~ImageProcessingController()
{
	StopImageProcessing();
	webcamService = nullptr;
}

void ImageProcessingController::StartImageProcessing() {

	webcamService->AddObserver(this);


	CreateTrackBarView();
	


	webcamService->StartRecording();
}

bool ImageProcessingController::StopImageProcessing() {
	webcamService->StopRecording();

	webcamService->RemoveObserver(this);

	return true;
}

void ImageProcessingController::Update(WebcamService* observable) {
	IplImage* frame= observable->GetLastImage();

	Mat input(frame);
	imshow("input", input);
	waitKey();

	/*
	Mat input = imread("img/colored_squares.png");
	imshow("input", input);
	waitKey();*/

	

	IplImage* image2 = cvCloneImage(&(IplImage)input);
	ObjectDetectionService* detectService = new ObjectDetectionService();
	Mat redOnly = detectService->DetectObject(frame,  iLowH,  iLowS,  iLowV,  iHighH,  iHighS,  iHighV);
	imshow("redOnly", redOnly);
	waitKey();
}


void ImageProcessingController::CreateTrackBarView(){
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);


}
