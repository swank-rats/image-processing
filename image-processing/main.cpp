//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Image process start point
//============================================================================
#include <iostream>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>

#include "services\videostreaming\webcam\WebcamService.h"
#include "services\ObjectDetectionService.h"
#include "controller\ImageProcessingController.h"
#include "controller\VideoStreamingController.h"

using namespace cv;
using namespace std;
using Poco::Logger;
using Poco::AutoPtr;
using Poco::WindowsConsoleChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;

enum Farbe { ROT = 0, GRUEN = 1, BLAU = 2 };

void InitLoggers();
void ShowLena(Logger& logger);
int WithoutThread(Logger& logger);
int WithThread(Logger& logger);
void FilterColor(Farbe color);
int  DetectWihoutServices(Logger& logger);

int main(int argc, char** argv)
{
	InitLoggers();
	Logger& logger = Logger::get("main");

	ShowLena(logger);

	return WithThread(logger);
}

void InitLoggers() {
	//setup logger
	AutoPtr<WindowsConsoleChannel> pChannel(new WindowsConsoleChannel);
	AutoPtr<PatternFormatter> pPF(new PatternFormatter);
	pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s (%p): %t");
	AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pChannel));
	Logger::root().setChannel(pFC);
}

void ShowLena(Logger& logger) {
	Mat im = imread("img/lena.bmp", 1);
	if (!im.empty())
	{
		cvNamedWindow("Lena");
		//imshow("Lena", im);
		imshow("Lena", im);
		logger.information("Hello Leno :)");
	}
	else {
		logger.warning("Lena could not be found! Go and find her!");
	}
}

int WithoutThread(Logger& logger) {
	logger.information("No threads were used");

	cvNamedWindow("Camera stream", CV_WINDOW_AUTOSIZE);

	CvCapture* capture = cvCaptureFromCAM(1);

	if (!capture){
		logger.critical("No camera found");
		return -1;
	}

	IplImage* frame;
	char key;

	//Create infinte loop for live streaming
	//exit by pressing ESC
	while (1) {
		//Create image frames from capture
		frame = cvQueryFrame(capture);
		//Show image frames on created window
		cvShowImage("Camera stream", frame);

		//Capture Keyboard stroke
		key = cvWaitKey(10);
		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	//Release capture.
	cvReleaseCapture(&capture);
	//Destroy Window
	destroyAllWindows();

	return 0;
}

int WithThread(Logger& logger)
{
	logger.information("Threads were used");

	auto webcamService = std::make_shared<services::webcam::WebcamService>();

	controller::image_processing::ImageProcessingController imgProcCtrl(webcamService);
	imgProcCtrl.StartImageProcessing();

	controller::video_streaming::VideoStreamingController vidStreamCtrl(webcamService);
	vidStreamCtrl.StartStreamingServer();

	char key;
	while (1) {
		key = cvWaitKey(10);

		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	imgProcCtrl.StopImageProcessing();
	vidStreamCtrl.StopNetworkService();

	destroyAllWindows();

	return 0;
}

int DetectWihoutServices(Logger& logger){

	VideoCapture cap(0); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		logger.critical("Cannot open the web cam");
		return -1;
	}

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	while (true)
	{
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			logger.warning("Cannot read a frame from video stream");
			break;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//morphological opening (removes small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		imshow("Thresholded Image", imgThresholded); //show the thresholded image
		imshow("Original", imgOriginal); //show the original image

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			logger.information("esc key is pressed by user");
			break;
		}
	}

	return 0;
}