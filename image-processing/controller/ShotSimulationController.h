//============================================================================
// Name        : ShotSimulationController.h
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#pragma once
#include "WebSocketController.h"

#include "..\shared\notifications\MessageNotification.h"
#include "..\shared\events\PlayerHitArgs.h"
#include "..\services\simulation\ShotSimulationService.h"
#include "..\services\ObjectDetectionService.h"
#include "..\shared\model\Player.h"

#include <Poco\Activity.h>
#include <Poco\AutoPtr.h>
#include <Poco\SharedPtr.h>
#include <Poco\NotificationQueue.h>
#include <Poco\Timer.h>

using Poco::Activity;
using Poco::Timer;
using Poco::AutoPtr;
using Poco::SharedPtr;
using Poco::NotificationQueue;
using services::simulation::ShotSimulationService;
using shared::events::PlayerHitArgs;
using shared::notifications::MessageNotification;
using controller::websocket::WebSocketController;
using services::object_detection::ObjectDetectionService;
using shared::model::Player;

namespace controller {
	namespace shot_simulation {
		class ShotSimulationController
		{
		public:
			ShotSimulationController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> webSocketController);
			~ShotSimulationController();
			void StartSimulationService();
			void StopSimulationService();
			void StartTestingSimulation(); //only for testing

		private:
			SharedPtr<WebSocketController> webSocketController;
			SharedPtr<WebcamService> webCamSrv;
			ShotSimulationService *shotSimulation;
			NotificationQueue playerHitQueue;
			ObjectDetectionService *detectionService;
			Player playerRect; //rectangle
			Player playerPent; //pentagon

			Timer timer; //only for testing

			void HandlePlayerHit(const void* pSender, const PlayerHitArgs& arg);
			void HandleMessage(const void* pSender, const Message& message);
			void StartShotSimulation(string playerType);
			void OnTimer(Timer& timer);
		};
	}
}