#ifndef BUILT_IN_LIBRARY_EXPORT
#define BUILT_IN_LIBRARY_EXPORT

#pragma once

#include "PlatformDLLExport.hpp"

#if defined (BUILT_IN_LIBRARY_DLL_COMPILE)
	#define BUILT_IN_LIBRARY_DLL_EXPORT		PLATFORM_DLL_EXPORT
#else
	#define BUILT_IN_LIBRARY_DLL_EXPORT		PLATFORM_DLL_IMPORT
#endif

#endif
