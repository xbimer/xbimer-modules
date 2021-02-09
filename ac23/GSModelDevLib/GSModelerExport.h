/*==========================================================================**
**								GSModelerExport.h							**
**																			**
**			DEFINITIONS FOR EXPORTATION FROM THE GSMODELER MODULE			**
**==========================================================================*/

#ifndef	GSMODELEREXPORT_H
#define	GSMODELEREXPORT_H

#pragma once

#include "PlatformDLLExport.hpp"

#if	defined	(GSMODELER_NOT_DLL)
	#define GSMODELER_DLL_EXPORT
#else
	#if defined (GSMODELER_DLL_COMPILE)
		#define GSMODELER_DLL_EXPORT	PLATFORM_DLL_EXPORT
	#else
		#define GSMODELER_DLL_EXPORT	PLATFORM_DLL_IMPORT
	#endif
#endif

#define GSModelerModulID 1000

#endif
