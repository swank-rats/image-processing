//============================================================================
// Name        : ShotSimulationController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ShotSimulationController.h"

#include "..\shared\model\message\MessageCommands.h"

#include <Poco\Delegate.h>

using shared::model::message::MessageCommandEnum;
using Poco::TimerCallback;
using Poco::FastMutex;
using Poco::Delegate;

namespace controller {
	namespace shot_simulation {
		ShotSimulationController::ShotSimulationController(SharedPtr<WebcamService> webcamService, SharedPtr<WebSocketController> webSocketController) :
			webCamSrv(webcamService), webSocketController(webSocketController) {
			playerRect.form = "square";
			playerRect.playerId = 0;
			playerPent.form = "circle";
			playerPent.playerId = 1;
			detectionService = new ObjectDetectionService();

			shotSimulation = new ShotSimulationService(webcamService, *detectionService);

			webSocketController->MessageReceived += Poco::delegate(this, &ShotSimulationController::HandleMessage);
			shotSimulation->PlayerHit += Poco::delegate(this, &ShotSimulationController::HandlePlayerHit);
		}

		ShotSimulationController::~ShotSimulationController() {
			playerHitQueue.clear();
			playerHitQueue.wakeUpAll();

			webSocketController->MessageReceived -= Poco::delegate(this, &ShotSimulationController::HandleMessage);
			shotSimulation->PlayerHit -= Poco::delegate(this, &ShotSimulationController::HandlePlayerHit);

			//do not delete, since it is a shared pointer
			webSocketController = nullptr;

			delete detectionService;
			detectionService = nullptr;

			delete shotSimulation;
			shotSimulation = nullptr;

#if defined(STANDALONE)
			timer.stop();
#endif
		}

		void ShotSimulationController::StartSimulationService() {
			Logger& logger = Logger::get("ShotSimulationController");

			//wait till webcam is available
			while (!webCamSrv->IsRecording()) {
				logger.information("webcam service not yet available...");
				Thread::sleep(20);
			}

			logger.information("waiting for webcam service...");
			Thread::sleep(2000);

			Mat frame = webCamSrv->GetLastImage();
			namedWindow("RobotDetection");

			while (frame.empty() || !detectionService->InitalDetection(frame)) {
				logger.information("detection failed - retry...");
				imshow("RobotDetection", frame);
				frame = webCamSrv->GetLastImage();
			}

			destroyWindow("RobotDetection");

			logger.information("robots detected successfully!");
			shotSimulation->Start();
		}

		void ShotSimulationController::StopSimulationService() {
			shotSimulation->Stop();
		}

		void ShotSimulationController::HandleMessage(const void* pSender, const Message& message) {
			switch (message.GetCmd()) {
			case MessageCommandEnum::shot:
				string form;
				if (message.GetParam("form", form)) {
					StartShotSimulation(form);
				}
				break;
			}
		}

		void ShotSimulationController::HandlePlayerHit(const void* pSender, const PlayerHitArgs& arg) {
			Message* msg = new Message(MessageCommandEnum::hit, "server");
			msg->AddParam("form", arg.GetHitPlayer().form);
			msg->AddParam("precision", std::to_string(arg.GetPrecision()));

			webSocketController->Send(msg);
		}

		void ShotSimulationController::StartShotSimulation(string playerType) {
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

			shotSimulation->SimulateShot(shot);
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