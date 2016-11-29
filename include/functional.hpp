#pragma once
#include <string.h>
namespace xutil
{
	namespace functional
	{
		struct strcasecmper
		{
			bool operator()(const char *left, const char *right)
			{
#ifdef _WIN32
				return _strcmpi(left, right) == 0;
#else
				return strcasecmp(left, right) == 0;
#endif
			}
		};
		
		struct strncasecmper
		{
			bool operator()(const char *left, const char *right, std::size_t len)
			{
#ifdef _WIN32
				return _strnicmp(left, right, len) == 0;
#else
				return strncasecmp(left, right, len) == 0;
#endif
			}
		};

		struct strncmper
		{
			bool operator()(const char *left, const char *right, std::size_t len)
			{
				return strncmp(left, right, len) == 0;
			}
	};
	}
}