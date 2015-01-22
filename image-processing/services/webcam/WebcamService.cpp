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
		}

		WebcamService::~WebcamService() {
			if (recordingActivity.isRunning()) {
				StopRecording();
			}

			if (capture.isOpened()) {
				capture.release();
			}
		}

		bool WebcamService::StartRecording() {
			Logger& logger = Logger::get("WebcamService");

			capture.open(CV_CAP_ANY);

			if (!capture.isOpened()){
				logger.error("No camera available!");
				return false;
			}

			logger.information("starting recording...");
			cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

			//camera settings
			capture.set(CV_CAP_PROP_FPS, 30);
			//Possible resolutions : 1280x720, 640x480; 440x330
			capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
			capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

			logger.information("Camera settings: ");
			logger.information("FPS: " + std::to_string(capture.get(CV_CAP_PROP_FPS)));
			logger.information("Resolution: " + std::to_string(capture.get(CV_CAP_PROP_FRAME_WIDTH)) + "x" + std::to_string(capture.get(CV_CAP_PROP_FRAME_HEIGHT)));
			logger.information("Codec: " + std::to_string(capture.get(CV_CAP_PROP_FOURCC)));
			logger.information("Format: " + std::to_string(capture.get(CV_CAP_PROP_FORMAT)));

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

		bool WebcamService::IsRecording() {
			return capture.isOpened() && recordingActivity.isRunning();
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
					//cv::imshow(windowName, frame);
					//Clone image
					lastImage = frame;
					//logger.warning("Captured webcam frame!" + std::to_string(frame.size().width) + "x" + std::to_string(frame.size().height));

					Notify();
				}
				else {
					logger.warning("Captured empty webcam frame!");
				}
			}
		}

		cv::Mat WebcamService::GetLastImage() {
			return lastImage;
		}
	}
}