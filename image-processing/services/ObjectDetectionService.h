//============================================================================
// Name        : ObjectDetectionService.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================

#pragma once

#include"..\shared\Thread.h"

using namespace std;
using namespace cv;




class ObjectDetectionService : public Thread
{
public:
	ObjectDetectionService();
	virtual ~ObjectDetectionService();

	Mat DetectObject(IplImage* src, int iLowH, int iLowS, int iLowV, int iHighH, int iHighS, int iHighV);

protected:
	virtual void run();
};

