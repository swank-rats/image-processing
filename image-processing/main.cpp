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

#include <Poco\Logger.h>
#include <Poco\AutoPtr.h>
#include <Poco\WindowsConsoleChannel.h>
#include <Poco\FormattingChannel.h>
#include <Poco\PatternFormatter.h>
#include <Poco\Util\Application.h>

#include "services\videostreaming\webcam\WebcamService.h"
#include "services\ObjectDetectionService.h"
#include "controller\ImageProcessingController.h"
#include "controller\VideoStreamingController.h"
#include "controller\WebSocketController.h"

#include "infrastructure\networking\websockets\client\WebSocketClient.h"

using namespace cv;
using namespace std;
using Poco::Logger;
using Poco::AutoPtr;
using Poco::WindowsConsoleChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Timestamp;
using Poco::ThreadPool;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;

static Mat srcdetect2;
static Mat src_graydetect2;
static int threshdetect2;
static int max_threshdetect2;
static RNG rngdetect2;

/**
* @function thresh_callback
*/
static void thresh_callbackdetect2(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(src_graydetect2, canny_output, threshdetect2, threshdetect2 * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255));
		drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
	}

	/// Show in a window
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}


/**
* @function thresh_callback
*/
static void thresh_callbackdetect3(int, void*)
{
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	Canny(src_graydetect2, canny_output, threshdetect2, threshdetect2 * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	std::vector<cv::Point> approx;

	for (size_t i = 0; i < contours.size(); i++)
	{

		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		if (approx.size() == 4)
		{

			Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255));
			drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());

		}


	}

	/// Show in a window
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
}

class ImageProcessingServerApplication : public Poco::Util::Application {
public:
	ImageProcessingServerApplication() : helpRequested(false)
	{
	}
protected:
	void initialize(Application& self) {
		threshdetect2 = 100;
		max_threshdetect2 = 255;
		rngdetect2(12345);
	}

	void reinitialize() {

	}

	void uninitialize() {

	}

	void defineOptions() {

	}

	int main(const std::vector<std::string>& args)
	{
		InitLoggers();
		Logger& logger = Logger::get("main");
		ShowLena(logger);

		//return DetectWihoutServices(logger);
		//return DetectConture(logger);
		return DetectConture2(logger);
		//return DetectConture3(logger);

		//return WithThread(logger);
	}
private:
	bool helpRequested;

	enum Farbe { ROT = 0, GRUEN = 1, BLAU = 2 };

	void InitLoggers() {
		//setup logger
		AutoPtr<WindowsConsoleChannel> pChannel(new WindowsConsoleChannel);
		AutoPtr<PatternFormatter> pPF(new PatternFormatter);
		pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s (%p): %t");
		AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pChannel));
		Logger::root().setChannel(pFC);
	}

	void ShowLena(Logger& logger) {
		Mat im = imread("resources/images/lena.bmp", 1);
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

		controller::websocket::WebSocketController webSocketCtrl;
		webSocketCtrl.StartWebSocketServer();

		infrastructure::websocket::WebSocketClient webSocketClient;
		webSocketClient.OpenConnection();

		char key;
		while (1) {
			key = cvWaitKey(10);

			if (char(key) == 27){
				break; //If you hit ESC key loop will break.
			}
		}

		imgProcCtrl.StopImageProcessing();
		vidStreamCtrl.StopStreamingServer();
		webSocketCtrl.StopWebSocketServer();

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

	int DetectConture(Logger& logger)
	{

		CvCapture* capture = cvCaptureFromCAM(0);
		IplImage* img;
		CvMemStorage *storage;
		IplImage* imgGrayScale;

		while (true)
		{
			img = cvQueryFrame(capture);

			//show the original image
			cvNamedWindow("Raw");
			cvShowImage("Raw", img);

			//converting the original image into grayscale
			imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1);
			cvCvtColor(img, imgGrayScale, CV_BGR2GRAY);

			//thresholding the grayscale image to get better results
			cvThreshold(imgGrayScale, imgGrayScale, 128, 255, CV_THRESH_BINARY);

			CvSeq* contours;  //hold the pointer to a contour in the memory block
			CvSeq* result;   //hold sequence of points of a contour
			storage = cvCreateMemStorage(0); //storage area for all contours

			//finding all contours in the image
			cvFindContours(imgGrayScale, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));


			//iterating through each contour
			while (contours)
			{
				//obtain a sequence of points of contour, pointed by the variable 'contour'
				result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);

				//if there are 3  vertices  in the contour(It should be a triangle)
				if (result->total == 3)
				{
					//iterating through each point
					CvPoint *pt[3];
					for (int i = 0; i < 3; i++){
						pt[i] = (CvPoint*)cvGetSeqElem(result, i);
					}

					//drawing lines around the triangle
					cvLine(img, *pt[0], *pt[1], cvScalar(255, 0, 0), 4);
					cvLine(img, *pt[1], *pt[2], cvScalar(255, 0, 0), 4);
					cvLine(img, *pt[2], *pt[0], cvScalar(255, 0, 0), 4);

				}

				//if there are 4 vertices in the contour(It should be a quadrilateral)
				else if (result->total == 4)
				{
					//iterating through each point
					CvPoint *pt[4];
					for (int i = 0; i < 4; i++){
						pt[i] = (CvPoint*)cvGetSeqElem(result, i);
					}

					//drawing lines around the quadrilateral
					cvLine(img, *pt[0], *pt[1], cvScalar(0, 255, 0), 4);
					cvLine(img, *pt[1], *pt[2], cvScalar(0, 255, 0), 4);
					cvLine(img, *pt[2], *pt[3], cvScalar(0, 255, 0), 4);
					cvLine(img, *pt[3], *pt[0], cvScalar(0, 255, 0), 4);
				}

				//if there are 7  vertices  in the contour(It should be a heptagon)
				else if (result->total == 7)
				{
					//iterating through each point
					CvPoint *pt[7];
					for (int i = 0; i < 7; i++){
						pt[i] = (CvPoint*)cvGetSeqElem(result, i);
					}

					//drawing lines around the heptagon

					cvLine(img, *pt[0], *pt[1], cvScalar(0, 0, 255), 4);
					cvLine(img, *pt[1], *pt[2], cvScalar(0, 0, 255), 4);
					cvLine(img, *pt[2], *pt[3], cvScalar(0, 0, 255), 4);
					cvLine(img, *pt[3], *pt[4], cvScalar(0, 0, 255), 4);
					cvLine(img, *pt[4], *pt[5], cvScalar(0, 0, 255), 4);
					cvLine(img, *pt[5], *pt[6], cvScalar(0, 0, 255), 4);
					cvLine(img, *pt[6], *pt[0], cvScalar(0, 0, 255), 4);
				}

				//obtain the next contour
				contours = contours->h_next;
			}

			Mat img2 = img;
			//morphological opening (removes small objects from the foreground)
			erode(img2, img2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(img2, img2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//morphological closing (removes small holes from the foreground)
			dilate(img2, img2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(img2, img2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//show the image in which identified shapes are marked   
			cvNamedWindow("Tracked");
			//cvShowImage("Tracked", img2);
			imshow("Tracked", img2);


			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			{
				logger.information("esc key is pressed by user");
				break;
			}
		}

		//cleaning up
		cvDestroyAllWindows();
		cvReleaseMemStorage(&storage);
		cvReleaseImage(&img);
		cvReleaseImage(&imgGrayScale);

		return 0;
	}

	int DetectConture2(Logger& logger){


		CvCapture* capture = cvCaptureFromCAM(0);


		while (true)
		{

			srcdetect2 = cvQueryFrame(capture);


			/// Convert image to gray and blur it
			cvtColor(srcdetect2, src_graydetect2, COLOR_BGR2GRAY);
			blur(src_graydetect2, src_graydetect2, Size(3, 3));

			/// Create Window
			const char* source_window = "Source";
			namedWindow(source_window, WINDOW_AUTOSIZE);
			imshow(source_window, srcdetect2);

			createTrackbar(" Canny thresh:", "Source", &threshdetect2, max_threshdetect2, thresh_callbackdetect2);
			/*thresh_callbackdetect2*/(0, 0);
			thresh_callbackdetect3(0, 0);



			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			{
				logger.information("esc key is pressed by user");
				break;
			}
		}

		//cleaning up
		cvDestroyAllWindows();

		return 0;
	}

	int DetectConture3(Logger& logger){


		CvCapture* capture = cvCaptureFromCAM(0);


		while (true)
		{

			Mat src = cvQueryFrame(capture);

			// Convert to grayscale
			cv::Mat gray;
			cv::cvtColor(src, gray, CV_BGR2GRAY);

			// Use Canny instead of threshold to catch squares with gradient shading
			cv::Mat bw;
			cv::Canny(gray, bw, 0, 50, 5);

			// Find contours
			std::vector<std::vector<cv::Point> > contours;
			cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

			std::vector<cv::Point> approx;
			cv::Mat dst = src.clone();

			for (int i = 0; i < contours.size(); i++)
			{
				// Approximate contour with accuracy proportional
				// to the contour perimeter
				cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

				// Skip small or non-convex objects 
				if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
					continue;

				if (approx.size() == 3)
				{
					setLabel(dst, "TRI", contours[i]);    // Triangles
				}
				else if (approx.size() >= 4 && approx.size() <= 6)
				{
					// Number of vertices of polygonal curve
					int vtc = approx.size();

					// Get the cosines of all corners
					std::vector<double> cos;
					for (int j = 2; j < vtc + 1; j++)
						cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));

					// Sort ascending the cosine values
					std::sort(cos.begin(), cos.end());

					// Get the lowest and the highest cosine
					double mincos = cos.front();
					double maxcos = cos.back();

					// Use the degrees obtained above and the number of vertices
					// to determine the shape of the contour
					if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
						setLabel(dst, "RECT", contours[i]);
					else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
						setLabel(dst, "PENTA", contours[i]);
					else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
						setLabel(dst, "HEXA", contours[i]);
				}
				else
				{
					// Detect and label circles
					double area = cv::contourArea(contours[i]);
					cv::Rect r = cv::boundingRect(contours[i]);
					int radius = r.width / 2;

					if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
						std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
						setLabel(dst, "CIR", contours[i]);
				}
			}

			cv::imshow("src", src);
			cv::imshow("dst", dst);
			//cv::waitKey(0);


			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			{
				logger.information("esc key is pressed by user");
				break;
			}
		}

		//cleaning up
		cvDestroyAllWindows();

		return 0;
	}



	/**
	* Helper function to find a cosine of angle between vectors
	* from pt0->pt1 and pt0->pt2
	*/
	static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
	{
		double dx1 = pt1.x - pt0.x;
		double dy1 = pt1.y - pt0.y;
		double dx2 = pt2.x - pt0.x;
		double dy2 = pt2.y - pt0.y;
		return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
	}

	/**
	* Helper function to display text in the center of a contour
	*/
	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
	{
		int fontface = cv::FONT_HERSHEY_SIMPLEX;
		double scale = 0.4;
		int thickness = 1;
		int baseline = 0;

		cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
		cv::Rect r = cv::boundingRect(contour);

		cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
		cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
		cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
	}
};

POCO_APP_MAIN(ImageProcessingServerApplication);