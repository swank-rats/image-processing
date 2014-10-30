//============================================================================
// Name        : RequestParser.h
// Author      : ITM13
// Version     : 1.0
// Description : Parses a request
//				 Code adapted, original source see http://www.boost.org/doc/libs/1_56_0/doc/html/boost_asio/examples/cpp11_examples.html
//============================================================================
#pragma once

#include <tuple>

namespace infrastructure {
	namespace video_streaming {
		struct Request;

		class RequestParser
		{
		public:
			RequestParser();
			void Reset();
			enum ResultType { good, bad, indeterminate };

			// Parse some data. The enum return value is good when a complete request has
			// been parsed, bad if the data is invalid, indeterminate when more data is
			// required. The InputIterator return value indicates how much of the input
			// has been consumed.
			template <typename InputIterator>
			std::tuple<ResultType, InputIterator> Parse(Request& req, InputIterator begin, InputIterator end)
			{
				while (begin != end)
				{
					ResultType result = Consume(req, *begin++);
					if (result == good || result == bad)
						return std::make_tuple(result, begin);
				}
				return std::make_tuple(indeterminate, begin);
			}
		private:
			// Handle the next character of input.
			ResultType Consume(Request& req, char input);
			// Check if a byte is an HTTP character.
			static bool IsChar(int c);
			// Check if a byte is an HTTP control character.
			static bool IsCtl(int c);
			// Check if a byte is defined as an HTTP tspecial character.
			static bool IsTspecial(int c);
			// Check if a byte is a digit.
			static bool IsDigit(int c);
			// The current state of the parser.
			enum State
			{
				method_start,
				method,
				uri,
				http_version_h,
				http_version_t_1,
				http_version_t_2,
				http_version_p,
				http_version_slash,
				http_version_major_start,
				http_version_major,
				http_version_minor_start,
				http_version_minor,
				expecting_newline_1,
				header_line_start,
				header_lws,
				header_name,
				space_before_header_value,
				header_value,
				expecting_newline_2,
				expecting_newline_3
			} state;
		};
	}
}