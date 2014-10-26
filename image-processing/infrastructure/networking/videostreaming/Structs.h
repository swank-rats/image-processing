//============================================================================
// Name        : Structs.h
// Author      : ITM13
// Version     : 1.0
// Description : 
//============================================================================
#pragma once

#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace infrastructure {
	namespace video_streaming {
		struct Header
		{
			std::string name;
			std::string value;
		};

		struct Request
		{
			std::string method;
			std::string uri;
			int http_version_major;
			int http_version_minor;
			std::vector<Header> headers;
		};

		struct Reply
		{
			enum StatusType
			{
				ok = 200,
				created = 201,
				accepted = 202,
				no_content = 204,
				multiple_choices = 300,
				moved_permanently = 301,
				moved_temporarily = 302,
				not_modified = 304,
				bad_request = 400,
				unauthorized = 401,
				forbidden = 403,
				not_found = 404,
				internal_server_error = 500,
				not_implemented = 501,
				bad_gateway = 502,
				service_unavailable = 503
			} status;

			std::vector<Header> headers;
			std::string content;

			std::vector<boost::asio::const_buffer> Reply::ToBuffers();
			static Reply GetStockReply(StatusType status);
		};

		//struct MimeType
		//{
		//	const char* extension;
		//	const char* mime_type;

		//	static std::string ExtensionToType(const std::string& extension);
		//} MimeTypes[] =
		//{
		//	{ "gif", "image/gif" },
		//	{ "htm", "text/html" },
		//	{ "html", "text/html" },
		//	{ "jpg", "image/jpeg" },
		//	{ "png", "image/png" }
		//};
	}
}