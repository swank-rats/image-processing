//============================================================================
// Name        : RequestHandler.cpp
// Author      : ITM13
// Version     : 1.0
// Description : The common handler for all incoming requests.
//============================================================================
#include "RequestHandler.h"
#include "Structs.h"

namespace infrastructure {
	namespace video_streaming {
		RequestHandler::RequestHandler(const std::string uri) 
			: uri(uri)
		{	}

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

			rep = Reply::GetStockReply(Reply::ok);
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