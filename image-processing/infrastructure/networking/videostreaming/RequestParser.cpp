//============================================================================
// Name        : RequestParser.cpp
// Author      : ITM13
// Version     : 1.0
// Description : Parses a request
//				 Code adapted, original source see http://www.boost.org/doc/libs/1_56_0/doc/html/boost_asio/examples/cpp11_examples.html
//============================================================================
#include "RequestParser.h"
#include "Structs.h"

namespace infrastructure {
	namespace video_streaming {
		RequestParser::RequestParser()
			: state(method_start)
		{
		}

		void RequestParser::Reset()
		{
			state = method_start;
		}

		RequestParser::ResultType RequestParser::Consume(Request& req, char input)
		{
			switch (state)
			{
			case method_start:
				if (!IsChar(input) || IsCtl(input) || IsTspecial(input))
				{
					return bad;
				}
				else
				{
					state = method;
					req.method.push_back(input);
					return indeterminate;
				}
			case method:
				if (input == ' ')
				{
					state = uri;
					return indeterminate;
				}
				else if (!IsChar(input) || IsCtl(input) || IsTspecial(input))
				{
					return bad;
				}
				else
				{
					req.method.push_back(input);
					return indeterminate;
				}
			case uri:
				if (input == ' ')
				{
					state = http_version_h;
					return indeterminate;
				}
				else if (IsCtl(input))
				{
					return bad;
				}
				else
				{
					req.uri.push_back(input);
					return indeterminate;
				}
			case http_version_h:
				if (input == 'H')
				{
					state = http_version_t_1;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case http_version_t_1:
				if (input == 'T')
				{
					state = http_version_t_2;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case http_version_t_2:
				if (input == 'T')
				{
					state = http_version_p;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case http_version_p:
				if (input == 'P')
				{
					state = http_version_slash;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case http_version_slash:
				if (input == '/')
				{
					req.http_version_major = 0;
					req.http_version_minor = 0;
					state = http_version_major_start;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case http_version_major_start:
				if (IsDigit(input))
				{
					req.http_version_major = req.http_version_major * 10 + input - '0';
					state = http_version_major;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case http_version_major:
				if (input == '.')
				{
					state = http_version_minor_start;
					return indeterminate;
				}
				else if (IsDigit(input))
				{
					req.http_version_major = req.http_version_major * 10 + input - '0';
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case http_version_minor_start:
				if (IsDigit(input))
				{
					req.http_version_minor = req.http_version_minor * 10 + input - '0';
					state = http_version_minor;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case http_version_minor:
				if (input == '\r')
				{
					state = expecting_newline_1;
					return indeterminate;
				}
				else if (IsDigit(input))
				{
					req.http_version_minor = req.http_version_minor * 10 + input - '0';
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case expecting_newline_1:
				if (input == '\n')
				{
					state = header_line_start;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case header_line_start:
				if (input == '\r')
				{
					state = expecting_newline_3;
					return indeterminate;
				}
				else if (!req.headers.empty() && (input == ' ' || input == '\t'))
				{
					state = header_lws;
					return indeterminate;
				}
				else if (!IsChar(input) || IsCtl(input) || IsTspecial(input))
				{
					return bad;
				}
				else
				{
					req.headers.push_back(Header());
					req.headers.back().name.push_back(input);
					state = header_name;
					return indeterminate;
				}
			case header_lws:
				if (input == '\r')
				{
					state = expecting_newline_2;
					return indeterminate;
				}
				else if (input == ' ' || input == '\t')
				{
					return indeterminate;
				}
				else if (IsCtl(input))
				{
					return bad;
				}
				else
				{
					state = header_value;
					req.headers.back().value.push_back(input);
					return indeterminate;
				}
			case header_name:
				if (input == ':')
				{
					state = space_before_header_value;
					return indeterminate;
				}
				else if (!IsChar(input) || IsCtl(input) || IsTspecial(input))
				{
					return bad;
				}
				else
				{
					req.headers.back().name.push_back(input);
					return indeterminate;
				}
			case space_before_header_value:
				if (input == ' ')
				{
					state = header_value;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case header_value:
				if (input == '\r')
				{
					state = expecting_newline_2;
					return indeterminate;
				}
				else if (IsCtl(input))
				{
					return bad;
				}
				else
				{
					req.headers.back().value.push_back(input);
					return indeterminate;
				}
			case expecting_newline_2:
				if (input == '\n')
				{
					state = header_line_start;
					return indeterminate;
				}
				else
				{
					return bad;
				}
			case expecting_newline_3:
				return (input == '\n') ? good : bad;
			default:
				return bad;
			}
		}

		bool RequestParser::IsChar(int c)
		{
			return c >= 0 && c <= 127;
		}

		bool RequestParser::IsCtl(int c)
		{
			return (c >= 0 && c <= 31) || (c == 127);
		}

		bool RequestParser::IsTspecial(int c)
		{
			switch (c)
			{
			case '(': case ')': case '<': case '>': case '@':
			case ',': case ';': case ':': case '\\': case '"':
			case '/': case '[': case ']': case '?': case '=':
			case '{': case '}': case ' ': case '\t':
				return true;
			default:
				return false;
			}
		}

		bool RequestParser::IsDigit(int c)
		{
			return c >= '0' && c <= '9';
		}
	}
}