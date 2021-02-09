
// *****************************************************************************
//
//                               Class StableArray
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB
//
// SG compatible
//
// *****************************************************************************


#if !defined STABLEARRAY_HPP
#define STABLEARRAY_HPP

#pragma once

// --------------------------------- Includes ----------------------------------

#include "RandomContainer.hpp"

#include "RandomContainerIterator.hpp"
#include "RandomContainerReverseIterator.hpp"
#include "Enumerator.hpp"
#include "BaseAlgorithms.hpp"

#include "ClassInfo.hpp"
#include "IOFrame.hpp"
#include "Channel.hpp"
#include "BM.hpp"
#include "InitializerRefList.hpp"

#include <new>
#include <utility>
#include <functional>


// ============================= Class StableArray =============================

namespace GS {

class GSROOT_DLL_EXPORT StableArrayPrivate {
public:
	static GS::ClassInfo	classInfo;	// StableArray's private classInfo
};


template <class Type>
class StableArray: public RandomContainer<Type> {
public:
	enum AllocationStrategy { Constant, Linear, Exponential };
	enum CompactionStrategy { Manual, AtQuarter };

private:
	Type**	mapTable;			// "index -> pointer to item" conversion table (contains pointers to items in pages)
	USize	mapTableCapacity;	// capacity of the map table
	Type**	pageTable;			// table of pointers to pages of items
	USize	pageTableCapacity;	// capacity of the page table
	UChar	allocationStrategy;	// memory allocation strategy (affects number of pages in allocation units)
	UChar	log2PageSize;		// log2 of the size of a page
	UShort	offsetMask;			// mask of the offset inside a page
	USize	capacity;			// capacity of the StableArray (in items)
	USize	allocationUnit;		// size of the current (last) allocation unit in pages
	USize	size;				// number of items in the StableArray
	UChar	compactionStrategy;	// controls compaction of the StableArray

	void	Init (USize pageSize = 0, AllocationStrategy allocStrategy = Linear, CompactionStrategy compactStrategy = Manual);

	inline Type&		Cell (UIndex idx);
	inline const Type&	Cell (UIndex idx) const;

	inline USize	GetPageNumber (void) const;

	inline void		IncreaseCapacity (USize minCapacity);
	void			AddNewPages		 (USize minCapacity);

	inline void		DecreaseCapacity  (USize minCapacity);

	inline ULong	GetNextAllocationUnit (ULong currentAllocationUnit) const;
	inline ULong	GetPrevAllocationUnit (ULong currentAllocationUnit) const;

	static char*	AllocateBuffer (USize bufferCapacity);
	static void		DeleteBuffer   (char* buffer);

	static inline ULong		ComputeLog2 (ULong n);

//	friend inline bool	operator== (UChar uc, AllocationStrategy as) { return (uc == static_cast<UChar> (as)); }
//	friend inline bool	operator== (UChar uc, CompactionStrategy cs) { return (uc == static_cast<UChar> (cs)); }

public:
		// Types

	typedef RandomContainerIterator<StableArray>				Iterator;
	typedef ConstRandomContainerIterator<StableArray>			ConstIterator;

	typedef GeneralRandomContainerIterator<StableArray>			GeneralIterator;
	typedef ConstGeneralRandomContainerIterator<StableArray>	ConstGeneralIterator;

	typedef RandomContainerReverseIterator<StableArray>			ReverseIterator;
	typedef ConstRandomContainerReverseIterator<StableArray>	ConstReverseIterator;

	typedef RandomContainerRange<StableArray>					Range;
	typedef RandomContainerConstRange<StableArray>				ConstRange;

	class ConstEnumerator;
	class ConversionEnumerator;

		// Constructors

	explicit StableArray (USize pageSize = 0, AllocationStrategy allocStrategy = Linear, CompactionStrategy compactStrategy = Manual);
	explicit StableArray (ConstIterator begin);
	explicit StableArray (ConstForwardIterator<Type> begin);
	explicit StableArray (const GS::ConstEnumerator<Type>& enumerator);
	StableArray (const GS::ConversionEnumerator<Type>& enumerator);
	StableArray (std::initializer_list<Type> items);
	StableArray (ByRefSelector, InitializerRefList<Type> items);
	StableArray (const StableArray& source);
	StableArray (StableArray&& source);

	template <class SourceType>
	explicit StableArray (const StableArray<SourceType>& source);

	StableArray&	operator= (const StableArray& source);
	StableArray&	operator= (StableArray&& source);
	StableArray&	operator= (const GS::ConstEnumerator<Type>& enumerator);

	template <class SourceType>
	StableArray&	operator= (const StableArray<SourceType>& source);

   ~StableArray ();

	inline	void	Move (StableArray& source);
	void			Move (const std::function<void (Type&&)>& processor);

		// Item access

	inline Type&		operator[] (UIndex idx);
	inline const Type&	operator[] (UIndex idx) const;

	inline Type&		Get (UIndex idx);
	inline const Type&	Get (UIndex idx) const;

	inline Type&		GetFirst (void);
	inline const Type&	GetFirst (void) const;

	inline Type&		GetLast (void);
	inline const Type&	GetLast (void) const;
	inline UIndex		GetLastIndex (void) const;

	inline void			Set (UIndex idx, const Type& item);
	inline void			Set (UIndex idx, Type&& item);

		// Content management

	void	Fill (const Type& item, UIndex from = 0, USize range = MaxUSize);

	USize	Replace (const Type& oldItem, const Type& newItem, UIndex from = 0, USize range = MaxUSize);

	void	Insert (UIndex to, const Type& item);
	void	Insert (UIndex to, Type&& item);
	void	Insert (UIndex to, ConstIterator begin);
	void	Insert (UIndex to, ConstIterator begin, ConstIterator end);
	void	Insert (UIndex to, ConstForwardIterator<Type> begin);
	void	Insert (UIndex to, ConstForwardIterator<Type> begin, ConstForwardIterator<Type> end);

	void	Append (const StableArray& items);
	void	Append (StableArray&& items);

	void	Delete		(UIndex idx);
	void	Delete		(UIndex from, USize range);
	void	DeleteLast	(void);

	UIndex	DeleteFirst	(const Type& item);
	UIndex	DeleteLast	(const Type& item);
	void	DeleteAll	(const Type& item);

	void	Clear (void);

	void	Swap (StableArray& rightOp);

	void	Push (const Type& item);
	void	Push (Type&& item);

	template <class... ItemParameters>
	void	PushNew (ItemParameters&&... itemParameters);

	void	Pop	 (Type* item);
	Type	Pop	 (void);

	inline USize	GetSize		 (void) const;
	inline bool		IsEmpty		 (void) const;
	inline bool		IsLast		 (UIndex idx) const;
	void			SetSize		 (USize newSize);
	void			IncreaseSize (USize sizeIncrement);
	void			EnsureSize	 (USize requiredSize);

		// Content query

	inline Iterator				Enumerate (void);
	inline ConstIterator		Enumerate (void) const;

	template <class Function>
	inline void					Enumerate	   (Function&& processor);
	template <class Function>
	inline void					Enumerate	   (Function&& processor) const;
	template <class Function>
	inline void					EnumerateConst (Function&& processor) const;

	inline Iterator				Begin (void);
	inline ConstIterator		Begin (void) const;

	inline Iterator				End	(void);
	inline ConstIterator		End	(void) const;

	inline GeneralIterator		EnumerateGeneral (void);
	inline ConstGeneralIterator	EnumerateGeneral (void) const;

	inline GeneralIterator		BeginGeneral (void);
	inline ConstGeneralIterator	BeginGeneral (void) const;

	inline GeneralIterator		EndGeneral	(void);
	inline ConstGeneralIterator	EndGeneral	(void) const;

	inline ReverseIterator		ReverseEnumerate (void);
	inline ConstReverseIterator	ReverseEnumerate (void) const;

	template <class Function>
	inline void					ReverseEnumerate	  (Function&& processor);
	template <class Function>
	inline void					ReverseEnumerate	  (Function&& processor) const;
	template <class Function>
	inline void					ReverseEnumerateConst (Function&& processor) const;

	inline ReverseIterator		ReverseBegin (void);
	inline ConstReverseIterator	ReverseBegin (void) const;

	inline ReverseIterator		ReverseEnd	(void);
	inline ConstReverseIterator	ReverseEnd	(void) const;

	Range         operator() (UIndex from, USize to);
	ConstRange    operator() (UIndex from, USize to) const;

	inline operator ConstEnumerator	() const;
	inline ConversionEnumerator		Convert (void) const;

	inline auto&				AsConst (void) const;

	UIndex	FindFirst (const Type& item, UIndex from = 0, USize range = MaxUSize) const;
	UIndex	FindFirst (const std::function<bool (const Type&)>& condition, UIndex from = 0, USize range = MaxUSize) const;
	UIndex	FindLast  (const Type& item) const;
	UIndex	FindLast  (const std::function<bool (const Type&)>& condition) const;

	bool	Contains	 (const Type& item) const;
	bool	Contains	 (const std::function<bool (const Type&)>& condition) const;
	bool	Contains	 (std::initializer_list<Type> items) const;
	bool	ContainsOnly (std::initializer_list<Type> items) const;

	USize	Count	 (const Type& item, UIndex from = 0, USize range = MaxUSize) const;
	USize	Count	 (const std::function<bool (const Type&)>& condition, UIndex from = 0, USize range = MaxUSize) const;

	bool	operator== (const StableArray& rightOp) const;
	bool	operator!= (const StableArray& rightOp) const;

		// Performance tuning

	USize				GetCapacity	  (void) const;
	double				GetFillFactor (void) const;
	inline USize		GetPageSize	  (void) const;
	AllocationStrategy	GetAllocationStrategy (void) const;
	CompactionStrategy	GetCompactionStrategy (void) const;

	void	Compact	(void);

		// I/O

	GSErrCode	Read  (IChannel& ic);
	GSErrCode	Read  (IChannel& ic, const std::function<GSErrCode (IChannel&, Type&)>& reader);
	GSErrCode	Write (OChannel& oc) const;
	GSErrCode	Write (OChannel& oc, const std::function<GSErrCode (OChannel&, const Type&)>& writer) const;

		// Debug

	void	Print	   (OChannel& oc) const;
	void	Dump	   (OChannel& oc) const;
	bool	CheckState (void) const;

		// Types

	class ConstEnumerator: public GS::ConstEnumerator<Type> {
	private:
		const StableArray* source;

	public:
		inline ConstEnumerator (const StableArray& source);

		virtual void	Enumerate (const std::function<void (const Type&)>& processor) const override;
	};

	class ConversionEnumerator: public GS::ConversionEnumerator<Type> {
	private:
		const StableArray* source;

	public:
		inline ConversionEnumerator (const StableArray& source);

		virtual void	Enumerate (const std::function<void (const Type&)>& processor) const override;
	};
};

// === StableArray methods =============================================================================================

template <class Type>
StableArray<Type>::StableArray (USize pageSize, AllocationStrategy allocStrategy, CompactionStrategy compactStrategy)
{
	Init (pageSize, allocStrategy, compactStrategy);
}


template <class Type>
StableArray<Type>::StableArray (ConstIterator begin)
{
	Init ();

	while (begin != nullptr) {
		Push (*begin);
		++begin;
	}
}


template <class Type>
StableArray<Type>::StableArray (ConstForwardIterator<Type> begin)
{
	Init ();

	while (begin != nullptr) {
		Push (*begin);
		++begin;
	}
}


template <class Type>
StableArray<Type>::StableArray (const GS::ConstEnumerator<Type>& enumerator)
{
	Init ();

	enumerator.Enumerate (GetInserter (*this));
}


template <class Type>
StableArray<Type>::StableArray (const GS::ConversionEnumerator<Type>& enumerator)
{
	Init ();

	enumerator.Enumerate (GetInserter (*this));
}


template <class Type>
StableArray<Type>::StableArray (const StableArray& source)
{
	Init (source.GetPageSize (), static_cast<AllocationStrategy> (source.allocationStrategy), static_cast<CompactionStrategy> (source.compactionStrategy));

	IncreaseCapacity (source.size);

	for (UIndex i = 0; i < source.size; i++)
		new (mapTable[i]) Type (*source.mapTable[i]);
	size = source.size;
}


template <class Type>
StableArray<Type>::StableArray (std::initializer_list<Type> items)
{
	Init ();

	IncreaseCapacity (USize (items.size ()));

	UIndex i = 0;
	for (const auto& item : items)
		new (mapTable[i++]) Type (item);

	size = USize (items.size ());
}


template <class Type>
StableArray<Type>::StableArray (ByRefSelector, InitializerRefList<Type> items)
{
	Init ();

	IncreaseCapacity (USize (items.size ()));

	UIndex i = 0;
	for (const auto& item : items)
		new (mapTable[i++]) Type (*item);

	size = USize (items.size ());
}


template <class Type>
StableArray<Type>::StableArray (StableArray&& source)
{
	Init ();

	Move (source);
}


template <class Type>
void	StableArray<Type>::Init (USize pageSize, AllocationStrategy allocStrategy, CompactionStrategy compactStrategy)
{
	mapTable = nullptr;
	mapTableCapacity = 0;

	pageTable = nullptr;
	pageTableCapacity = 0;

	allocationStrategy = static_cast<UChar> (allocStrategy);

	if (pageSize == 0)	{	// 0 indicates default page size
		switch (allocStrategy) {
			case Constant:
				pageSize = 128;
				break;

			case Linear:
				pageSize = 32;
				break;

			case Exponential:
				pageSize = 16;
				break;

			default:
				DBBREAK ();
				pageSize = 32;
				break;
		}
	}
	if (pageSize > 65536)
		pageSize = 65536;	// because offsetMask is unsigned short
	log2PageSize = static_cast<UChar> (ComputeLog2 (pageSize));

//	DBASSERT (sizeof (offsetMask) == 2);	// type of offsetMask should be changed if this condition is violated
	offsetMask = static_cast<UShort> ((1 << log2PageSize) - 1);

	capacity = 0;
	allocationUnit = 0;
	size = 0;
	compactionStrategy = static_cast<UChar> (compactStrategy);
}


template <class Type>
StableArray<Type>&	StableArray<Type>::operator= (const StableArray& source)
{
	StableArray copy = source;
	Swap (copy);

	return *this;
}


template <class Type>
StableArray<Type>&	StableArray<Type>::operator= (StableArray&& source)
{
	Move (source);

	return *this;
}


template <class Type>
StableArray<Type>&	StableArray<Type>::operator= (const GS::ConstEnumerator<Type>& enumerator)
{
	Clear ();

	enumerator.Enumerate (GetInserter (*this));

	return *this;
}


template <class Type>
StableArray<Type>::~StableArray ()
{
	Clear ();
}


template <class Type>
template <class SourceType>
StableArray<Type>::StableArray (const StableArray<SourceType>& source)
{
	static_assert (IsConvertible<SourceType, Type>, "'SourceType' is not convertible to 'Type'.");

	Init ();

	IncreaseCapacity (source.GetSize ());

	USize i = 0;
	for (const auto& item : source)
		new (mapTable[i++]) Type (item);
	size = source.GetSize ();
}


template <class Type>
template <class SourceType>
StableArray<Type>&	StableArray<Type>::operator= (const StableArray<SourceType>& source)
{
	static_assert (IsConvertible<SourceType, Type>, "'SourceType' is not convertible to 'Type'.");

	StableArray<Type> copy (source);
	Swap (copy);

	return *this;
}


template <class Type>
inline	void	StableArray<Type>::Move (StableArray& source)
{
	if (DBERROR (&source == this))
		return;

	Swap (source);
	source.Clear ();
}


template <class Type>
void			StableArray<Type>::Move (const std::function<void (Type&&)>& processor)
{
	for (UIndex i = 0; i < size; ++i)
		processor (std::move (Cell (i)));

	Clear ();
}


template <class Type>
inline Type&	StableArray<Type>::operator[] (UIndex idx)
{
	REF_CHECK (idx < size);

	return *mapTable[idx];
}


template <class Type>
inline const Type&	StableArray<Type>::operator[] (UIndex idx) const
{
	REF_CHECK (idx < size);

	return *mapTable[idx];
}


template <class Type>
inline Type&	StableArray<Type>::Get (UIndex idx)
{
	REF_CHECK_R (idx < size);

	return *mapTable[idx];
}


template <class Type>
inline const Type&	StableArray<Type>::Get (UIndex idx) const
{
	REF_CHECK_R (idx < size);

	return *mapTable[idx];
}


template <class Type>
inline Type&	StableArray<Type>::GetFirst (void)
{
	PRECOND (size != 0);

	return *mapTable[0];
}


template <class Type>
inline const Type&	StableArray<Type>::GetFirst (void) const
{
	PRECOND (size != 0);

	return *mapTable[0];
}


template <class Type>
inline Type&	StableArray<Type>::GetLast (void)
{
	PRECOND (size != 0);

	return *mapTable[size - 1];
}


template <class Type>
inline const Type&	StableArray<Type>::GetLast (void) const
{
	PRECOND (size != 0);

	return *mapTable[size - 1];
}


template <class Type>
inline UIndex	StableArray<Type>::GetLastIndex (void) const
{
	PRECOND (size != 0);

	return size - 1;
}


template <class Type>
inline void		StableArray<Type>::Set (UIndex idx, const Type& item)
{
	REF_CHECK (idx < size);
	NULL_REF_CHECK (item);

	*mapTable[idx] = item;
}


template <class Type>
inline void		StableArray<Type>::Set (UIndex idx, Type&& item)
{
	REF_CHECK (idx < size);
	NULL_REF_CHECK (item);

	*mapTable[idx] = std::move (item);
}


template <class Type>
void	StableArray<Type>::Fill (const Type& item, UIndex from, USize range)
{
	NULL_REF_CHECK (item);
	REF_CHECK ((range == MaxUSize && from <= size) || (range != MaxUSize && from + range <= size));

	if (range == MaxUSize)
		range = size - from;

	GS::Fill (Begin () + from, Begin () + from + range, item);
}


template <class Type>
USize	StableArray<Type>::Replace (const Type& oldItem, const Type& newItem, UIndex from, USize range)
{
	NULL_REF_CHECK (oldItem);
	NULL_REF_CHECK (newItem);
	REF_CHECK ((range == MaxUSize && from <= size) || (range != MaxUSize && from + range <= size));

	if (range == MaxUSize)
		range = size - from;

	return GS::Replace (Begin () + from, Begin () + from + range, oldItem, newItem);
}


template <class Type>
void	StableArray<Type>::Insert (UIndex to, const Type& item)
{
	REF_CHECK (to <= size);
	NULL_REF_CHECK (item);

	IncreaseCapacity (size + 1);

	Type* cellAddress = mapTable[size];
	new (cellAddress) Type (item);
	if (to != size)
		::BNMoveMemory (mapTable + to + 1, mapTable + to, (size - to) * sizeof (Type*));
	mapTable[to] = cellAddress;

	size++;
}


template <class Type>
void	StableArray<Type>::Insert (UIndex to, Type&& item)
{
	REF_CHECK (to <= size);
	NULL_REF_CHECK (item);

	IncreaseCapacity (size + 1);

	Type* cellAddress = mapTable[size];
	new (cellAddress) Type (std::move (item));
	if (to != size)
		::BNMoveMemory (mapTable + to + 1, mapTable + to, (size - to) * sizeof (Type*));
	mapTable[to] = cellAddress;

	size++;
}


template <class Type>
void	StableArray<Type>::Insert (UIndex to, ConstIterator begin)
{
	REF_CHECK (to <= size);

	while (begin != nullptr) {
		Insert (to, *begin);
		to++;
		++begin;
	}
}


template <class Type>
void	StableArray<Type>::Insert (UIndex to, ConstIterator begin, ConstIterator end)
{
	REF_CHECK (to <= size);

	while (begin != end) {
		Insert (to, *begin);
		to++;
		++begin;
	}
}


template <class Type>
void	StableArray<Type>::Insert (UIndex to, ConstForwardIterator<Type> begin)
{
	REF_CHECK (to <= size);

	while (begin != nullptr) {
		Insert (to, *begin);
		to++;
		++begin;
	}
}


template <class Type>
void	StableArray<Type>::Insert (UIndex to, ConstForwardIterator<Type> begin, ConstForwardIterator<Type> end)
{
	REF_CHECK (to <= size);

	while (begin != end) {
		Insert (to, *begin);
		to++;
		++begin;
	}
}


template <class Type>
void	StableArray<Type>::Append (const StableArray& items)
{
	NULL_REF_CHECK (items);

	if (&items == this) {
		StableArray temp = items;
		Push (std::move (temp));
		return;
	}

	IncreaseCapacity (size + items.GetSize ());

	for (UIndex i = 0; i < items.GetSize (); i++)
		new (mapTable[size + i]) Type (items[i]);

	size += items.GetSize ();
}


template <class Type>
void	StableArray<Type>::Append (StableArray&& items)
{
	NULL_REF_CHECK (items);

	IncreaseCapacity (size + items.GetSize ());

	for (UIndex i = 0; i < items.GetSize (); i++)
		new (mapTable[size + i]) Type (std::move (items[i]));

	size += items.GetSize ();
}


template <class Type>
void	StableArray<Type>::Delete (UIndex idx)
{
	REF_CHECK (idx < size);

	Type* cellAddress = mapTable[idx];
	cellAddress->~Type ();
	if (idx != size - 1) {
		::BNMoveMemory (mapTable + idx, mapTable + idx + 1, (size - idx - 1) * sizeof (Type*));
		mapTable[size - 1] = cellAddress;	//lint !e449 [mar ervenyes helyre mutat, a BNMoveMemory miatt]
	}
	size--;

	DecreaseCapacity (size);
}


template <class Type>
void	StableArray<Type>::Delete (UIndex from, USize range)
{
	REF_CHECK (from + range <= size);

	if (range == 0)
		return;

	for (UIndex i = from; i < from + range; i++)
		mapTable[i]->~Type ();

	for (UIndex j = from + range; j < size; j++) {
		Type* tmp = mapTable[j - range];
		mapTable[j - range] = mapTable[j];
		mapTable[j] = tmp;
	}
	size -= range;

	DecreaseCapacity (size);
}


template <class Type>
void	StableArray<Type>::DeleteLast (void)
{
	Type* cellAddress = mapTable[size - 1];
	cellAddress->~Type ();
	size--;

	DecreaseCapacity (size);
}


template <class Type>
UIndex	StableArray<Type>::DeleteFirst (const Type& item)
{
	NULL_REF_CHECK (item);

	UIndex first = FindFirst (item);
	if (first != MaxUIndex)
		Delete (first);

	return first;
}


template <class Type>
UIndex	StableArray<Type>::DeleteLast (const Type& item)
{
	NULL_REF_CHECK (item);

	UIndex last = FindLast (item);
	if (last != MaxUIndex)
		Delete (last);

	return last;
}


template <class Type>
void	StableArray<Type>::DeleteAll (const Type& item)
{
	NULL_REF_CHECK (item);

	UIndex last = 0;
	while (last != MaxUIndex)
		last = DeleteLast (item);	// deleting from the end is faster
}


template <class Type>
void	StableArray<Type>::Clear (void)
{
	for (UIndex i = 0; i < size; i++)
		mapTable[i]->~Type ();
	DeleteBuffer (reinterpret_cast<char*> (mapTable));

	UIndex j = GetPageNumber ();
	while (j > 0) {
		j -= allocationUnit;	// start index of the allocation unit
		DeleteBuffer (reinterpret_cast<char*> (pageTable[j]));
		allocationUnit = GetPrevAllocationUnit (allocationUnit);
	}

	DeleteBuffer (reinterpret_cast<char*> (pageTable));

	mapTable = nullptr;
	mapTableCapacity = 0;

	pageTable = nullptr;
	pageTableCapacity = 0;

	capacity = 0;
	allocationUnit = 0;
	size = 0;
}


template <class Type>
void	StableArray<Type>::Swap (StableArray& rightOp)
{
	NULL_REF_CHECK (rightOp);

	if (this == &rightOp)
		return;

	GS::Swap (mapTable,			  rightOp.mapTable);
	GS::Swap (mapTableCapacity,   rightOp.mapTableCapacity);
	GS::Swap (pageTable,		  rightOp.pageTable);
	GS::Swap (pageTableCapacity,  rightOp.pageTableCapacity);
	GS::Swap (allocationStrategy, rightOp.allocationStrategy);
	GS::Swap (log2PageSize,		  rightOp.log2PageSize);
	GS::Swap (offsetMask,		  rightOp.offsetMask);
	GS::Swap (capacity,			  rightOp.capacity);
	GS::Swap (allocationUnit,	  rightOp.allocationUnit);
	GS::Swap (size,				  rightOp.size);
	GS::Swap (compactionStrategy, rightOp.compactionStrategy);
}


template <class Type>
void	StableArray<Type>::Push (const Type& item)
{
	NULL_REF_CHECK (item);

	IncreaseCapacity (size + 1);

	new (mapTable[size]) Type (item);
	size++;
}


template <class Type>
void	StableArray<Type>::Push (Type&& item)
{
	NULL_REF_CHECK (item);

	IncreaseCapacity (size + 1);

	new (mapTable[size]) Type (std::move (item));
	size++;
}


template <class Type>
template <class... ItemParameters>
void	StableArray<Type>::PushNew (ItemParameters&&... itemParameters)
{
	IncreaseCapacity (size + 1);

	new (mapTable[size]) Type (std::forward<ItemParameters> (itemParameters)...);
	size++;
}


template <class Type>
void	StableArray<Type>::Pop (Type* item)
{
	PRECOND (size != 0);
	NULL_PTR_CHECK (item);

	*item = std::move (*mapTable[size - 1]);

	mapTable[size - 1]->~Type ();
	size--;

	DecreaseCapacity (size);
}


template <class Type>
Type	StableArray<Type>::Pop (void)
{
	PRECOND (size != 0);

	Type result = std::move (*mapTable[size - 1]);

	mapTable[size - 1]->~Type ();
	size--;

	DecreaseCapacity (size);

	return result;
}


template <class Type>
inline USize	StableArray<Type>::GetSize (void) const
{
	return size;
}


template <class Type>
inline bool		StableArray<Type>::IsEmpty (void) const
{
	return (size == 0);
}


template <class Type>
inline bool		StableArray<Type>::IsLast (UIndex idx) const
{
	return (size > 0 && idx == size - 1);
}


template <class Type>
void	StableArray<Type>::SetSize (USize newSize)
{
	if (newSize < size) {
		for (UIndex i = newSize; i < size; i++)
			mapTable[i]->~Type ();
		size = newSize;
		DecreaseCapacity (size);
	} else if (newSize > size) {
		IncreaseCapacity (newSize);
		for (UIndex i = size; i < newSize; i++)
			new (mapTable[i]) Type ();
		size = newSize;
	}
}


template <class Type>
void	StableArray<Type>::IncreaseSize (USize sizeIncrement)
{
	SetSize (size + sizeIncrement);
}


template <class Type>
void	StableArray<Type>::EnsureSize (USize requiredSize)
{
	if (size < requiredSize)
		SetSize (requiredSize);
}


template <class Type>
inline typename StableArray<Type>::Iterator		StableArray<Type>::Enumerate (void)
{
	return Iterator (*this, 0);
}


template <class Type>
inline typename StableArray<Type>::ConstIterator	StableArray<Type>::Enumerate (void) const
{
	return ConstIterator (*this, 0);
}


template <class Type>
template <class Function>
inline void	StableArray<Type>::Enumerate (Function&& processor)
{
	for (UIndex i = 0; i < size; i++)
		processor (Cell (i));
}


template <class Type>
template <class Function>
inline void	StableArray<Type>::Enumerate (Function&& processor) const
{
	for (UIndex i = 0; i < size; i++)
		processor (Cell (i));
}


template <class Type>
template <class Function>
inline void	StableArray<Type>::EnumerateConst (Function&& processor) const
{
	Enumerate (processor);
}


template <class Type>
inline typename StableArray<Type>::Iterator		StableArray<Type>::Begin (void)
{
	return Iterator (*this, 0);
}


template <class Type>
inline typename StableArray<Type>::ConstIterator	StableArray<Type>::Begin (void) const
{
	return ConstIterator (*this, 0);
}


template <class Type>
inline typename StableArray<Type>::Iterator		StableArray<Type>::End (void)
{
	return Iterator (*this, size);
}


template <class Type>
inline typename StableArray<Type>::ConstIterator	StableArray<Type>::End (void) const
{
	return ConstIterator (*this, size);
}


template <class Type>
inline typename StableArray<Type>::GeneralIterator		StableArray<Type>::EnumerateGeneral (void)
{
	return GeneralIterator (*this, 0);
}


template <class Type>
inline typename StableArray<Type>::ConstGeneralIterator		StableArray<Type>::EnumerateGeneral (void) const
{
	return ConstGeneralIterator (*this, 0);
}


template <class Type>
inline typename StableArray<Type>::GeneralIterator		StableArray<Type>::BeginGeneral (void)
{
	return GeneralIterator (*this, 0);
}


template <class Type>
inline typename StableArray<Type>::ConstGeneralIterator		StableArray<Type>::BeginGeneral (void) const
{
	return ConstGeneralIterator (*this, 0);
}


template <class Type>
inline typename StableArray<Type>::GeneralIterator		StableArray<Type>::EndGeneral (void)
{
	return GeneralIterator (*this, size);
}


template <class Type>
inline typename StableArray<Type>::ConstGeneralIterator		StableArray<Type>::EndGeneral (void) const
{
	return ConstGeneralIterator (*this, size);
}


template <class Type>
inline typename StableArray<Type>::ReverseIterator	StableArray<Type>::ReverseEnumerate (void)
{
	return ReverseIterator (*this, size - 1);
}


template <class Type>
inline typename StableArray<Type>::ConstReverseIterator	StableArray<Type>::ReverseEnumerate (void) const
{
	return ConstReverseIterator (*this, size - 1);
}


template <class Type>
template <class Function>
inline void	StableArray<Type>::ReverseEnumerate (Function&& processor)
{
	for (UIndex i = size; i > 0; i--)
		processor (Cell (i - 1));
}


template <class Type>
template <class Function>
inline void	StableArray<Type>::ReverseEnumerate (Function&& processor) const
{
	for (UIndex i = size; i > 0; i--)
		processor (Cell (i - 1));
}


template <class Type>
template <class Function>
inline void	StableArray<Type>::ReverseEnumerateConst (Function&& processor) const
{
	ReverseEnumerate (processor);
}


template <class Type>
inline typename StableArray<Type>::ReverseIterator	StableArray<Type>::ReverseBegin (void)
{
	return ReverseIterator (*this, size - 1);
}


template <class Type>
inline typename StableArray<Type>::ConstReverseIterator	StableArray<Type>::ReverseBegin (void) const
{
	return ConstReverseIterator (*this, size - 1);
}


template <class Type>
inline typename StableArray<Type>::ReverseIterator	StableArray<Type>::ReverseEnd (void)
{
	return ReverseIterator (*this, -1);
}


template <class Type>
inline typename StableArray<Type>::ConstReverseIterator	StableArray<Type>::ReverseEnd (void) const
{
	return ConstReverseIterator (*this, -1);
}


template <class Type>
typename StableArray<Type>::Range         StableArray<Type>::operator() (UIndex from, USize to)
{
	return { *this, from, to };
}


template <class Type>
typename StableArray<Type>::ConstRange    StableArray<Type>::operator() (UIndex from, USize to) const
{
	return { *this, from, to };
}


template <class Type>
inline StableArray<Type>::operator typename StableArray<Type>::ConstEnumerator () const
{
	return ConstEnumerator (*this);
}


template <class Type>
inline typename StableArray<Type>::ConversionEnumerator		StableArray<Type>::Convert (void) const
{
	return ConversionEnumerator (*this);
}


template <class Type>
inline auto&	StableArray<Type>::AsConst (void) const
{
	return reinterpret_cast<const StableArray<typename Container<Type>::template MakeConstPtr<Type>::ConstType>&> (*this);
}


template <class Type>
UIndex	StableArray<Type>::FindFirst (const Type& item, UIndex from, USize range) const
{
	NULL_REF_CHECK (item);
	REF_CHECK ((range == MaxUSize && from <= size) || (range != MaxUSize && from + range <= size));

	if (range == MaxUSize)
		range = size - from;

	ConstIterator it = GS::Find (Begin () + from, Begin () + from + range, item);

	if (it.GetIndex () == from + range)
		return MaxUIndex;

	return it.GetIndex ();
}


template <class Type>
UIndex	StableArray<Type>::FindFirst (const std::function<bool (const Type&)>& condition, UIndex from, USize range) const
{
	REF_CHECK ((range == MaxUSize && from <= size) || (range != MaxUSize && from + range <= size));

	if (range == MaxUSize)
		range = size - from;

	ConstIterator it = GS::Find (Begin () + from, Begin () + from + range, condition);

	if (it.GetIndex () == from + range)
		return MaxUIndex;

	return it.GetIndex ();
}


template <class Type>
UIndex	StableArray<Type>::FindLast (const Type& item) const
{
	NULL_REF_CHECK (item);

	for (UIndex i = size; i > 0; i--) {
		if (*mapTable[i - 1] == item)
			return i - 1;
	}

	return MaxUIndex;
}


template <class Type>
UIndex	StableArray<Type>::FindLast (const std::function<bool (const Type&)>& condition) const
{
	for (UIndex i = size; i > 0; i--) {
		if (condition (*mapTable[i - 1]))
			return i - 1;
	}

	return MaxUIndex;
}


template <class Type>
bool	StableArray<Type>::Contains (const Type& item) const
{
	NULL_REF_CHECK (item);

	return (GS::Find (Begin (), End (), item) != nullptr);
}


template <class Type>
bool	StableArray<Type>::Contains (const std::function<bool (const Type&)>& condition) const
{
	return (GS::Find (Begin (), End (), condition) != nullptr);
}


template <class Type>
bool	StableArray<Type>::Contains (std::initializer_list<Type> items) const
{
	for (const auto& item : items) {
		if (!Contains (item))
			return false;
	}

	return true;
}


template <class Type>
bool	StableArray<Type>::ContainsOnly (std::initializer_list<Type> items) const
{
	if (size != items.size ())
		return false;

	for (const auto& item : items) {
		if (!Contains (item))
			return false;
	}

	return true;
}


template <class Type>
USize	StableArray<Type>::Count (const Type& item, UIndex from, USize range) const
{
	NULL_REF_CHECK (item);
	REF_CHECK ((range == MaxUSize && from <= size) || (range != MaxUSize && from + range <= size));

	if (range == MaxUSize)
		range = size - from;

	return GS::Count (Begin () + from, Begin () + from + range, item);
}


template <class Type>
USize	StableArray<Type>::Count (const std::function<bool (const Type&)>& condition, UIndex from, USize range) const
{
	REF_CHECK ((range == MaxUSize && from <= size) || (range != MaxUSize && from + range <= size));

	if (range == MaxUSize)
		range = size - from;

	return GS::Count (Begin () + from, Begin () + from + range, condition);
}


template <class Type>
bool	StableArray<Type>::operator== (const StableArray& rightOp) const
{
	NULL_REF_CHECK (rightOp);

	if (size != rightOp.size)
		return false;

	return (GS::IsEqual (Begin (), End (), rightOp.Begin ()));
}



template <class Type>
bool	StableArray<Type>::operator!= (const StableArray& rightOp) const
{
	NULL_REF_CHECK (rightOp);

	return !operator== (rightOp);
}


template <class Type>
USize	StableArray<Type>::GetCapacity (void) const
{
	return capacity;
}


template <class Type>
double	StableArray<Type>::GetFillFactor (void) const
{
	if (capacity == 0)
		return 0;
	else
		return (static_cast<double> (size) / static_cast<double> (capacity));
}


template <class Type>
inline USize	StableArray<Type>::GetPageSize (void) const
{
	return (1 << log2PageSize);
}


template <class Type>
typename StableArray<Type>::AllocationStrategy	StableArray<Type>::GetAllocationStrategy (void) const
{
	return static_cast<AllocationStrategy> (allocationStrategy);
}


template <class Type>
typename StableArray<Type>::CompactionStrategy	StableArray<Type>::GetCompactionStrategy (void) const
{
	return static_cast<CompactionStrategy> (compactionStrategy);
}


template <class Type>
void	StableArray<Type>::Compact (void)
{
	if (size + allocationUnit * GetPageSize () > capacity)
		return;	// no page can be deleted

	StableArray compactedArray = *this;
	this->Swap (compactedArray);
}


template <class Type>
GSErrCode	StableArray<Type>::Read (IChannel& ic)
{
	NULL_REF_CHECK (ic);

	Clear ();

	GS::InputFrame frame (ic, StableArrayPrivate::classInfo);
	if (frame.GetStatus () != NoError)
		return frame.GetStatus ();

	USize newSize;
	GSErrCode errorCode = ic.Read (newSize);
	if (errorCode == NoError)
		SetSize (newSize);

	for (UIndex i = 0; i < newSize && errorCode == NoError; i++)
		errorCode = GS::ReadType (ic, *mapTable[i]);

	return errorCode;
}


template <class Type>
GSErrCode	StableArray<Type>::Write (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	GS::OutputFrame frame (oc, StableArrayPrivate::classInfo);

	GSErrCode errorCode = oc.Write (size);

	for (UIndex i = 0; i < size && errorCode == NoError; i++)
		errorCode = GS::WriteType (oc, *mapTable[i]);

	return errorCode;
}


template <class Type>
GSErrCode	StableArray<Type>::Read (IChannel& ic, const std::function<GSErrCode (IChannel&, Type&)>& reader)
{
	NULL_REF_CHECK (ic);

	Clear ();

	GS::InputFrame frame (ic, StableArrayPrivate::classInfo);
	if (frame.GetStatus () != NoError)
		return frame.GetStatus ();

	USize newSize;
	GSErrCode errorCode = ic.Read (newSize);
	if (errorCode == NoError)
		SetSize (newSize);

	for (UIndex i = 0; i < newSize && errorCode == NoError; i++)
		errorCode = reader (ic, *mapTable[i]);

	return errorCode;
}


template <class Type>
GSErrCode	StableArray<Type>::Write (OChannel& oc, const std::function<GSErrCode (OChannel&, const Type&)>& writer) const
{
	NULL_REF_CHECK (oc);

	GS::OutputFrame frame (oc, StableArrayPrivate::classInfo);

	GSErrCode errorCode = oc.Write (size);

	for (UIndex i = 0; i < size && errorCode == NoError; i++)
		errorCode = writer (oc, *mapTable[i]);

	return errorCode;
}


template <class Type>
void	StableArray<Type>::Print (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	oc << "StableArray" << '\n';
}


template <class Type>
void	StableArray<Type>::Dump (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	const char* allocationStrategyText = nullptr;
	switch (allocationStrategy) {
		case Constant:
			allocationStrategyText = "Constant";
			break;

		case Linear:
			allocationStrategyText = "Linear";
			break;

		case Exponential:
			allocationStrategyText = "Exponential";
			break;

		default:
			DBBREAK ();
			break;
	}

	const char* compactionStrategyText = nullptr;
	switch (compactionStrategy) {
		case Manual:
			compactionStrategyText = "Manual";
			break;

		case AtQuarter:
			compactionStrategyText = "AtQuarter";
			break;

		default:
			DBBREAK ();
			break;
	}

	oc << "mapTable:           " << mapTable          << '\n';
	oc << "mapTableCapacity:   " << mapTableCapacity  << '\n';
	oc << "pageTable:          " << pageTable << '\n';
	oc << "pageTableCapacity:  " << pageTableCapacity << '\n';
	oc << "allocationStrategy: " << allocationStrategyText << '\n';
	oc << "log2PageSize:       " << static_cast<USize> (log2PageSize) << '\n';
	oc << "offsetMask:         " << offsetMask << '\n';
	oc << "capacity:           " << capacity << '\n';
	oc << "allocationUnit:     " << allocationUnit << '\n';
	oc << "size:               " << size << '\n';
	oc << "compactionStrategy: " << compactionStrategyText << '\n';
}


template <class Type>
bool	StableArray<Type>::CheckState (void) const
{
	if ((mapTable == nullptr && (mapTableCapacity != 0 || pageTable != nullptr || capacity != 0 || allocationUnit != 0 || pageTableCapacity != 0 || size != 0)) ||
		(mapTable != nullptr && (mapTableCapacity == 0 || pageTable == nullptr || capacity == 0 || pageTableCapacity == 0 || size == 0)) ||
		mapTableCapacity < capacity ||
		offsetMask != (1 << log2PageSize) - 1 ||
		capacity < size ||
		capacity < allocationUnit)
		return false;

	AllocationStrategy allocStrategy = static_cast<AllocationStrategy> (allocationStrategy);
	if ((allocStrategy == Linear && capacity != (allocationUnit * (allocationUnit + 1) / 2) * GetPageSize ()) ||
		(allocStrategy == Exponential && capacity != 0 && capacity != (allocationUnit * 2 - 1) * GetPageSize ()))
		return false;

	if (static_cast<CompactionStrategy> (compactionStrategy) == AtQuarter && size < capacity >> 2 && GetPageNumber () > 1)
		return false;

	for (UIndex i = 0; i < capacity; i++) {
		if (mapTable[i] == nullptr)
			return false;
	}
	for (UIndex j = capacity; j < mapTableCapacity; j++) {
		if (mapTable[j] != nullptr)
			return false;
	}

	USize pageNumber = GetPageNumber ();
	for (UIndex m = 0; m < pageNumber; m++) {
		if (pageTable[m] == nullptr)
			return false;
	}
	for (UIndex n = pageNumber; n < pageTableCapacity; n++) {
		if (pageTable[n] != nullptr)
			return false;
	}

	return true;
}


template <class Type>
inline Type&	StableArray<Type>::Cell (UIndex idx)
{
	return *(pageTable[idx >> log2PageSize] + (idx & offsetMask));
}


template <class Type>
inline const Type&	StableArray<Type>::Cell (UIndex idx) const
{
	return *(pageTable[idx >> log2PageSize] + (idx & offsetMask));
}


template <class Type>
inline USize	StableArray<Type>::GetPageNumber (void) const
{
	return (capacity / GetPageSize ());
}


template <class Type>
inline void		StableArray<Type>::IncreaseCapacity (USize minCapacity)
{
	if (minCapacity > capacity)
		AddNewPages (minCapacity);
}


template <class Type>
void	StableArray<Type>::AddNewPages (USize minCapacity)
{
	if (minCapacity <= capacity)
		return;		// there is no need to add new pages

	USize pageSize   = GetPageSize ();
	USize pageNumber = GetPageNumber ();

	USize minPageNumber = (minCapacity + pageSize - 1) >> log2PageSize;
	USize growth = allocationUnit;
	USize newPageNumber = pageNumber;
	while (newPageNumber < minPageNumber) {
		growth = GetNextAllocationUnit (growth);
		newPageNumber += growth;
	}

	USize newCapacity = newPageNumber * pageSize;

	if (newPageNumber > pageTableCapacity) {
		USize newPageTableCapacity = Max (newPageNumber, pageTableCapacity * 2);
		Type** newPageTable = reinterpret_cast<Type**> (AllocateBuffer (newPageTableCapacity * sizeof (Type*)));
		if (pageNumber > 0)
			::BNCopyMemory (newPageTable, pageTable, pageNumber * sizeof (Type*));
		DeleteBuffer (reinterpret_cast<char*> (pageTable));
		pageTable = newPageTable;
		pageTableCapacity = newPageTableCapacity;

		for (UIndex k = newPageNumber; k < pageTableCapacity; k++)
			pageTable[k] = nullptr;
	}

	UIndex i = pageNumber;
	while (i < newPageNumber) {
		allocationUnit = GetNextAllocationUnit (allocationUnit);
		Type* buffer = reinterpret_cast<Type*> (AllocateBuffer (allocationUnit * pageSize * sizeof (Type)));
		for (UIndex j = 0; j < allocationUnit; j++)
			pageTable[i + j] = buffer + j * pageSize;
		i += allocationUnit;
	}

	if (newCapacity > mapTableCapacity) {
		USize newMapTableCapacity = Max (newCapacity, mapTableCapacity * 2);

		Type** newMapTable = reinterpret_cast<Type**> (AllocateBuffer (newMapTableCapacity * sizeof (Type*)));
		if (capacity > 0)
			::BNCopyMemory (newMapTable, mapTable, capacity * sizeof (Type*));
		DeleteBuffer (reinterpret_cast<char*> (mapTable));
		mapTable = newMapTable;
		mapTableCapacity = newMapTableCapacity;

#if defined DEBUVERS
		for (UIndex n = newCapacity; n < mapTableCapacity; n++)
			mapTable[n] = nullptr;
#endif
	}

	for (UIndex m = capacity; m < newCapacity; m++)
		mapTable[m] = &Cell (m);

	capacity = newCapacity;
}


template <class Type>
inline void		StableArray<Type>::DecreaseCapacity (USize minCapacity)
{
	if (minCapacity == 0) {
		Clear ();
		return;
	}

	switch (compactionStrategy) {
		case Manual:
			break;

		case AtQuarter:
			if (size <= capacity >> 2 && GetPageNumber () > 1)
				Compact ();
			break;

		default:
			DBBREAK ();
			break;
	}
}


template <class Type>
inline ULong	StableArray<Type>::GetNextAllocationUnit (ULong currentAllocationUnit) const
{
	switch (allocationStrategy) {
		case Constant:
			return 1;

		case Linear:
			return currentAllocationUnit + 1;

		case Exponential:
			return (currentAllocationUnit == 0 ? 1 : currentAllocationUnit << 1);

		default:
			DBBREAK ();
			return 1;
	}
}


template <class Type>
inline ULong	StableArray<Type>::GetPrevAllocationUnit (ULong currentAllocationUnit) const
{
	switch (allocationStrategy) {
		case Constant:
			return 1;

		case Linear:
			return (currentAllocationUnit == 0 ? 0 : currentAllocationUnit - 1);

		case Exponential:
			return currentAllocationUnit >> 1;

		default:
			DBBREAK ();
			return 1;
	}
}


template <class Type>
char*	StableArray<Type>::AllocateBuffer (USize bufferCapacity)
{
	return new char[bufferCapacity];	// buffer_overrun_reviewed_0
}


template <class Type>
void	StableArray<Type>::DeleteBuffer (char* buffer)
{
	delete [] buffer;
}


template <class Type>
inline ULong	StableArray<Type>::ComputeLog2 (ULong n)
{
	PRECOND (n > 0);

	n += n - 1;	// to reach the next power of 2 (if it isn't power of 2)

	ULong log2 = 0;
	while (n > 1) {
		n = n >> 1;
		log2++;
	}

	return log2;
}


template <class Type>
inline StableArray<Type>::ConstEnumerator::ConstEnumerator (const StableArray& source):
	source (&source)
{
}


template <class Type>
void	StableArray<Type>::ConstEnumerator::Enumerate (const std::function<void (const Type&)>& processor) const
{
	source->Enumerate (processor);
}


template <class Type>
inline StableArray<Type>::ConversionEnumerator::ConversionEnumerator (const StableArray& source):
	source (&source)
{
}


template <class Type>
void	StableArray<Type>::ConversionEnumerator::Enumerate (const std::function<void (const Type&)>& processor) const
{
	source->Enumerate (processor);
}

// === class StableArrayInserter =======================================================================================

template<class Type>
class StableArrayInserter {
	StableArray<Type>* array;

public:
	explicit StableArrayInserter (StableArray<Type>* array) :
		array (array)
	{
	}

	void operator() (const Type& item) const { array->Push (item); }
	void operator() (Type&& item)      const { array->Push (std::move (item)); }

	typedef Type DecayedFirstArgumentType;
};


// === Global functions ================================================================================================

template <class Type>
inline auto		begin (StableArray<Type>& stableArray)
{
	return stableArray.Begin ();
}


template <class Type>
inline auto		end (StableArray<Type>& stableArray)
{
	return stableArray.End ();
}


template <class Type>
inline auto		begin (const StableArray<Type>& stableArray)
{
	return stableArray.Begin ();
}


template <class Type>
inline auto		end (const StableArray<Type>& stableArray)
{
	return stableArray.End ();
}


template <class Type>
inline std::function<void (const Type&)>	GetInserter (StableArray<Type>& destination)
{
	return [&] (const Type& item) { destination.Push (item); };
}


template <class Type>
inline GSErrCode	Read (IChannel& ic, StableArray<Type>& obj)
{
	return obj.Read (ic);
}


template <class Type>
inline GSErrCode	Write (OChannel& oc, const StableArray<Type>& obj)
{
	return obj.Write (oc);
}


template <class Type>
inline GSErrCode	Print (OChannel& oc, const StableArray<Type>& obj)
{
	obj.Print (oc);

	return NoError;
}

}	// namespace GS
// _____________________________ Class StableArray _____________________________


#endif
