// ****************************************************************************
//
// Module:			GSRoot/BasicEnvironment
// Namespace:		GS
// Contact person:	SzA
//
// ****************************************************************************


#ifndef GSUSERINFO
#define GSUSERINFO


// ------------------------------- Includes -----------------------------------

// from GSRoot
#include "GSRootExport.hpp"


// --- Predeclarations	-------------------------------------------------------

namespace GS {
	class UniString;
}


// ------------------------------- Namespace -----------------------------------

namespace GS {

// Undefine Windows Macros, we want to use "GetComputerName" and "GetUserName"

#if defined (WINDOWS)
#if defined (GetComputerName)
#undef GetComputerName
#endif
#if defined (GetUserName)
#undef GetUserName
#endif
#endif


GS::UniString	GSROOT_DLL_EXPORT	GetComputerName		();
GS::UniString	GSROOT_DLL_EXPORT	GetUserName			();

} // namespace GS

#endif //GSUSERINFO
