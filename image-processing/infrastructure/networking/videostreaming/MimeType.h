#pragma once

#include <string>

namespace infrastructure {
	namespace video_streaming {
		namespace mime_type {
			/// Convert a file extension into a MIME type.
			std::string ExtensionToType(const std::string& extension);
		}
	}
}