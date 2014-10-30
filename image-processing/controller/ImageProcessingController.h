//============================================================================
// Name        : ImageProcessingController.h
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#pragma once

#include "..\services\WebcamService.h"
#include "..\services\ObjectDetectionService.h"
#include "..\shared\observer\IObserver.h"

class ImageProcessingController : public IObserver<WebcamService>
{
public:
	ImageProcessingController();
	~ImageProcessingController();
	void StartImageProcessing();
	bool StopImageProcessing();
	virtual void Update(WebcamService* observable);

private:
	WebcamService* webcamService;
	ObjectDetectionService* detectService;
	int iLowH = 0;
	int iHighH = 179;
	int iLowS = 0;
	int iHighS = 255;
	int iLowV = 0;
	int iHighV = 255;
	void CreateTrackBarView();

};

