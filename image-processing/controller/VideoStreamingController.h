//============================================================================
// Name        : VideoStreamingController.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once
#include "..\infrastructure\networking\VideoStreamingServer.h"

class VideoStreamingController
{
public:
	VideoStreamingController();
	~VideoStreamingController();

	void StartNetworkService();
	bool StopNetworkService();
private:
	VideoStreamingServer* streamingServer;
};

