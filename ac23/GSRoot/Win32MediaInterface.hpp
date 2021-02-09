// *****************************************************************************
// File:			Win32MediaInterface.hpp
//
// Description:		Includes ATL Headers
//
// Namespace:		-
//
// Contact person:	MM
//
// SG compatible
// *****************************************************************************

#if !defined (WIN32OLEINTERFACE_HPP)
#define	WIN32OLEINTERFACE_HPP

#pragma once


// -- Includes -----------------------------------------------------------------

#include "Win32Interface.hpp"

#pragma warning (push)
#pragma warning (disable : 4995)

#include <Mfapi.h>
#include <Mfobjects.h>
#include <Mfidl.h>
#include <Mfreadwrite.h>

#pragma warning (pop)

#endif
