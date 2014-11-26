//============================================================================
// Name        : ImageProcessingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ImageProcessingController.h"

#include "..\services\ObjectDetectionService.h"
#include "..\shared\notifications\PlayerHitNotification.h"

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
			webcamService->AddObserver(this);

			CreateTrackBarView();

			webcamService->StartRecording();
			playerHitActivity.start();
		}

		void ImageProcessingController::StopImageProcessing() {
			webcamService->StopRecording();

			webcamService->RemoveObserver(this);
			playerHitActivity.stop();
		}

		void ImageProcessingController::Update(WebcamService* observable) {
			Mat frame = observable->GetLastImage();

			//detectService->DetectObject(frame, iLowH, iLowS, iLowV, iHighH, iHighS, iHighV);
		}

		void ImageProcessingController::HandleMessageNotification(const AutoPtr<MessageNotification>& notification) {
			//TODO handle message - eg simulate shot and so on
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
			static int player = 0;
			Mat frame = webcamService->GetLastImage();
			Shot shot = detectService->DetectShotRoute(frame, player);
			shotSimulation.SimulateShot(shot);
			player = player == 0 ? 1 : 0;
		}

		void ImageProcessingController::HandlePlayerHitNotification() {
			AutoPtr<Notification> notification(playerHitQueue.waitDequeueNotification());

			while (!playerHitActivity.isStopped() && notification) {
				PlayerHitNotification* playerHitNotification = dynamic_cast<PlayerHitNotification*>(notification.get());
				if (playerHitNotification)
				{
					// TODO concrete command
					websocketController->Send(new Message("hit"));
				}

				notification = playerHitQueue.waitDequeueNotification();
			}
		}
	}
}