//============================================================================
// Name        : RequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : The common handler for all incoming requests.
//============================================================================
#include "RequestHandler.h"
#include "MimeType.h"
#include "Structs.h"

namespace infrastructure {
	namespace video_streaming {
		RequestHandler::RequestHandler(const std::string uri, services::webcam::WebcamServicePtr webcamService) 
			: uri(uri), webcamService(webcamService)
		{

		}

		void RequestHandler::HandleRequest(const Request& req, Reply& rep)
		{
			// Decode url to path.
			std::string requestPath;
			if (!UrlDecode(req.uri, requestPath))
			{
				rep = Reply::GetStockReply(Reply::bad_request);
				return;
			}

			// Request path must be absolute and not contain "..".
			if (requestPath.empty() || requestPath[0] != '/' || requestPath.find("..") != std::string::npos)
			{
				rep = Reply::GetStockReply(Reply::bad_request);
				return;
			}

			// Determine the file extension.
			std::size_t post_of_uri = requestPath.find(uri);
			if (post_of_uri == std::string::npos)
			{
				rep = Reply::GetStockReply(Reply::bad_request);
				return;
			}

			std::string extension;

			// Fill out the reply to be sent to the client.
			rep.status = Reply::ok;
			IplImage* frame = webcamService->GetLastImage();
			//char buf[512];
			//while (is.read(buf, sizeof(buf)).gcount() > 0)
			//	rep.content.append(buf, is.gcount());
			rep.headers.resize(2);
			rep.headers[0].name = "Content-Length";
			//rep.headers[0].value = std::to_string(rep.content.size());
			rep.headers[0].value = "hello";
			rep.headers[1].name = "Content-Type";
			rep.headers[1].value = mime_type::ExtensionToType(extension);


			"HTTP/1.0 200 OK\r\n" +
				"Server: YourServerName\r\n" +
				"Connection: close\r\n" +
				"Max-Age: 0\r\n" +
				"Expires: 0\r\n" +
				"Cache-Control: no-cache, private\r\n" +
				"Pragma: no-cache\r\n" +
				"Content-Type: multipart/x-mixed-replace; " +
				"boundary=--BoundaryString\r\n\r\n").getBytes());

		}

		bool RequestHandler::UrlDecode(const std::string& in, std::string& out)
		{
			out.clear();
			out.reserve(in.size());
			for (std::size_t i = 0; i < in.size(); ++i)
			{
				if (in[i] == '%')
				{
					if (i + 3 <= in.size())
					{
						int value = 0;
						std::istringstream is(in.substr(i + 1, 2));
						if (is >> std::hex >> value)
						{
							out += static_cast<char>(value);
							i += 2;
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else if (in[i] == '+')
				{
					out += ' ';
				}
				else
				{
					out += in[i];
				}
			}
			return true;
		}
	}
}