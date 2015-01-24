//============================================================================
// Name        : NewFrameNotification.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "NewFrameNotification.h"

namespace shared {
	namespace notifications {
		NewFrameNotification::NewFrameNotification(Mat& frame) {
			this->frame = frame;
		}

		NewFrameNotification::~NewFrameNotification() { }
		
		Mat& NewFrameNotification::GetFrame() {
			return frame;
		}
	}
}
