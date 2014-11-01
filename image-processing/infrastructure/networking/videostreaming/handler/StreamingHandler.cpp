//============================================================================
// Name        : StreamingHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#include "StreamingHandler.h"
#include <Poco\Net\MultipartWriter.h>
#include <Poco\Net\MessageHeader.h>

namespace infrastructure {
	namespace video_streaming {
		StreamingHandler::StreamingHandler(services::webcam::WebcamServicePtr webcamService) : webcamService(webcamService)
		{
		}

		void StreamingHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
			Poco::Logger& logger = Poco::Logger::get("StreamingHandler");
			string boundary = "--VIDEOSTREAM";

			logger.warning("Video streaming started for client " + request.clientAddress().toString());
			
			//rep.status = Reply::ok;
			//response.setStatus(Poco::Net::HTTPResponse::HTTP_OK); //default not needed to be set
			response.setContentType("multipart/x-mixed-replace; boundary=" + boundary);
			response.setChunkedTransferEncoding(false);

			//rep.headers.resize(5);
			//rep.headers[0].name = "Max-Age";
			//rep.headers[0].value = "0";
			//rep.headers[1].name = "Expires";
			//rep.headers[1].value = "0";
			//rep.headers[2].name = "Cache-Control";
			//rep.headers[2].value = "no-cache, private";
			//rep.headers[3].name = "Pragma";
			//rep.headers[3].value = "no-cache";
			//rep.headers[4].name = "Content-Type";
			//rep.headers[4].value = "multipart/x-mixed-replace; boundary=--BoundaryString";

			std::ostream& out = response.send();
			Poco::Net::MultipartWriter writer(out, boundary);

			while (out.good()) {
				cv::Mat* image = webcamService->GetLastImage();

				if (image->empty()) {
					logger.warning("Read empty stream image");
					continue;
				}

				// encode mat to jpg and copy it to content
				std::vector<uchar> buf;
				cv::imencode(".jpg", image, buf, std::vector<int>());
				std::string content = std::string(buf.begin(), buf.end());

				out << content;

				Poco::Net::MessageHeader header = Poco::Net::MessageHeader();
				header.set("Content-Type", "image/jpeg");
				header.set("Content-Length", std::to_string(content.size()));
				writer.nextPart(header);
			}

			logger.warning("Video streaming stopped for client " + request.clientAddress().toString());



			//rep.content = std::string(buf.begin(), buf.end());

			//rep.headers.resize(3);
			//rep.headers[0].name = "";
			//rep.headers[0].value = "--BoundaryString";
			//rep.headers[1].name = "Content-type";
			//rep.headers[1].value = "image/jpeg";
			//rep.headers[2].name = "Content-Length";
			//rep.headers[2].value = std::to_string(rep.content.size());
		}
	}
}