//============================================================================
// Name        : WebcamService.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\shared\observer\Observable.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <Poco\Thread.h>
#include <Poco\Activity.h>
#include <Poco\Logger.h>
#include <Poco\RWLock.h>

#include <memory>

using cv::VideoCapture;
using cv::Mat;
using Poco::Activity;
using Poco::Logger;

namespace services {
	namespace webcam {
		class WebcamService : public Observable < WebcamService > {
		public:
			WebcamService();
			~WebcamService();
			/*
			 * This method will start the recording from the webcam
			 * Returns true if starting was successful, false otherwise
			 */
			bool StartRecording();
			/*
			 * This method will stop the recodirng from the webcam
			 */
			bool StopRecording();

			Mat GetLastImage();

			bool IsRecording();
		private:
			void RecordingCore();

			VideoCapture capture;
			Mat lastImage;
			Activity<WebcamService> recordingActivity;
		};
	}
}