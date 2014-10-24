//============================================================================
// Name        : VideoStreamingController.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "VideoStreamingController.h"


VideoStreamingController::VideoStreamingController()
{
	streamingServer = new VideoStreamingServer("127.0.0.1", "80");
}


VideoStreamingController::~VideoStreamingController()
{
}

void VideoStreamingController::StartNetworkService() {
	streamingServer->StartServer();
}

bool VideoStreamingController::StopNetworkService() {
	return true;
}