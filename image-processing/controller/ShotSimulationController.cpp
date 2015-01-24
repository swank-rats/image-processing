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
using Poco::FastMutex;

namespace controller {
	namespace shot_simulation {
		ShotSimulationController::ShotSimulationController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> websocketController) :
			webCamSrv(webcamService), shotSimulation(webcamService, playerHitQueue), websocketController(websocketController),
			playerHitActiviity(this, &ShotSimulationController::HandlePlayerHitNotification) {
			//TODO THOMAS Init players
			playerRect.form = "square";
			playerRect.playerId = 0;
			playerPent.form = "circle";
			playerPent.playerId = 1;
			detectionService = new ObjectDetectionService();
		}

		ShotSimulationController::~ShotSimulationController() {
			playerHitQueue.clear();
			playerHitQueue.wakeUpAll();

			//do not delete, since it is a shared pointer
			websocketController = nullptr;

			delete detectionService;
			detectionService = nullptr;

#if defined(STANDALONE)
			timer.stop();
#endif
		}

		void ShotSimulationController::StartSimulationService() {
			shotSimulation.Start();

			if (!playerHitActiviity.isRunning()) {
				playerHitActiviity.start();
			}
		}

		void ShotSimulationController::StopSimulationService() {
			shotSimulation.Stop();

			if (playerHitActiviity.isRunning()) {
				playerHitActiviity.stop();
				playerHitQueue.wakeUpAll();
				playerHitActiviity.wait();
			}
		}

		void ShotSimulationController::HandleMessageNotification(MessageNotification* notification) {
			Message *message = notification->GetData();

			switch (message->GetCmd()) {
			case MessageCommandEnum::shot:
				string form;
				if (message->GetParam("form", form)) {
					StartShotSimulation(form);
				}
				break;
			}

			notification->release();
		}

		void ShotSimulationController::HandlePlayerHitNotification() {
			while(!playerHitActiviity.isStopped())
			{
				Notification::Ptr notification(playerHitQueue.waitDequeueNotification());
				if (notification)
				{
					PlayerHitNotification::Ptr playerHitNotification = notification.cast<PlayerHitNotification>();
					if (playerHitNotification)
					{
						Message* msg = new Message(MessageCommandEnum::hit, "server");
						msg->AddParam("form", playerHitNotification->GetHitPlayer().form);
						msg->AddParam("precision", std::to_string(playerHitNotification->GetPrecision()));

						websocketController->Send(msg);
					}
				}
				else {
					//null signals that worker should stop polling queue
					playerHitActiviity.stop();
					break;
				} 
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

			Shot shot = detectionService->DetectShotRoute(webCamSrv->GetLastImage(), shootingPlayer,hitPlayer);
			
			shotSimulation.SimulateShot(shot);
		}

		// ONLY FOR TESTING PURPOSE
		void ShotSimulationController::OnTimer(Timer& timer) {
			//StartShotSimulation("pentagon");
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