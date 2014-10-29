//============================================================================
// Name        : StreamResponseHandler.h
// Author      : ITM13
// Version     : 1.0
// Description : The common handler for all outgoing responses containing 
//				 streaming data.
//============================================================================
#pragma once
#include <string>

#include "..\..\..\services\videostreaming\webcam\WebcamService.h"

namespace infrastructure {
	namespace video_streaming {
		struct Reply;

		class StreamResponseHandler
		{
		public:
			StreamResponseHandler(services::webcam::WebcamServicePtr webcamService);
			~StreamResponseHandler();

			bool CreateResponse(Reply& reply);

		private:
			services::webcam::WebcamServicePtr webcamService;
		};
	}
}
