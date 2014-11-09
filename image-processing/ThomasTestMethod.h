//============================================================================
// Name        : main.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Image process start point
//============================================================================


#include <iostream>
#include <list>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>

#include "services\videostreaming\webcam\WebcamService.h"
#include "services\ObjectDetectionService.h"
#include "controller\ImageProcessingController.h"


#include "Poco/HashMap.h"



using namespace cv;
using namespace std;

static Mat srcdetect2;
static Mat src_graydetect2;
static int threshdetect2;
static int max_threshdetect2;
static RNG rngdetect2;



static vector<int> CheckForAllowedPentagons(vector<vector<cv::Point>> pentagons, vector<vector<cv::Point>> triangles, vector<int> pentagonContourPositions)
{

	int pentagonPos;
	vector<int> allowedPentagonsContourPosition;

	if (pentagons.size() == pentagonContourPositions.size()){


		for (size_t i = 0; i < triangles.size(); i++)
		{
			std::vector<cv::Point> triangleSelected = triangles[i];

			for (size_t a = 0; a < pentagons.size(); a++)
			{

				if (triangleSelected.size() != 3)
					break;

				Point2f triPointOne;
				Point2f triPointTwo;
				Point2f triPointThree;

				triPointOne.x = triangleSelected[0].x;
				triPointOne.y = triangleSelected[0].y;

				triPointTwo.x = triangleSelected[1].x;
				triPointTwo.y = triangleSelected[1].y;

				triPointThree.x = triangleSelected[2].x;
				triPointThree.y = triangleSelected[2].y;

				if (pointPolygonTest(Mat(pentagons[a]), triPointOne, true) > 0 && pointPolygonTest(Mat(pentagons[a]), triPointTwo, true) > 0 && pointPolygonTest(Mat(pentagons[a]), triPointThree, true) > 0){

					allowedPentagonsContourPosition.push_back(pentagonContourPositions[a]);

				}


			}
		}

	}

	return  allowedPentagonsContourPosition;

}


static vector<int> CheckForAllowedRectangles(vector<vector<cv::Point>> rectangles, vector<vector<cv::Point>> triangles, vector<int> rectanglesContourPositions)
{

	int rectPos;
	vector<int> allowedRectanglesContourPositions;

	if (rectangles.size() == rectanglesContourPositions.size()){


		for (size_t i = 0; i < triangles.size(); i++)
		{
			std::vector<cv::Point> triSelected = triangles[i];

			for (size_t a = 0; a < rectangles.size(); a++)
			{

				if (triSelected.size() != 3)
					break;

				Point2f triPointOne;
				Point2f triPointTwo;
				Point2f triPointThree;

				triPointOne.x = triSelected[0].x;
				triPointOne.y = triSelected[0].y;

				triPointTwo.x = triSelected[1].x;
				triPointTwo.y = triSelected[1].y;

				triPointThree.x = triSelected[2].x;
				triPointThree.y = triSelected[2].y;

				if (pointPolygonTest(Mat(rectangles[a]), triPointOne, true) > 0 && pointPolygonTest(Mat(rectangles[a]), triPointTwo, true) > 0 && pointPolygonTest(Mat(rectangles[a]), triPointThree, true) > 0){

					allowedRectanglesContourPositions.push_back(rectanglesContourPositions[a]);

				}


			}


		}


	}

	return allowedRectanglesContourPositions;


}

static void setLabel2(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(0, 0, 0), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(255, 255, 255), thickness, 8);
}

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

static void calcPosition()
{

}

/**
* @function thresh_callback
*/
static void thresh_callbackdetect3(int, void*)
{
	//Drawing and contours 
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// Finding rects,tris and pentagons
	std::vector<cv::Point> approx;

	vector<vector<cv::Point>> rectangles;
	vector<vector<cv::Point>> pentagons;
	vector<int> rectanglesContourPositions;
	vector<int> pentagonsContourPositions;
	vector<vector<cv::Point>> triangles;

	//Found correct elements
	vector<int> allowedRectanglesContourPositions;
	vector<int> allowedPentagonsContourPosition;


	/// Detect edges using canny
	Canny(src_graydetect2, canny_output, threshdetect2, threshdetect2 * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);



	/*
	*	Starting detection process
	*/

	for (size_t i = 0; i < contours.size(); i++)
	{

		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		// Rectangles
		if (approx.size() == 4)
		{
			rectangles.push_back(contours[i]);
			rectanglesContourPositions.push_back(i);
		}

		if (approx.size() == 3)
		{
			triangles.push_back(approx);
		}

		if (approx.size() == 5)
		{
			pentagons.push_back(contours[i]);
			pentagonsContourPositions.push_back(i);
		}
	}


	allowedRectanglesContourPositions = CheckForAllowedRectangles(rectangles, triangles, rectanglesContourPositions);
	allowedPentagonsContourPosition = CheckForAllowedPentagons(pentagons, triangles, pentagonsContourPositions);


	
	for (size_t i = 0; i < contours.size(); i++)
	{
		for (size_t y = 0; y < allowedRectanglesContourPositions.size(); y++)
		{
			if (allowedRectanglesContourPositions[y] == i)
			{

				Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255));
				drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());

				cv::Moments mom;

				mom = cv::moments(cv::Mat(contours[i]));

				cv::Point point = Point(mom.m10 / mom.m00, mom.m01 / mom.m00);
				// draw mass center
				cv::circle(drawing,point
					// position of mass center converted to integer
					,
					2, cv::Scalar(255, 255, 255), 2);// draw white dot

				

				string xAsString = static_cast<ostringstream*>(&(ostringstream() << point.x))->str();
				string yAsString = static_cast<ostringstream*>(&(ostringstream() << point.y))->str();

				std::string zeichenkette= "x: " + xAsString + "y: " + yAsString;

				setLabel2(drawing, zeichenkette, contours[i]);
			
				
			}
		}

		for (size_t y = 0; y < allowedPentagonsContourPosition.size(); y++)
		{
			if (allowedPentagonsContourPosition[y] == i)
			{

				Scalar color = Scalar(rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255), rngdetect2.uniform(0, 255));
				drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
			}
		}

	}





	/// Show in a window
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
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

class ThomasTest{
public:
	enum Farbe { ROT = 0, GRUEN = 1, BLAU = 2 };

	ThomasTest() {
		threshdetect2 = 100;
		max_threshdetect2 = 250;
		rngdetect2(12345);
	}

	void DetectWihoutServices(){

		VideoCapture cap(0); //capture the video from web cam

		if (!cap.isOpened())  // if not success, exit program
		{

			return;
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

				break;
			}
		}
	}

	void DetectConture()
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

				break;
			}
		}

		//cleaning up
		cvDestroyAllWindows();
		cvReleaseMemStorage(&storage);
		cvReleaseImage(&img);
		cvReleaseImage(&imgGrayScale);
	}

	void DetectConture2() {


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

			//createTrackbar(" Canny thresh:", "Source", &threshdetect2, max_threshdetect2, thresh_callbackdetect2);
			//thresh_callbackdetect2(0, 0);

			createTrackbar(" Canny thresh:", "Source", &threshdetect2, max_threshdetect2, thresh_callbackdetect3);
			thresh_callbackdetect3(0, 0);

			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			{

				break;
			}
		}

		//cleaning up
		cvDestroyAllWindows();
	}

	void DetectConture3() {


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

				break;
			}
		}

		//cleaning up
		cvDestroyAllWindows();
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

	void Test4()
	{

		// create a RGB colour image (set it to a black background)

		Mat img = Mat::zeros(400, 400, CV_8UC3);

		// define a polygon (as a vector of points)

		vector<Point> contour;
		contour.push_back(Point(50, 50));
		contour.push_back(Point(300, 50));
		contour.push_back(Point(350, 200));
		contour.push_back(Point(300, 150));
		contour.push_back(Point(150, 350));
		contour.push_back(Point(100, 100));

		// create a pointer to the data as an array of points (via a conversion to 
		// a Mat() object)

		const cv::Point *pts = (const cv::Point*) Mat(contour).data;
		int npts = Mat(contour).rows;

		std::cout << "Number of polygon vertices: " << npts << std::endl;

		// draw the polygon 

		polylines(img, &pts, &npts, 1,
			true, 			// draw closed contour (i.e. joint end to start) 
			Scalar(0, 255, 0),// colour RGB ordering (here = green) 
			3, 		        // line thickness
			CV_AA, 0);


		// do point in polygon test (by conversion/cast to a Mat() object)
		// define and test point one (draw it in red)

		Point2f test_pt;
		test_pt.x = 150;
		test_pt.y = 75;

		rectangle(img, test_pt, test_pt, Scalar(0, 0, 255), 3, 8, 0); // RED point

		if (pointPolygonTest(Mat(contour), test_pt, true) > 0){
			std::cout << "RED {" << test_pt.x << "," << test_pt.y
				<< "} is in the polygon (dist. "
				<< pointPolygonTest(Mat(contour), test_pt, 1) << ")"
				<< std::endl;
		}

		// define and test point two (draw it in blue)

		test_pt.x = 50;
		test_pt.y = 350;

		rectangle(img, test_pt, test_pt, Scalar(255, 0, 0), 3, 8, 0); // BLUE point

		if (pointPolygonTest(Mat(contour), test_pt, true) < 0){
			std::cout << "BLUE {" << test_pt.x << "," << test_pt.y
				<< "} is NOT in the polygon (dist. "
				<< pointPolygonTest(Mat(contour), test_pt, 1) << ")"
				<< std::endl;
		}

		// pointPolygonTest :- 
		// "The function determines whether the point is inside a contour, outside, 
		// or lies on an edge (or coincides with a vertex). It returns positive 
		// (inside), negative (outside) or zero (on an edge) value, correspondingly. 
		// When measureDist=false , the return value is +1, -1 and 0, respectively. 
		// Otherwise, the return value it is a signed distance between the point 
		// and the nearest contour edge." - OpenCV Manual version 2.1

		// create an image and display the image

		namedWindow("Polygon Test", 0);
		imshow("Polygon Test", img);
		waitKey(0);




	}
};