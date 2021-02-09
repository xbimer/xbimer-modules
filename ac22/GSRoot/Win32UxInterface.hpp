// *****************************************************************************
// File:			Win32UxInterface.hpp
//
// Description:		Includes ATL Headers
//
// Namespace:		-
//
// Contact person:	MM
//
// SG compatible
// *****************************************************************************

#if !defined (WIN32UXINTERFACE_HPP)
#define	WIN32UXINTERFACE_HPP

#pragma once


// -- Includes -----------------------------------------------------------------

#include "Win32Interface.hpp"

#pragma warning (push)
#pragma warning (disable : 4995)

#include <CommCtrl.h>
#include <CommDlg.h>
#include <Imm.h>
#include <RichEdit.h>
#include <UxTheme.h>
#include <vsstyle.h>
#include <WindowsX.h>

#pragma warning (pop)

#endif
