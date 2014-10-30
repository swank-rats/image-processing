//============================================================================
// Name        : StreamResponseHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : The common handler for all outgoing responses containing streaming data.
//============================================================================
#include <opencv2\opencv.hpp>

#include "StreamResponseHandler.h"
#include "Structs.h"

namespace infrastructure {
	namespace video_streaming {
		StreamResponseHandler::StreamResponseHandler(services::webcam::WebcamServicePtr webcamService)
			: webcamService(webcamService)
		{
		}


		StreamResponseHandler::~StreamResponseHandler()
		{
		}

		bool StreamResponseHandler::CreateResponse(Reply& rep) {
			cv::Mat image = webcamService->GetLastImage();

			if (image.empty()) {
				return false;
			}

			rep.status = Reply::ok;

			// encode mat to jpg and copy it to content
			std::vector<uchar> buf;
			cv::imencode(".jpg", image, buf, std::vector<int>());

			rep.content = std::string(buf.begin(), buf.end());

			rep.headers.resize(3);
			rep.headers[0].name = "";
			rep.headers[0].value = "--BoundaryString";
			rep.headers[1].name = "Content-type";
			rep.headers[1].value = "image/jpeg";
			rep.headers[2].name = "Content-Length";
			rep.headers[2].value = std::to_string(rep.content.size());

			return true;
		}
	}
}
