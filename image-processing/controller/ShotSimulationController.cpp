//============================================================================
// Name        : ShotSimulationController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ShotSimulationController.h"

#include "..\shared\model\message\MessageCommands.h"
#include "..\shared\notifications\PlayerHitNotification.h"

using shared::notifications::PlayerHitNotification;
using shared::model::message::MessageCommandEnum;
using Poco::TimerCallback;

namespace controller {
	namespace shot_simulation {
		ShotSimulationController::ShotSimulationController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> websocketController) :
			webCamSrv(webcamService), playerHitQueue(), shotSimulation(webcamService, playerHitQueue), websocketController(websocketController) {
			//TODO THOMAS Init players
			playerRect.form = "square";
			playerRect.playerId = 0;
			playerPent.form = "pentagon";
			playerPent.playerId = 1;
			detectionService = new ObjectDetectionService();
		}


		ShotSimulationController::~ShotSimulationController() {
			//do not delete, since it is a shared pointer
			websocketController = nullptr;

			delete detectionService;
			detectionService = nullptr;

#if defined(STANDALONE)
			timer.stop();
#endif
		}


		void ShotSimulationController::HandleMessageNotification(MessageNotification* notification) {
			const Message &message = notification->GetData();

			switch (message.GetCmd()) {
			case MessageCommandEnum::shot:
				string form;
				if (message.GetParam("form", form)) {
					StartShotSimulation(form);
				}
			break;
			}

			notification->release();
		}

		void ShotSimulationController::HandlePlayerHitNotification() {
			AutoPtr<Notification> notification(playerHitQueue.waitDequeueNotification());

			while (notification) {

				PlayerHitNotification* playerHitNotification = dynamic_cast<PlayerHitNotification*>(notification.get());
				if (playerHitNotification)
				{
					Message* msg = new Message(MessageCommandEnum::hit, "server");
					msg->AddParam("player", std::to_string(playerHitNotification->GetHitPlayer().playerId));
					msg->AddParam("precision", std::to_string(playerHitNotification->GetPrecision()));
					websocketController->Send(msg);
				}

				notification = playerHitQueue.waitDequeueNotification();
			}
		}

		void ShotSimulationController::StartShotSimulation(string playerType) {
			//TODO THOMAS
			Player shootingPlayer;
			Player hitPlayer;
			if (playerType == "rectangle") {
				shootingPlayer = playerRect;
				hitPlayer = playerPent;
			}
			else
			{
				shootingPlayer = playerPent;
				hitPlayer = playerRect;
			}

			Shot shot = detectionService->DetectShotRoute(webCamSrv->GetLastImage(), shootingPlayer);
			shot.hitPlayer = hitPlayer;
			shotSimulation.SimulateShot(shot);
		}

		// ONLY FOR TESTING PURPOSE
		void ShotSimulationController::OnTimer(Timer& timer) {
			StartShotSimulation("rectangle");
		}

		void ShotSimulationController::StartTestingSimulation() {
			//automatic shot simulation
			TimerCallback<ShotSimulationController> callback(*this, &ShotSimulationController::OnTimer);
			timer.setStartInterval(5000);
			timer.setPeriodicInterval(10000);
			timer.start(callback);
		}
	}
}