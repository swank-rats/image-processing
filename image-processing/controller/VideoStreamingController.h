//============================================================================
// Name        : VideoStreamingController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include "..\infrastructure\networking\videostreaming\VideoStreamingServer.h"
#include "..\services\videostreaming\webcam\WebcamService.h"

using namespace services::webcam;
using namespace infrastructure::video_streaming;

namespace controller {
	namespace video_streaming {

		class VideoStreamingController
		{
		public:
			VideoStreamingController(WebcamServicePtr webcamService);
			~VideoStreamingController();
			void StartStreamingServer();
			bool StopStreamingServer();
		private:
			VideoStreamingServer* streamingServer;
			WebcamServicePtr webcamService;
		};
	}
}