
// *****************************************************************************
//
//							Protected Alloc interface
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	EP
//
// *****************************************************************************

#ifndef GS_PROTECTEDALLOC_HPP
#define GS_PROTECTEDALLOC_HPP

#pragma once


// --- Includes ----------------------------------------------------------------

#include "Definitions.hpp"


// ============================== ProtectedAlloc ===============================

namespace GS {
namespace ProtectedAlloc {


// --- Implementation details; use the interface below! ------------------------

namespace Impl {

GSROOT_DLL_EXPORT extern bool isInitialized;
GSROOT_DLL_EXPORT extern bool isEnabled;
GSROOT_DLL_EXPORT extern bool isBackwards;
GSROOT_DLL_EXPORT extern bool isUnderLockdown;

GSROOT_DLL_EXPORT bool Initialize (void);

}	// namespace Impl


// --- Public interface --------------------------------------------------------

inline bool IsEnabled (void);											// Returns whether the application was started in Protected Alloc mode
inline bool IsBackwards (void);											// Returns whether allocations are guarded against buffer overruns or underruns
inline bool IsUnaligned (void) { return true; }							// Currently, only this mode is supported/implemented
inline bool IsUnderLockdown (void) { return Impl::isUnderLockdown; }	// Returns whether the heap is in lockdown mode or not (not implemented on Mac)

void*  Allocate (size_t size);
void*  Reallocate (void* memory, size_t size);
void   Free (void* memory);

size_t GetAllocationSize (void* memory);

bool   CheckHeap (void);


// --- Debug interface ---------------------------------------------------------

GSROOT_DLL_EXPORT void DBLookupBlockInfo (void* memoryAddress);
GSROOT_DLL_EXPORT void DBLookupDeadBlocks (void* memoryAddress);	// Takes an arbitrary address, not a user pointer
GSROOT_DLL_EXPORT void DBDumpDeadBlocks ();
GSROOT_DLL_EXPORT void DBDumpBlocks ();
GSROOT_DLL_EXPORT bool DBCheckBlock (void* memoryAddress);
GSROOT_DLL_EXPORT bool DBCheckHeap ();
GSROOT_DLL_EXPORT void DBPrintHeapStatistics ();
GSROOT_DLL_EXPORT void DBEnableLockdown ();
GSROOT_DLL_EXPORT void DBDisableLockdown ();


// --- Inline functions --------------------------------------------------------

inline bool IsEnabled ()
{
	if (!Impl::isInitialized)
		Impl::Initialize ();

	return Impl::isEnabled;
}


inline bool IsBackwards ()
{
	if (!Impl::isInitialized)
		Impl::Initialize ();

	return Impl::isBackwards;
}

}	// namespace ProtectedAlloc
}	// namespace GS

#endif // GS_PROTECTEDALLOC_HPP
