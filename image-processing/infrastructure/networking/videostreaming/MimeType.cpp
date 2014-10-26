#include "MimeType.h"

namespace infrastructure {
	namespace video_streaming {
		namespace mime_type {
			struct Mapping
			{
				const char* extension;
				const char* mime_type;
			} mappings[] =
			{
				{ "gif", "image/gif" },
				{ "htm", "text/html" },
				{ "html", "text/html" },
				{ "jpg", "image/jpeg" },
				{ "png", "image/png" }
			};

			std::string ExtensionToType(const std::string& extension)
			{
				for (Mapping m : mappings)
				{
					if (m.extension == extension)
					{
						return m.mime_type;
					}
				}

				return "text/plain";
			}
		}
	}
}