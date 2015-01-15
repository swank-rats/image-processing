//============================================================================
// Name        : ImageProcessingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ImageProcessingController.h"

#include "..\services\ObjectDetectionService.h"
#include "..\shared\notifications\PlayerHitNotification.h"
#include "..\shared\model\Player.h"
#include "..\shared\model\message\MessageCommands.h"

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>
#include <algorithm>

using std::max;
using cv::Mat;
using cv::Rect;
using cv::Point;
using Poco::TimerCallback;
using shared::notifications::PlayerHitNotification;
using shared::model::Player;
using shared::model::message::MessageCommandEnum;

namespace controller {
	namespace image_processing {
		int iLowH = 0;
		int iHighH = 179;

		int iLowS = 0;
		int iHighS = 255;

		int iLowV = 0;
		int iHighV = 255;

		ImageProcessingController::ImageProcessingController(SharedPtr<WebcamService> webcamService,
			SharedPtr<WebSocketController> websocketController) : webcamService(webcamService), playerHitQueue(),
			shotSimulation(webcamService, playerHitQueue), websocketController(websocketController),
			playerHitActivity(this, &ImageProcessingController::HandlePlayerHitNotification) {
			detectService = new ObjectDetectionService();
		}

		ImageProcessingController::~ImageProcessingController()	{
			delete detectService;
			detectService = nullptr;

			//do not delete, since it is a shared pointer
			webcamService = nullptr;
			websocketController = nullptr;
		}

		void ImageProcessingController::StartImageProcessing() {
			//CreateTrackBarView();

			webcamService->StartRecording();
			playerHitActivity.start();
		}

		void ImageProcessingController::StopImageProcessing() {
			webcamService->StopRecording();
			playerHitActivity.stop();
		}

		void ImageProcessingController::HandleMessageNotification(MessageNotification* notification) {
			const Message &message = notification->GetData();

			switch (message.GetCmd()) {
			case MessageCommandEnum::shot:
				//TODO implement shot method and so
				break;
			}

			notification->release();
		}

		void ImageProcessingController::CreateTrackBarView() {
			namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

			//Create trackbars in "Control" window
			cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
			cvCreateTrackbar("HighH", "Control", &iHighH, 179);

			cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
			cvCreateTrackbar("HighS", "Control", &iHighS, 255);

			cvCreateTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
			cvCreateTrackbar("HighV", "Control", &iHighV, 255);
		}

		void ImageProcessingController::OnTimer(Timer& timer) {
			Mat frame = webcamService->GetLastImage();
			Player p;
			Shot shot = detectService->DetectShotRoute(frame, p);
			shotSimulation.SimulateShot(shot);
		}

		void ImageProcessingController::HandlePlayerHitNotification() {
			AutoPtr<Notification> notification(playerHitQueue.waitDequeueNotification());

			while (!playerHitActivity.isStopped() && notification) {
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
	}
}