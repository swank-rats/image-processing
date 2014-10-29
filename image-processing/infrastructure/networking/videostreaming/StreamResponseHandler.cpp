//============================================================================
// Name        : StreamResponseHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : The common handler for all outgoing responses containing 
//				 streaming data.
//============================================================================
#include <opencv2\opencv.hpp>

#include "StreamResponseHandler.h"
#include "Structs.h"
#include "MimeType.h"

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

			// copy image to buffer
			for (int i = 0; i < image.rows; i++)
			{
				for (int j = 0; j < image.cols; j++)
				{
					uchar* x = (uchar*)(image.data + i * image.step + j);
					rep.content.append(*(uchar*)(image.data + i * image.step + j), 1);
				}
			}

			rep.headers.resize(3);
			rep.headers[0].name = "";
			rep.headers[0].value = "--BoundaryString";
			rep.headers[1].name = "Content-type";
			rep.headers[1].value = mime_type::ExtensionToType("jpg");
			rep.headers[2].name = "Content-Length";
			rep.headers[2].value = std::to_string(rep.content.size());

			return true;
		}
	}
}
