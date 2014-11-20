//============================================================================
// Name        : ImageProcessingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <algorithm>

#include "ImageProcessingController.h"
#include "..\services\ObjectDetectionService.h"

using std::max;
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
			gunShot = imread("resources/images/gunfire_small.png", CV_LOAD_IMAGE_UNCHANGED);
			roi = Rect(Point(0, 0), gunShot.size());
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

			try {
				Mat frame = observable->GetLastImage();
				OverlayImage(frame, gunShot, Point2i(0, 0));
			}
			catch (cv::Exception& e) {
				Logger& logger = Logger::get("Test");
				logger.error(e.what());
			}

			//detectService->DetectObject(frame, iLowH, iLowS, iLowV, iHighH, iHighS, iHighV);
		}

		void ImageProcessingController::OverlayImage(cv::Mat &background, const cv::Mat &foreground, cv::Point2i location)
		{
			// start at the row indicated by location, or at row 0 if location.y is negative.
			for (int y = max(location.y, 0); y < background.rows; ++y)
			{
				int fY = y - location.y; // because of the translation

				// we are done of we have processed all rows of the foreground image.
				if (fY >= foreground.rows)
					break;

				// start at the column indicated by location, 

				// or at column 0 if location.x is negative.
				for (int x = max(location.x, 0); x < background.cols; ++x)
				{
					int fX = x - location.x; // because of the translation.

					// we are done with this row if the column is outside of the foreground image.
					if (fX >= foreground.cols) {
						break;
					}

					// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
					double opacity = ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3]) / 255.;


					// and now combine the background and foreground pixel, using the opacity, 

					// but only if opacity > 0.
					for (int c = 0; opacity > 0 && c < background.channels(); ++c)
					{
						unsigned char foregroundPx = foreground.data[fY * foreground.step + fX * foreground.channels() + c];
						unsigned char backgroundPx = background.data[y * background.step + x * background.channels() + c];
						background.data[y * background.step + background.channels()*x + c] = backgroundPx * (1. - opacity) + foregroundPx * opacity;
					}
				}
			}
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
