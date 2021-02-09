
// *****************************************************************************
//
//	                          Class MemoryIChannel32
//
// Module: IO
// Namespace: IO
// Contact person: MB
//
// SG compatible
//
// *****************************************************************************


#if !defined MEMORYICHANNEL32_HPP
#define MEMORYICHANNEL32_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

#include "IOBase.hpp"

#include "IChannelAdapter.hpp"



// =========================== Class MemoryIChannel32 ============================

namespace IO {

class IO_DLL_EXPORT MemoryIChannel32: public IChannelAdapter,
									private InputBuffer::BinInput
{
public:
	enum OwnershipType { OriginalOwnership, PassOwnership };
	enum MemoryAllocationMode { BMAllocation, CppAllocation };

private:
	const char*		source;		// source of the data
	USize			length;		// length of the source in characters
	bool			ownsSrc;	// specifies whether this object owns the source
	UIndex			srcPos;		// position in the source
	MemoryAllocationMode memoryAllocationMode;	// specifies whether to use BM functions or native C++ memory operators for the memory deletion

	MemoryIChannel32 (const MemoryIChannel32& source);				// disabled
	MemoryIChannel32&	operator= (const MemoryIChannel32& source);	// disabled

	void	Init (void);
	void	Init (const char* sourceData, USize sourceDataLength, OwnershipType ot);

	virtual GSErrCode	InternalRead (char* destination, USize nOfCharToRead, USize* read) override;

	void DeleteMemory (char* src);

public:
		// Types

	struct InputBuffer {
		char* buffer;
		USize capacity;

		InputBuffer (): buffer (nullptr), capacity (0) {}
		InputBuffer (char* buffer, USize capacity): buffer (buffer), capacity (capacity) {}
	};

		// Constructors

	MemoryIChannel32 (const InputBuffer& inputBuffer = InputBuffer (), MemoryAllocationMode memoryAllocationMode = CppAllocation);
	MemoryIChannel32 (const char* sourceData, USize sourceDataLength, OwnershipType ot = OriginalOwnership, const InputBuffer& inputBuffer = InputBuffer (), MemoryAllocationMode memoryAllocationMode = CppAllocation);
   ~MemoryIChannel32 ();

		// User interface

	void			SetSource	  (const char* sourceData, USize sourceDataLength, OwnershipType ot = OriginalOwnership);
	const char*		GetSource	  (void) const;
	USize			GetDataLength (void) const;
	void			RemoveSource  (void);
	void			ResetSource	  (void);

	virtual GSErrCode	GetInputPosition (UInt64* position) const override;
	virtual GSErrCode	GetAvailable     (UInt64* available) const override;

	virtual void	SetDefaultInputProtocol (void) override;
};

}	// namespace IO
// ___________________________ Class MemoryIChannel32 ____________________________


#endif
