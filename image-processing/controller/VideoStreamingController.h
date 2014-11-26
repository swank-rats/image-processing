//============================================================================
// Name        : VideoStreamingController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "..\infrastructure\networking\videostreaming\VideoStreamingServer.h"
#include "..\services\webcam\WebcamService.h"

#include <Poco\SharedPtr.h>

using namespace services::webcam;
using namespace infrastructure::video_streaming;

using Poco::SharedPtr;

namespace controller {
	namespace video_streaming {
		class VideoStreamingController
		{
		public:
			VideoStreamingController(SharedPtr<WebcamService> webcamService);
			~VideoStreamingController();
			void StartStreamingServer();
			bool StopStreamingServer();
		private:
			VideoStreamingServer* streamingServer;
		};
	}
}