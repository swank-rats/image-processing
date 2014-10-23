//============================================================================
// Name        : ObjectDetectionService.cpp
// Author      : ITM13
// Version     : 1.0
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

	Mat DetectObject(IplImage* src, int iLowH, int iLowS, int iLowV, int iHighH, int iHighS, int iHighV);
};

