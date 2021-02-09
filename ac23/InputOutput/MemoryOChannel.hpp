
// *****************************************************************************
//
//	                          Class MemoryOChannel
//
// Module: IO
// Namespace: IO
// Contact person: MB
//
// SG compatible
//
// *****************************************************************************


#if !defined MEMORYOCHANNEL_HPP
#define MEMORYOCHANNEL_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

#include "IOBase.hpp"

#include "OChannelAdapter.hpp"



// =========================== Class MemoryOChannel ============================

namespace IO {
class IO_DLL_EXPORT MemoryOChannel : public OChannelAdapter,
	private OutputBuffer::BinOutput
{
public:
	enum OwnershipType { OriginalOwnership, PassOwnership, GrantOwnership };

private:
	char*	destination;	// destination buffer which receives the data
	UInt64	size;			// size of the data stored in the destination buffer in characters
	UInt64	sizeLimit;		// maximum allowed size of the data in characters
	UInt64	capacity;		// actual capacity of the destination buffer (in characters)
	bool	ownsDest;		// specifies whether this object owns the destination buffer

	MemoryOChannel (const MemoryOChannel& source);				// disabled
	MemoryOChannel&	operator= (const MemoryOChannel& source);	// disabled

	GSErrCode	Init (UInt64 initialCapacity, UInt64 maxSize);
	void		Init (char* destBuffer, UInt64 destBufferSize, UInt64 maxSize, OwnershipType ot);

	virtual GSErrCode	InternalWrite (const char* source, USize nOfCharToWrite, USize* written) override;


	char*	AllocateMemory (UInt64 size);
	void	DeleteMemory (char* ptr);
public:
	// Constructors

	explicit MemoryOChannel (UInt64 initialCapacity = 0, UInt64 maxSize = 0);
	MemoryOChannel (char* destBuffer, UInt64 destBufferSize, UInt64 maxSize = 0, OwnershipType ot = OriginalOwnership);
	~MemoryOChannel ();

	// User interface

	GSErrCode	GetStatus (void) const;

	GSErrCode	SetDestination (UInt64 initialCapacity = 0, UInt64 maxSize = 0);
	void		SetDestination (char* destBuffer, UInt64 destBufferSize, UInt64 maxSize = 0, OwnershipType ot = OriginalOwnership);
	char*		GetDestination (OwnershipType ot = OriginalOwnership);
	const char*	GetDestination (void) const;
	UInt64		GetDataSize (void) const;
	GSErrCode   SetDataSize (UInt64 newSize);

	virtual GSErrCode   GetOutputPosition (UInt64* pos) const override;

	virtual	void	SetDefaultOutputProtocol (void) override;
};

}	// namespace IO
// ___________________________ class MemoryOChannel ____________________________


#endif
