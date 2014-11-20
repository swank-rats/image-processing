//============================================================================
// Name        : ObjectDetectionService.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================

#pragma once
#include "..\shared\model\Shot.h"

using namespace std;
using namespace cv;
using shared::model::Shot;

class ObjectDetectionService
{
public:
	ObjectDetectionService();
	virtual ~ObjectDetectionService();

	void DetectObject(Mat src, int iLowH, int iLowS, int iLowV, int iHighH, int iHighS, int iHighV);
	void ThreshCallback(int, void*, Mat src, Mat src_gray, int thresh);

	Shot DetectShotRoute(const Mat &frame, int player); //TODO add player parameter
private:
	Mat src; 
	Mat src_gray;
	int thresh = 100;
	int max_thresh = 255;
	

};

