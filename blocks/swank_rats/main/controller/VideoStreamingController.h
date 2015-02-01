//============================================================================
// Name        : VideoStreamingController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "swank_rats/main/infrastructure/networking/videostreaming/VideoStreamingServer.h"
#include "swank_rats/main/services/webcam/WebcamService.h"
#include "swank_rats/main/shared/notifications/MessageNotification.h"
#include "WebSocketController.h"

#include "fenix/poco/Foundation/include/Poco/SharedPtr.h"
#include "fenix/poco/Foundation/include/Poco/AutoPtr.h"
#include "fenix/poco/Foundation/include/Poco/Activity.h"
#include "fenix/poco/Foundation/include/Poco/NotificationQueue.h"

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