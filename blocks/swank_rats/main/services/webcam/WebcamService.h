//============================================================================
// Name        : WebcamService.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\..\shared\observer\Observable.h"

#include "diego/opencv/opencv2/core/core.hpp"
#include "diego/opencv/opencv2/opencv.hpp"

#include "fenix/poco/Foundation/include/Poco/Thread.h"
#include "fenix/poco/Foundation/include/Poco/Activity.h"
#include "fenix/poco/Foundation/include/Poco/Logger.h"
#include "fenix/poco/Foundation/include/Poco/RWLock.h"
#include "fenix/poco/Foundation/include/Poco/Mutex.h"

#include <memory>
#include <vector>

using cv::VideoCapture;
using cv::Mat;
using std::vector;
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
			vector<uchar>* GetModifiedImage();
			void SetModifiedImage(Mat& image);
			bool IsRecording();
			int GetFPS();
			int GetDelay();
		private:
			bool isRecording;
			bool isModifiedAvailable;
			int fps;
			int delay;
			VideoCapture capture;
			Mat lastImage;
			vector<uchar> modifiedImage;
			Activity<WebcamService> recordingActivity;
			Poco::Mutex lastImgMutex;
			Poco::Mutex modifiedImgMutex;
			vector<int> params;

			void RecordingCore();
		};
	}
}