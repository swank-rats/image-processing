//============================================================================
// Name        : ImageProcessingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description :
//============================================================================
#include "ImageProcessingController.h"


namespace controller {
	namespace image_processing {
		ImageProcessingController::ImageProcessingController(SharedPtr<WebcamService> webcamService) : webcamService(webcamService) {
		}

		ImageProcessingController::~ImageProcessingController()	{
			//do not delete, since it is a shared pointer
			webcamService = nullptr;
		}

		void ImageProcessingController::StartImageProcessing() {
			webcamService->StartRecording();
		}

		void ImageProcessingController::StopImageProcessing() {
			webcamService->StopRecording();
		}
	}
}