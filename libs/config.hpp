#pragma once

#ifndef LIB_CONFIG
#define LIB_CONFIG

namespace config
{
#ifdef NDEBUG
	const int debugmod=0;
#else
	const int debugmod=1;
#endif
}

#endif
