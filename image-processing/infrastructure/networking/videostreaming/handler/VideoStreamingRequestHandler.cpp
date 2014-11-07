//============================================================================
// Name        : VideoStreamingRequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "VideoStreamingRequestHandler.h"
#include <Poco\Net\MultipartWriter.h>
#include <Poco\Net\MessageHeader.h>

using Poco::Net::MessageHeader;
using Poco::Net::MultipartWriter;


namespace infrastructure {
	namespace video_streaming {

		VideoStreamingRequestHandler::VideoStreamingRequestHandler(WebcamServicePtr webcamService) : webcamService(webcamService)
		{
		}

		void VideoStreamingRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
			static std::vector<int> params = { CV_IMWRITE_JPEG_QUALITY, 30 };
			Poco::Logger& logger = Poco::Logger::get("VideoStreamingRequestHandler");

			string boundary = "VIDEOSTREAM";

			logger.information("Video streaming started for client " + request.clientAddress().toString());
			
			response.set("Max-Age", "0");
			response.set("Expires", "0");
			response.set("Cache-Control", "no-cache, private");
			response.set("Pragma", "no-cache");
			response.setContentType("multipart/x-mixed-replace; boundary=--" + boundary);
			response.setChunkedTransferEncoding(false);

			std::ostream& out = response.send();

			while (out.good()) {
				MultipartWriter writer(out, boundary);

				cv::Mat& image = webcamService->GetLastImage();

				if (image.empty()) {
					logger.warning("Read empty stream image");
					continue;
				}

				// encode mat to jpg and copy it to content
				std::vector<uchar> buf;
				cv::imencode(".jpg", image, buf, params);
				std::string content = std::string(buf.begin(), buf.end());

				MessageHeader header = MessageHeader();
				header.set("Content-Length", std::to_string(content.size()));
				header.set("Content-Type", "image/jpeg");
				writer.nextPart(header);
				out << content;
				out << "\r\n\r\n";
			}

			logger.information("Video streaming stopped for client " + request.clientAddress().toString());
		}
	}
}