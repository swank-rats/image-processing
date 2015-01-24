//============================================================================
// Name        : WebcamService.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\shared\observer\Observable.h"

#include <opencv2\core\core.hpp>
#include <opencv2\opencv.hpp>
#include <Poco\Thread.h>
#include <Poco\Activity.h>
#include <Poco\Logger.h>
#include <Poco\RWLock.h>
#include <Poco\Mutex.h>

#include <memory>

using cv::VideoCapture;
using cv::Mat;
using Poco::Activity;
using Poco::Logger;
using Poco::Mutex;

namespace services {
	namespace webcam {
		class WebcamService : public Observable < WebcamService > {
		public:
			WebcamService();
			~WebcamService();

			bool StartRecording();
			bool StopRecording();
			Mat& GetLastImage();
			Mat& GetModifiedImage();
			void SetModifiedImage(Mat image);
			bool IsRecording();
			bool IsModifiedAvailable();
			int GetFPS();
			int GetDelay();
		private:
			bool isRecording;
			bool isModifiedAvailable;
			int fps;
			int delay;
			VideoCapture capture;
			Mat lastImage;
			Mat modifiedImage;
			Activity<WebcamService> recordingActivity;
			Poco::Mutex mutex;

			void RecordingCore();
		};
	}
}