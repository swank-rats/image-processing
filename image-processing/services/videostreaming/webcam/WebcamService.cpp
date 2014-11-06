//============================================================================
// Name        : WebcamService.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "WebcamService.h"

namespace services {
	namespace webcam {
		static const char* windowName = "Webcam stream";

		WebcamService::WebcamService() : capture(VideoCapture()), recordingActivity(this, &WebcamService::RecordingCore) {
			//capture.set()
		}

		bool WebcamService::StartRecording() {
			Logger& logger = Logger::get("WebcamService");

			logger.information("starting recording...");
			cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

			capture.open(CV_CAP_ANY);

			if (!capture.isOpened()){
				logger.error("No camera found");
				return false;
			}

			recordingActivity.start();

			logger.information("started recording");

			return true;
		}

		bool WebcamService::StopRecording() {
			Logger& logger = Logger::get("WebcamService");

			logger.information("stopping recording...");

			if (recordingActivity.isRunning()) {
				recordingActivity.stop();
				logger.information("recording activity stop requested");
				recordingActivity.wait();
				logger.information("recording activity stopped successfully");

			}
			else {
				logger.error("recording activity is already stopped!");
			}
			
			logger.information("stopped recording");

			return true;
		}

		void WebcamService::RecordingCore() {
			Logger& logger = Logger::get("WebcamService");

			cv::Mat frame;
				
			while (!recordingActivity.isStopped()) {
				if (!capture.isOpened()) {
					logger.error("Lost connection to webcam!");
					break;
				}
				//Create image frames from capture
				capture >> frame;

				if (!frame.empty()) {
					//Show image frames on created window
					cv::imshow(windowName, frame);
					//Clone image
					rwLock.writeLock(); //make this operation thread safe
					lastImage = frame.clone();
					rwLock.unlock();

					Notify();
				}
				else {
					logger.warning("Captured empty webcam frame!");
				}
			}
		}

		cv::Mat WebcamService::GetLastImage() {
			Poco::ScopedReadRWLock readLock(rwLock);
			return lastImage;
		}
	}
}