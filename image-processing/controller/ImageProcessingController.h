//============================================================================
// Name        : ImageProcessingController.h
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#pragma once

#include "..\services\WebcamService.h"

class ImageProcessingController
{
public:
	ImageProcessingController();
	~ImageProcessingController();
	void StartImageProcessing();
	bool StopImageProcessing();
private:
	WebcamService* webcamService;

};

