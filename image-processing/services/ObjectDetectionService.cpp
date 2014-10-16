//============================================================================
// Name        : ObjectDetectionService.cpp
// Author      : ITM13
// Version     : 1.0
// Copyright   : Copyright (c) 2014 Swank Rat, MIT License (MIT)
// Description : 
//============================================================================


#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>

#include "ObjectDetectionService.h"
#include "..\shared\Logger.h"

using namespace cv;
using namespace std;

ObjectDetectionService::ObjectDetectionService()
{

}


ObjectDetectionService::~ObjectDetectionService()
{

}

Mat ObjectDetectionService::DetectObject(const Mat& src, int color){

	assert(src.type() == CV_8UC3);

	Mat redOnly;

	int red = 0;
	int blue = 0;
	int green = 0;

	switch (color)
	{
	case 0:
		red = 255;
		break;
	case 1:
		green = 255;
		break;
	case 2:
		blue = 255;
		break;
	default:
		red = 0;
		green = 0;
		blue = 0;
	}

	inRange(src, Scalar(0, 0, 0), Scalar(blue, green, red), redOnly);

	return redOnly;
}

void ObjectDetectionService::run() {

}

