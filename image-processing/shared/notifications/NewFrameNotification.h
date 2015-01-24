//============================================================================
// Name        : NewFrameNotification.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include <Poco\AutoPtr.h>
#include <Poco\Notification.h>
#include <opencv2\core\core.hpp>

using Poco::AutoPtr;
using Poco::Notification;
using cv::Mat;

namespace shared {
	namespace notifications {
		class NewFrameNotification : public Notification
		{
		public:
			typedef AutoPtr<NewFrameNotification> Ptr;

			NewFrameNotification(Mat& frame);
			~NewFrameNotification();

			Mat& GetFrame();
		private:
			Mat frame;
		};
	}
}

