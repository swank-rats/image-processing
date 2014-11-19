//============================================================================
// Name        : ImageProcessingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

#include "ImageProcessingController.h"
#include "..\services\ObjectDetectionService.h"

using cv::Mat;
using cv::Rect;
using cv::Point;

namespace controller {
	namespace image_processing {
		int iLowH = 0;
		int iHighH = 179;

		int iLowS = 0;
		int iHighS = 255;

		int iLowV = 0;
		int iHighV = 255;

		Mat gunShot;
		Rect roi;

		ImageProcessingController::ImageProcessingController(WebcamServicePtr webcamService) : webcamService(webcamService)
		{
			detectService= new ObjectDetectionService();
			gunShot = imread("resources/images/gunfire_small.png", -1);
			imshow("gunshot", gunShot);
			roi = Rect(Point(10, 10), gunShot.size());
		}

		ImageProcessingController::~ImageProcessingController()
		{
			delete detectService;
			detectService = nullptr;	
		}

		void ImageProcessingController::StartImageProcessing() {

			webcamService->AddObserver(this);

			CreateTrackBarView();

			webcamService->StartRecording();
		}

		void ImageProcessingController::StopImageProcessing() {
			webcamService->StopRecording();

			webcamService->RemoveObserver(this);
		}

		void ImageProcessingController::Update(WebcamService* observable) {
			static bool shown = false;
			//TODO still needed?
			Mat frame = observable->GetLastImage();

			if (!shown && !frame.empty()) {
				//gunShot.copyTo(frame.colRange(0, 80).rowRange(0, 80));

				//gunShot.copyTo(frame(roi));
				//imshow("drawn shot", frame);
				shown = true;
			}

			//Mat src(5, 7, CV_8UC1, Scalar(1)); // 5x7
			//Mat dst(10, 10, CV_8UC1, Scalar(0)); // 10x10

			//src.copyTo(dst.rowRange(1, 6).colRange(3, 10));

			//cv::Rect roi(cv::Point(originX, originY), cv::Size(width, height));
			//cv::Mat destinationROI = bigImage(roi);
			//smallImage.copyTo(destinationROI);
/*
			If you are certain the small image fits into the big image then you could simply do:

			cv::Rect roi(cv::Point(originX, originY), smallImage.size());
			smallImage.copyTo(bigImage(roi));*/

			//detectService->DetectObject(frame, iLowH, iLowS, iLowV, iHighH, iHighS, iHighV);
		}

		void ImageProcessingController::HandleMessageNotification(const AutoPtr<MessageNotification>& notification) {
			//TODO handle message - eg simulate shot and so on
		}

		void ImageProcessingController::CreateTrackBarView(){
			namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

			//Create trackbars in "Control" window
			cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
			cvCreateTrackbar("HighH", "Control", &iHighH, 179);

			cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
			cvCreateTrackbar("HighS", "Control", &iHighS, 255);

			cvCreateTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
			cvCreateTrackbar("HighV", "Control", &iHighV, 255);
		}
	}
}
