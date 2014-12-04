//============================================================================
// Name        : VideoStreamingController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\infrastructure\networking\videostreaming\VideoStreamingServer.h"
#include "..\services\webcam\WebcamService.h"
#include "..\shared\notifications\MessageNotification.h"

#include <Poco\SharedPtr.h>
#include <Poco\AutoPtr.h>

using namespace services::webcam;
using namespace infrastructure::video_streaming;

using Poco::SharedPtr;
using Poco::AutoPtr;
using shared::notifications::MessageNotification;

namespace controller {
	namespace video_streaming {
		class VideoStreamingController
		{
		public:
			VideoStreamingController(SharedPtr<WebcamService> webcamService);
			~VideoStreamingController();
			void HandleMessageNotification(const AutoPtr<MessageNotification>& notification);
			void StartStreamingServer();
			bool StopStreamingServer();
		private:
			VideoStreamingServer* streamingServer;
		};
	}
}