// *********************************************************************************************************************
// File:			Graphix.hpp
//
// Description:		Main include of the GRAPHISOFT Graphix Module
//
// Project:			GRAPHISOFT Graphix Module (Graphix)
//
// Namespace:
//
// Contact person:	NZ, DG
//
// SG compatible
// *********************************************************************************************************************

#ifndef	GRAPHIX_HPP
#define	GRAPHIX_HPP

#include	"GraphixDefinitions.hpp"
#include	"FileTypeManager.hpp"

// --- Module export defines -------------------------------------------------------------------------------------------

bool	GRAPHIX_DLL_EXPORT CCALL		GraphixInit (bool disableHwAcceleration = false, bool openGLEnabled = true);
void	GRAPHIX_DLL_EXPORT CCALL		GraphixExit ();

void	GRAPHIX_DLL_EXPORT CCALL		GraphixFillWithSupportedFileTypes (FTM::FileTypeManager& fileTypeManager);

UInt32	GRAPHIX_DLL_EXPORT CCALL		GraphixGetOpenGLVersion ();

#endif
