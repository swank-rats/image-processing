//============================================================================
// Name        : VideoStreamingRequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "VideoStreamingRequestHandler.h"
#include <Poco\Net\MultipartWriter.h>
#include <Poco\Net\MessageHeader.h>

namespace infrastructure {
	namespace video_streaming {
		Poco::Logger& VideoStreamingRequestHandler::logger = Poco::Logger::get("VideoStreamingRequestHandler");

		VideoStreamingRequestHandler::VideoStreamingRequestHandler(services::webcam::WebcamServicePtr webcamService) : webcamService(webcamService)
		{
		}

		void VideoStreamingRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
			string boundary = "VIDEOSTREAM";

			logger.information("Video streaming started for client " + request.clientAddress().toString());
			
			//response.setStatus(Poco::Net::HTTPResponse::HTTP_OK); //default not needed to be set
			response.set("Max-Age", "0");
			response.set("Expires", "0");
			response.set("Cache-Control", "no-cache, private");
			response.set("Pragma", "no-cache");
			response.setContentType("multipart/x-mixed-replace; boundary=--" + boundary);
			response.setChunkedTransferEncoding(false);

			std::ostream& out = response.send();

			while (out.good()) {
				Poco::Net::MultipartWriter writer(out, boundary);

				cv::Mat& image = webcamService->GetLastImage();

				if (image.empty()) {
					logger.warning("Read empty stream image");
					continue;
				}

				// encode mat to jpg and copy it to content
				std::vector<uchar> buf;
				cv::imencode(".jpg", image, buf, std::vector<int>());
				std::string content = std::string(buf.begin(), buf.end());

				Poco::Net::MessageHeader header = Poco::Net::MessageHeader();
				header.set("Content-Type", "image/jpeg");
				header.set("Content-Length", std::to_string(content.size()));
				writer.nextPart(header);
				out << content;
			}

			logger.information("Video streaming stopped for client " + request.clientAddress().toString());
		}
	}
}