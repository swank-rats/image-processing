//============================================================================
// Name        : ImageProcessingController.h
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================
#pragma once

#include "..\services\WebcamService.h"
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
	void CreateTrackBarView();

};

