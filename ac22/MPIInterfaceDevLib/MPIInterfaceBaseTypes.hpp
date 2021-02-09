// *****************************************************************************
// MPI (Modeler Programming Interface) Add-On interface calling convention
// GSUtils, Platform independent
//
// Namespaces:        Contact person:
//                         PCS
//
// SG compatible
// *****************************************************************************

#if !defined (MPIINTERFACEBASETYPES_HPP)
#define MPIINTERFACEBASETYPES_HPP

#pragma once

#if defined (macintosh)
	#define	MPI_ADD_ON_CALL
#endif
#if defined (WINDOWS)
	#define	MPI_ADD_ON_CALL	__cdecl
#endif


/////////////////////////////////////////////////////////////////////////////
// Import-export macros

#include "PlatformDLLExport.hpp"

#define MPI_ADD_ON_EXPORT		extern "C" PLATFORM_DLL_EXPORT
#define MPI_ADD_ON_VERSION		2


#endif
