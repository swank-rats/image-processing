//============================================================================
// Name        : WebcamService.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "WebcamService.h"
#include <iostream>
#include <iomanip>

#include <Poco\Clock.h>

#include <Poco\Stopwatch.h>
using Poco::Stopwatch;

using Poco::Clock;
using Poco::Thread;
using std::cout;

namespace services {
	namespace webcam {
		WebcamService::WebcamService() : capture(VideoCapture()), recordingActivity(this, &WebcamService::RecordingCore) {
			isRecording = false;
			params = { CV_IMWRITE_JPEG_QUALITY, 30 };
			fps = 15;
			delay = 1000 / fps;
		}

		WebcamService::~WebcamService() {
			if (recordingActivity.isRunning()) {
				StopRecording();
			}

			if (capture.isOpened()) {
				capture.release();
			}
		}

		int WebcamService::GetDelay() {
			return delay;
		}

		int WebcamService::GetFPS() {
			return fps;
		}

		void WebcamService::SetModifiedImage(Mat& image) {
			Poco::Mutex::ScopedLock lock(modifiedImgMutex); //will be released after leaving scop
			// encode mat to jpg and copy it to content
			cv::imencode(".jpg", image, modifiedImage, params);
		}

		vector<uchar>* WebcamService::GetModifiedImage() {
			Poco::Mutex::ScopedLock lock(modifiedImgMutex); //will be released after leaving scop
			vector<uchar> *tempImg = new vector<uchar>(modifiedImage.begin(), modifiedImage.end());
			return tempImg;
		}

		Mat& WebcamService::GetLastImage() {
			Poco::Mutex::ScopedLock lock(lastImgMutex); //will be released after leaving scop
			return lastImage;
		}

		bool WebcamService::StartRecording() {
			Logger& logger = Logger::get("WebcamService");

			capture.open(CV_CAP_ANY);

			if (!capture.isOpened()){
				logger.error("No camera available!");
				return false;
			}

			logger.information("starting recording...");

			//camera settings
			capture.set(CV_CAP_PROP_FPS, fps);
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
			Mat frame;

			//Stopwatch sw;
			Clock clock;
			int newDelay = 0;

			while (!recordingActivity.isStopped()) {
				if (!capture.isOpened()) {
					logger.error("Lost connection to webcam!");
					break;
				}

				//sw.restart();

				//Create image frames from capture
				capture >> frame;

				clock.update();
				if (!frame.empty()) {
						{
							Poco::Mutex::ScopedLock lock(lastImgMutex); //will be released after leaving scop
							lastImage = frame; //Clone image
						}

					//sw.stop();
					//printf("Capture frame: %f ms\n", sw.elapsed() * 0.001);
					//sw.restart();

					Notify();

					//sw.stop();
					//printf("Notifiy: %f ms\n", sw.elapsed() * 0.001);
				}
				else {
					logger.warning("Captured empty webcam frame!");
				}

				newDelay = delay - clock.elapsed() * 0.001;

				if (newDelay > 0) {
					//webcam can only be queried after some time again
					//according to the FPS rate
					Thread::sleep(newDelay);
				}
			}
		}
	}
}