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
#include "WebSocketController.h"

#include <Poco\SharedPtr.h>
#include <Poco\AutoPtr.h>
#include <Poco\Activity.h>
#include <Poco\NotificationQueue.h>

using namespace services::webcam;
using namespace infrastructure::video_streaming;

using Poco::NotificationQueue;
using Poco::SharedPtr;
using Poco::AutoPtr;
using Poco::Activity;
using shared::notifications::MessageNotification;
using controller::websocket::WebSocketController;

namespace controller {
	namespace video_streaming {
		class VideoStreamingController
		{
		public:
			VideoStreamingController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> webSocketController);
			~VideoStreamingController();
			void HandleMessageNotification(MessageNotification* notification);
			void StartStreamingServer();
			bool StopStreamingServer();
		private:
			VideoStreamingServer* streamingServer;
			NotificationQueue lostConnectionQueue;
			SharedPtr<WebSocketController> webSocketController;
			Activity<VideoStreamingController> lostConnectionHandlerActivity;

			void HandleLostConnections();
		};
	}
}