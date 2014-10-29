//============================================================================
// Name        : WebcamService.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include <boost/log/trivial.hpp>
#include "WebcamService.h"

namespace services {
	namespace webcam {
		static const char* windowName = "Webcam stream";

		WebcamService::WebcamService() : capture(cv::VideoCapture()) {
		}

		WebcamService::~WebcamService() {
			recordingThread = nullptr;
		}

		bool WebcamService::StartRecording() {
			BOOST_LOG_TRIVIAL(info) << "starting recording...";
			cvNamedWindow(windowName, CV_WINDOW_NORMAL);

			capture.open(CV_CAP_ANY);

			if (!capture.isOpened()){
				BOOST_LOG_TRIVIAL(error) << "No camera found";
				return false;
			}

			recordingThread = new boost::thread(boost::bind(&WebcamService::RecordingCore, this));

			BOOST_LOG_TRIVIAL(info) << "started recording";

			return true;
		}

		bool WebcamService::StopRecording() {
			BOOST_LOG_TRIVIAL(info) << "stopping recording...";

			if (recordingThread != NULL) {
				while (!recordingThread->timed_join(boost::posix_time::seconds(2)))
				{
					// Interupt the thread
					BOOST_LOG_TRIVIAL(info) << "recording thread interrupt request sent";
					recordingThread->interrupt();
				}

				BOOST_LOG_TRIVIAL(info) << "recording thread stopped successfully";

			}
			else {
				BOOST_LOG_TRIVIAL(error) << "recording thread is already disposed!";
			}

			////release resources
			//if (capture.isOpened()) {
			//	//Release capture.
			//	cvReleaseCapture(capture);
			//}
			// VideoCapture will be closed in the destructor
			
			BOOST_LOG_TRIVIAL(info) << "stopped recording";

			return true;
		}

		void WebcamService::RecordingCore() {
			cv::Mat frame;

			try {
				while (1) {
					if (!capture.isOpened()) {
						BOOST_LOG_TRIVIAL(error) << "Lost connection to webcam!";
						break;
					}
					//Create image frames from capture
					capture >> frame;

					if (!frame.empty()) {
						//Show image frames on created window
						cv::imshow(windowName, frame);
						//Clone image
						lastImage = frame.clone();

						Notify();
					}
					else {
						BOOST_LOG_TRIVIAL(warning) << "Captured empty webcam frame!";
					}

					//here is the place where the thread can be interrupted with join
					boost::this_thread::interruption_point();
				}
			}
			catch (boost::thread_interrupted) {
				BOOST_LOG_TRIVIAL(warning) << "recording thread stopped by interrupt";
			}
		}

		cv::Mat WebcamService::GetLastImage() {
			return lastImage.clone();
		}
	}
}