//============================================================================
// Name        : ObjectDetectionService.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================

#pragma once

using namespace std;
using namespace cv;


class ObjectDetectionService
{
public:
	ObjectDetectionService();
	virtual ~ObjectDetectionService();

	bool DetectObject();
};

