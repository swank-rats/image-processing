//============================================================================
// Name        : ShotSimulationController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ShotSimulationController.h"

#include "..\shared\model\message\MessageCommands.h"
#include "..\shared\notifications\PlayerHitNotification.h"

#include <Poco\Delegate.h>

using shared::notifications::PlayerHitNotification;
using shared::model::message::MessageCommandEnum;
using Poco::TimerCallback;
using Poco::FastMutex;
using Poco::Delegate;

namespace controller {
	namespace shot_simulation {
		ShotSimulationController::ShotSimulationController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> webSocketController) :
			webCamSrv(webcamService), shotSimulation(webcamService, playerHitQueue), webSocketController(webSocketController),
			playerHitActiviity(this, &ShotSimulationController::HandlePlayerHitNotification) {
			//TODO THOMAS Init players
			playerRect.form = "square";
			playerRect.playerId = 0;
			playerPent.form = "circle";
			playerPent.playerId = 1;
			detectionService = new ObjectDetectionService();

			webSocketController->MessageReceived += Poco::delegate(this, &ShotSimulationController::HandleMessage);
		}

		ShotSimulationController::~ShotSimulationController() {
			playerHitQueue.clear();
			playerHitQueue.wakeUpAll();

			webSocketController->MessageReceived -= Poco::delegate(this, &ShotSimulationController::HandleMessage);
			//do not delete, since it is a shared pointer
			webSocketController = nullptr;

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

		void ShotSimulationController::HandleMessage(const void* pSender, Message& message) {
			switch (message.GetCmd()) {
			case MessageCommandEnum::shot:
				string form;
				if (message.GetParam("form", form)) {
					StartShotSimulation(form);
				}
				break;
			}
		}

		void ShotSimulationController::HandlePlayerHitNotification() {
			while (!playerHitActiviity.isStopped())
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

						webSocketController->Send(msg);
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
			if (playerType == "square") {
				shootingPlayer = playerRect;
				hitPlayer = playerPent;
			}
			else
			{
				shootingPlayer = playerPent;
				hitPlayer = playerRect;
			}

			Shot shot = detectionService->DetectShotRoute(webCamSrv->GetLastImage(), shootingPlayer, hitPlayer);

			shotSimulation.SimulateShot(shot);
		}

		// ONLY FOR TESTING PURPOSE
		void ShotSimulationController::OnTimer(Timer& timer) {
			StartShotSimulation("circle");
			//StartShotSimulation("square");
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