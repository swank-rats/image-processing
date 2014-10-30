//============================================================================
// Name        : WebcamService.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================

#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <boost\thread.hpp>
#include "..\..\..\shared\observer\Observable.h"

namespace services {
	namespace webcam {
		class WebcamService : public Observable<WebcamService>, public std::enable_shared_from_this < WebcamService > {
		public:
			WebcamService();

			/*
			 * This method will start the recording from the webcam
			 * Returns true if starting was successful, false otherwise
			 */
			bool StartRecording();
			/*
			 * This method will stop the recodirng from the webcam
			 */
			bool StopRecording();

			cv::Mat GetLastImage();

		private:
			cv::VideoCapture capture;
			cv::Mat lastImage;
			boost::thread recordingThread;

			void RecordingCore();
		};

		typedef std::shared_ptr<WebcamService> WebcamServicePtr;
	}
}