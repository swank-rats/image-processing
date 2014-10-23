//============================================================================
// Name        : ImageProcessingController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include "..\services\videostreaming\webcam\WebcamService.h"
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

