#ifndef VBDOCUMENT_EXPORT
#define VBDOCUMENT_EXPORT

#pragma once

#include "PlatformDLLExport.hpp"

#if defined (VBDOCUMENT_DLL_COMPILE)
	#define VBDOCUMENT_DLL_EXPORT		PLATFORM_DLL_EXPORT
#else
	#define VBDOCUMENT_DLL_EXPORT		PLATFORM_DLL_IMPORT
#endif

#if defined (VBDOCUMENT_DLL_COMPILE)
	#define VBDOCUMENT_DLL_EXPORT_FOR_TEST	PLATFORM_DLL_EXPORT
#elif defined (VBDOCUMENT_TEST_COMPILE)
	#define VBDOCUMENT_DLL_EXPORT_FOR_TEST	PLATFORM_DLL_IMPORT
#else
	#define VBDOCUMENT_DLL_EXPORT_FOR_TEST
	// will compile but won't link
#endif

#endif
