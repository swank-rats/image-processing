//============================================================================
// Name        : ImageProcessingController.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#include "ImageProcessingController.h"


ImageProcessingController::ImageProcessingController()
{
	webcamService = new WebcamService();
}


ImageProcessingController::~ImageProcessingController()
{
	StopImageProcessing();
	webcamService = NULL;
}

void ImageProcessingController::StartImageProcessing() {
	webcamService->StartRecording();
}

bool ImageProcessingController::StopImageProcessing() {
	webcamService->StopRecording();

	return true;
}
