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
		}

		bool WebcamService::StartRecording() {
			BOOST_LOG_TRIVIAL(info) << "starting recording...";
			cvNamedWindow(windowName, CV_WINDOW_NORMAL);

			capture.open(CV_CAP_ANY);

			if (!capture.isOpened()){
				BOOST_LOG_TRIVIAL(error) << "No camera found";
				return false;
			}

			recordingThread = boost::thread(boost::bind(&WebcamService::RecordingCore, this));

			BOOST_LOG_TRIVIAL(info) << "started recording";

			return true;
		}

		bool WebcamService::StopRecording() {
			BOOST_LOG_TRIVIAL(info) << "stopping recording...";

			if (recordingThread.joinable()) {
				while (!recordingThread.timed_join(boost::posix_time::seconds(2)))
				{
					// Interupt the thread
					BOOST_LOG_TRIVIAL(info) << "recording thread interrupt request sent";
					recordingThread.interrupt();
				}

				BOOST_LOG_TRIVIAL(info) << "recording thread stopped successfully";

			}
			else {
				BOOST_LOG_TRIVIAL(error) << "recording thread is already disposed!";
			}
			
			BOOST_LOG_TRIVIAL(info) << "stopped recording";

			return true;
		}

		void WebcamService::RecordingCore() {
			try {
				cv::Mat frame;
				boost::mutex mutex;

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
						mutex.lock(); //make this operation thread safe
						lastImage = frame.clone();
						mutex.unlock();

						//std::string str;

						//for (int i = 0; i < lastImage.rows; i++)
						//{
						//	for (int j = 0; j < lastImage.cols; j++)
						//	{
						//		str.append(*(uchar*)(lastImage.data + i * lastImage.step + j), 1);
						//	}
						//}

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