// *********************************************************************************************************************
// VBCalculations compiler environment
//
// Module:			VBCalculations
// Namespace:		-
// Contact person:	CsP
//
// [SG compatible]
// *********************************************************************************************************************

#if !defined (VB_ELEM_OPERATIONS__EXPORT_H)
#define VB_ELEM_OPERATIONS__EXPORT_H

#pragma once

#if defined (_MSC_VER) && !defined (WINDOWS)
#define	WINDOWS
#endif

#include "PlatformDLLExport.hpp"

#if defined (VB_ELEM_OPERATIONS_DLL_COMPILE)
	#define VB_ELEM_OPERATIONS_DLL_EXPORT		PLATFORM_DLL_EXPORT
	#ifdef __clang__
	#pragma clang diagnostic ignored "-Wweak-vtables"
	#endif
#else
	#define VB_ELEM_OPERATIONS_DLL_EXPORT		PLATFORM_DLL_IMPORT
#endif

#if defined (VB_ELEM_OPERATIONS_DLL_COMPILE)
	#define VB_ELEM_OPERATIONS_DLL_EXPORT_FOR_TEST		PLATFORM_DLL_EXPORT
#elif defined (VB_ELEM_OPERATIONS_TEST_DLL_COMPILE)
	#define VB_ELEM_OPERATIONS_DLL_EXPORT_FOR_TEST		PLATFORM_DLL_IMPORT
#endif

#endif // VB_ELEM_OPERATIONS__EXPORT_H

