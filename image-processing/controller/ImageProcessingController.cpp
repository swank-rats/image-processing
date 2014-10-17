//============================================================================
// Name        : ImageProcessingController.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#include "ImageProcessingController.h"
#include "..\shared\Logger.h"

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

	webcamService->StartRecording();
}

bool ImageProcessingController::StopImageProcessing() {
	webcamService->StopRecording();

	webcamService->RemoveObserver(this);

	return true;
}

void ImageProcessingController::Update(WebcamService* observable) {
	observable->GetLastImage();


}
