//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Image process start point
//============================================================================

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <boost\log\trivial.hpp>

#include "services\videostreaming\webcam\WebcamService.h"
#include "services\ObjectDetectionService.h"
#include "controller\ImageProcessingController.h"
#include "controller\VideoStreamingController.h"

using namespace cv;
using namespace std;

enum Farbe { ROT = 0, GRUEN = 1, BLAU = 2 };

void ShowLena();
int WithoutThread();
int WithThread();
void FilterColor(Farbe color);
int  DetectWihoutServices();

int main(int argc, char** argv)
{
	ShowLena();

	return WithThread();
}

void ShowLena() {
	Mat im = imread("img/lena.bmp", 1);
	if (!im.empty())
	{
		cvNamedWindow("Lena");
		//imshow("Lena", im);
		imshow("Lena", im);
		BOOST_LOG_TRIVIAL(info) << "Hello Leno :)";
	}
	else {
		BOOST_LOG_TRIVIAL(warning) << "Lena could not be found! Go and find her!";
	}
}

int WithoutThread() {
	BOOST_LOG_TRIVIAL(info) << "No threads were used";

	cvNamedWindow("Camera stream", CV_WINDOW_AUTOSIZE);

	CvCapture* capture = cvCaptureFromCAM(1);

	if (!capture){
		BOOST_LOG_TRIVIAL(error) << "No camera found";
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

	BOOST_LOG_TRIVIAL(info) << "bye bye";

	//Release capture.
	cvReleaseCapture(&capture);
	//Destroy Window
	destroyAllWindows();

	return 0;
}

int WithThread()
{
	BOOST_LOG_TRIVIAL(info) << "Threads were used";

	controller::image_processing::ImageProcessingController imgProcCtrl(std::make_shared<services::webcam::WebcamService>());
	imgProcCtrl.StartImageProcessing();

	controller::video_streaming::VideoStreamingController vidStreamCtrl(std::make_shared<services::webcam::WebcamService>());
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

int DetectWihoutServices(){

	VideoCapture cap(0); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		BOOST_LOG_TRIVIAL(error) << "Cannot open the web cam";
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
			BOOST_LOG_TRIVIAL(warning) << "Cannot read a frame from video stream";
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
			BOOST_LOG_TRIVIAL(info) << "esc key is pressed by user";
			break;
		}
	}

	return 0;





}