#pragma once

namespace ams
{
	typedef unsigned char byte;
#ifdef _WIN32
	typedef unsigned _int64 uint64;
#else
	typedef unsigned long long uint64;
#endif
}