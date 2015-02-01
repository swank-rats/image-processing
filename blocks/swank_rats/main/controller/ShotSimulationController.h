//============================================================================
// Name        : ShotSimulationController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "WebSocketController.h"

#include "..\shared\notifications\MessageNotification.h"
#include "..\services\simulation\ShotSimulationService.h"
#include "..\services\ObjectDetectionService.h"
#include "..\shared\model\Player.h"

#include "fenix/poco/Foundation/include/Poco/Activity.h"
#include "fenix/poco/Foundation/include/Poco/AutoPtr.h"
#include "fenix/poco/Foundation/include/Poco/SharedPtr.h"
#include "fenix/poco/Foundation/include/Poco/NotificationQueue.h"
#include "fenix/poco/Foundation/include/Poco/Timer.h"

using Poco::Activity;
using Poco::Timer;
using Poco::AutoPtr;
using Poco::SharedPtr;
using Poco::NotificationQueue;
using services::simulation::ShotSimulationService;
using shared::notifications::MessageNotification;
using controller::websocket::WebSocketController;
using services::object_detection::ObjectDetectionService;
using shared::model::Player;

namespace controller {
	namespace shot_simulation {
		class ShotSimulationController
		{
		public:
			ShotSimulationController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> websocketController);
			~ShotSimulationController();
			void HandleMessageNotification(MessageNotification* notification);
			void StartSimulationService();
			void StopSimulationService();
			void StartTestingSimulation(); //only for testing
		private:
			SharedPtr<WebSocketController> websocketController;
			SharedPtr<WebcamService> webCamSrv;
			ShotSimulationService shotSimulation;
			NotificationQueue playerHitQueue;
			ObjectDetectionService *detectionService;
			Player playerRect; //rectangle
			Player playerPent; //pentagon
			Activity<ShotSimulationController> playerHitActiviity;

			Timer timer; //only for testing

			void HandlePlayerHitNotification();
			void StartShotSimulation(string playerType);
			void OnTimer(Timer& timer);
		};
	}
}