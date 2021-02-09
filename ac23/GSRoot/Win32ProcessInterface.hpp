// *****************************************************************************
// File:			Win32ProcessPlusInterface.hpp
//
// Description:		Includes Communication Related Windows Headers
//
// Namespace:		-
//
// Contact person:	MM
//
// SG compatible
// *****************************************************************************

#if !defined (WIN32PROCESSINTERFACE_HPP)
#define	WIN32PROCESSINTERFACE_HPP

#pragma once


// -- Includes -----------------------------------------------------------------

#include "Win32Interface.hpp"

#pragma warning (push)
#pragma warning (disable : 4995)

#include <PSAPI.h>
#include <process.h>
#include <Tlhelp32.h>

#pragma warning (pop)

#endif
