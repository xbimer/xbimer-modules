
// *****************************************************************************
//
//                                 Class HashSet
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB
//
// SG compatible
//
// *****************************************************************************


#if !defined HASHSET_HPP
#define HASHSET_HPP

#pragma once

// --------------------------------- Includes ----------------------------------

#include "Container.hpp"

#include "HashCommon.hpp"
#include "Array.hpp"
#include "PagedArray.hpp"
#include "ForwardContainerIterator.hpp"
#include "Enumerator.hpp"
#include "BaseAlgorithms.hpp"
#include "InitializerRefList.hpp"

#include <functional>


// ------------------------------ Predeclarations ------------------------------

namespace GS {

template <class Type>
GSErrCode	PrintType (OChannel& oc, const Type& arg);

}


// =============================== Class HashSet ===============================

namespace GS {

class GSROOT_DLL_EXPORT HashSetPrivate {
public:
	static GS::ClassInfo	classInfo;	// HashSet's private classInfo
};


template <class Type>
class HashSet: public Container<Type> {
private:
	struct HashEntry {
		Type		item;	// stores the item
		HashEntry*	next;	// pointer to the next entry (or nullptr)

		HashEntry (const Type& item, HashEntry* next):
			item (item),
			next (next) {}

		HashEntry (Type&& item, HashEntry* next):
			item (std::move (item)),
			next (next) {}

		void	Move (HashEntry& source)
		{
			item = std::move (source.item);
			next = source.next;
		}
	};

	ArrayFB<HashEntry*, 1>	hashListTable;				// stores pointers to lists of hash entries
	PagedArray<HashEntry>	hashEntryTable;				// stores hash entries (lists of hash entries)
	float					averageSeekFactorLimit;		// specifies limit for the average seek factor above which the hash set should be rehashed (if the minimum fill factor is also reached)
	float					minFillFactor;				// specifies the minimum fill factor above which the rehashing should be considered (depending on the average seek factor)
	ULong					seekWeightSum;				// sum of seek weights (it is very unlikely to exceed MaxULong because in 4GB can fit at most 512 millions items which can have average seek factor 16, but if it yet does exceed then hash set will be not rehashed any more)

	HashEntry*		AccessEntry (const Type& item) const;

	inline void		EnsureAverageSeekFactor	  (void);
	inline void		DecreaseAverageSeekFactor (void);
	inline void		IncreaseAverageSeekFactor (void);
	void			Rehash (USize newHashListTableSize);

	void			SwapPrimitiveMembers (HashSet& rightOp);

		// Iterator support

	struct IteratorState {
		UIndex	current;	// current entry

		explicit IteratorState (UIndex initIndex = 0): current (initIndex) {}
	};
	inline const Type&  GetCurrent (const IteratorState& state) const;
	inline void			Increment  (IteratorState& state) const;
	inline bool			IsEqual    (const IteratorState& state1, const IteratorState& state2) const;
	inline bool			IsEnd      (const IteratorState& state) const;

	friend class ConstForwardContainerIterator<HashSet, IteratorState, Type>;
	friend class ConstGeneralForwardContainerIterator<HashSet, IteratorState, Type>;

public:
		// Types

	typedef ConstForwardContainerIterator<HashSet, IteratorState, Type>			ConstIterator;
	typedef ConstGeneralForwardContainerIterator<HashSet, IteratorState, Type>	ConstGeneralIterator;

	class ConstEnumerator;
	class ConversionEnumerator;

		// Constructors

	HashSet ();
	explicit HashSet (float averageSeekFactorLimit, float minFillFactor = 0.0);
	explicit HashSet (const GS::ConstEnumerator<Type>& enumerator);
	HashSet (const GS::ConversionEnumerator<Type>& enumerator);
	HashSet (std::initializer_list<Type> items);
	HashSet (ByRefSelector, InitializerRefList<Type> items);
	HashSet (const HashSet& source);
	HashSet (HashSet&& source);

	template <class SourceType>
	explicit HashSet (const HashSet<SourceType>& source);

	HashSet&	operator= (const HashSet& source);
	HashSet&	operator= (HashSet&& source);
	HashSet&	operator= (const GS::ConstEnumerator<Type>& enumerator);

	template <class SourceType>
	HashSet&	operator= (const HashSet<SourceType>& source);

   ~HashSet ();
   
	inline	void	Move (HashSet& source);
	void			Move (const std::function<void (Type&&)>& processor);

		// Content management

	bool	Add	   (const Type& item);
	bool	Add	   (Type&& item);
	bool	Delete (const Type& item);

	void	Clear (void);

	void	Swap (HashSet& rightOp);

	inline USize	GetSize (void) const;
	inline bool		IsEmpty (void) const;

		// Content query

	inline ConstIterator			Enumerate (void) const;

	template <class Function>
	inline void						Enumerate	   (Function&& processor) const;
	template <class Function>
	inline void						EnumerateConst (Function&& processor) const;

	inline ConstIterator			Begin (void) const;

	inline ConstIterator			End	(void) const;

	inline ConstGeneralIterator		EnumerateGeneral (void) const;
	inline ConstGeneralIterator		BeginGeneral (void) const;
	inline ConstGeneralIterator		EndGeneral	(void) const;

	inline const Type&				GetTheOnlyItem () const;

	inline operator ConstEnumerator	() const;
	inline ConversionEnumerator		Convert (void) const;

	inline auto&			AsConst (void) const;

	inline bool			Contains (const Type& item) const;
	bool				Contains (std::initializer_list<Type> items) const;

	inline const Type&	Get (const Type& item) const;
	inline bool			Get (const Type& item, Type* result) const;
	inline bool			Get (const Type& item, Type** result);
	inline bool			Get (const Type& item, const Type** result) const;

	void				SetEquivalent (const Type& oldItem, const Type& newItem);
	inline Type&		ModifyValue	  (const Type& item);

	bool				operator== (const HashSet& rightOp) const;
	bool				operator!= (const HashSet& rightOp) const;

		// Set operations

	void	Unify     (const HashSet& rightOp);
	void	Unify     (HashSet&&      rightOp);
	void	Intersect (const HashSet& rightOp);
	void	Subtract  (const HashSet& rightOp);

		// Performance tuning

	void			EnsureCapacity		 (USize minCapacity);

	inline double	GetAverageSeekFactor (void) const;
	ULong			FindMaxSeekFactor	 (void) const;
	inline double	GetFillFactor		 (void) const;
	ULong			CountEmptyLists		 (void) const;

		// I/O

	GSErrCode	Read  (IChannel& ic);
	GSErrCode	Write (OChannel& oc) const;

		// Debug

	void	Print	   (OChannel& oc) const;
	void	Dump	   (OChannel& oc) const;
	bool	CheckState (void) const;

	const Type&	GetAnItem (ULong rnd) const;

		// Types

	class ConstEnumerator: public GS::ConstEnumerator<Type> {
	private:
		const HashSet* source;

	public:
		inline ConstEnumerator (const HashSet& source);

		virtual void	Enumerate (const std::function<void (const Type&)>& processor) const override;
	};

	class ConversionEnumerator: public GS::ConversionEnumerator<Type> {
	private:
		const HashSet* source;

	public:
		inline ConversionEnumerator (const HashSet& source);

		virtual void	Enumerate (const std::function<void (const Type&)>& processor) const override;
	};
};

// === HashSet methods =================================================================================================

template <class Type>
HashSet<Type>::HashSet ():
	HashSet (0.0, 0.0)
{
}


template <class Type>
HashSet<Type>::HashSet (float averageSeekFactorLimit, float minFillFactor /*= 0.0*/)
{
	ARG_CHECK (averageSeekFactorLimit >= 0.0);
	ARG_CHECK (minFillFactor >= 0.0);

	if (averageSeekFactorLimit < 1.0)	// 0.0 indicates the default value, whereas values below 1.0 have no sense
		averageSeekFactorLimit = 2.0;

	if (minFillFactor < 0.01)	// 0.0 indicates the default value, whereas "small" values are unsafe because may cause continuous reallocation in some very unlikely circumstances
		minFillFactor = 0.75;

	this->averageSeekFactorLimit = averageSeekFactorLimit;
	this->minFillFactor			 = minFillFactor;
	this->seekWeightSum			 = 0;
}


template <class Type>
HashSet<Type>::HashSet (std::initializer_list<Type> items):
	HashSet ()
{
	EnsureCapacity (USize (items.size ()));

	for (const auto& item : items)
		Add (item);
}


template <class Type>
HashSet<Type>::HashSet (ByRefSelector, InitializerRefList<Type> items):
	HashSet ()
{
	for (const auto& item : items)
		Add (*item);
}


template <class Type>
HashSet<Type>::HashSet (const GS::ConstEnumerator<Type>& enumerator):
	averageSeekFactorLimit (2.0),
	minFillFactor (0.75),
	seekWeightSum (0)
{
	enumerator.Enumerate (GetInserter (*this));
}


template <class Type>
HashSet<Type>::HashSet (const GS::ConversionEnumerator<Type>& enumerator):
	averageSeekFactorLimit (2.0),
	minFillFactor (0.75),
	seekWeightSum (0)
{
	enumerator.Enumerate (GetInserter (*this));
}


template <class Type>
HashSet<Type>::HashSet (const HashSet& source)
{
	NULL_REF_CHECK (source);

	averageSeekFactorLimit = source.averageSeekFactorLimit;
	minFillFactor		   = source.minFillFactor;
	seekWeightSum		   = 0;

	EnsureCapacity (source.GetSize ());

	USize sourceHashEntryCount = source.hashEntryTable.GetSize ();
	for (UIndex i = 0; i < sourceHashEntryCount; i++)
		Add (source.hashEntryTable[i].item);
}


template <class Type>
HashSet<Type>::HashSet (HashSet&& source):
	averageSeekFactorLimit (2.0),
	minFillFactor (0.75),
	seekWeightSum (0)
{
	Move (source);
}


template <class Type>
HashSet<Type>&	HashSet<Type>::operator= (const HashSet& source)
{
	HashSet copy = source;

	hashListTable.Move (copy.hashListTable);
	hashEntryTable.Move (copy.hashEntryTable);
	SwapPrimitiveMembers (copy);

	return *this;
}


template <class Type>
HashSet<Type>&	HashSet<Type>::operator= (HashSet&& source)
{
	Move (source);

	return *this;
}


template <class Type>
HashSet<Type>&	HashSet<Type>::operator= (const GS::ConstEnumerator<Type>& enumerator)
{
	Clear ();

	enumerator.Enumerate (GetInserter (*this));

	return *this;
}


template <class Type>
HashSet<Type>::~HashSet ()
{
}


template <class Type>
template <class SourceType>
HashSet<Type>::HashSet (const HashSet<SourceType>& source):
	HashSet ()
{
	static_assert (IsConvertible<SourceType, Type>, "'SourceType' is not convertible to 'Type'.");
	NULL_REF_CHECK (source);

	EnsureCapacity (source.GetSize ());

	for (const SourceType& item : source)
		Add (item);
}


template <class Type>
template <class SourceType>
HashSet<Type>&	HashSet<Type>::operator= (const HashSet<SourceType>& source)
{
	HashSet<Type> copy (source);

	hashListTable.Move (copy.hashListTable);
	hashEntryTable.Move (copy.hashEntryTable);
	SwapPrimitiveMembers (copy);

	return *this;
}


template <class Type>
inline	void	HashSet<Type>::Move (HashSet& source)
{
	if (DBERROR (&source == this))
		return;

	hashListTable.Move (source.hashListTable);
	hashEntryTable.Move (source.hashEntryTable);
	SwapPrimitiveMembers (source);

	source.Clear ();
}


template <class Type>
void			HashSet<Type>::Move (const std::function<void (Type&&)>& processor)
{
	for (UIndex i = 0; i < hashEntryTable.GetSize (); ++i)
		processor (std::move (hashEntryTable[i].item));

	Clear ();
}


template <class Type>
bool	HashSet<Type>::Add (const Type& item)
{
	NULL_REF_CHECK (item);

	if (hashListTable.IsEmpty ()) {
		USize firstHashListTableSize = GetNextHashCapacity (0);
		hashListTable.SetCapacity (firstHashListTableSize);	// in this way SetSize will not waste capacity
		hashListTable.SetSize (firstHashListTableSize);
		hashListTable.Fill (nullptr);
	}

	UIndex hashListIndex = GenerateHashValue (item) % hashListTable.GetSize ();
	HashEntry* hashEntry = hashListTable[hashListIndex];
	UIndex hashListLength = 0;
	while (hashEntry != nullptr) {
		if (hashEntry->item == item)
			return false;
		hashListLength++;
		hashEntry = hashEntry->next;
	}
	seekWeightSum += hashListLength + 1;

	hashEntryTable.PushNew (item, hashListTable[hashListIndex]);
	hashListTable[hashListIndex] = &hashEntryTable.GetLast ();

	EnsureAverageSeekFactor ();

	return true;
}


template <class Type>
bool	HashSet<Type>::Add (Type&& item)
{
	NULL_REF_CHECK (item);

	if (hashListTable.IsEmpty ()) {
		USize firstHashListTableSize = GetNextHashCapacity (0);
		hashListTable.SetCapacity (firstHashListTableSize);	// in this way SetSize will not waste capacity
		hashListTable.SetSize (firstHashListTableSize);
		hashListTable.Fill (nullptr);
	}

	UIndex hashListIndex = GenerateHashValue (item) % hashListTable.GetSize ();
	HashEntry* hashEntry = hashListTable[hashListIndex];
	UIndex hashListLength = 0;
	while (hashEntry != nullptr) {
		if (hashEntry->item == item)
			return false;
		hashListLength++;
		hashEntry = hashEntry->next;
	}
	seekWeightSum += hashListLength + 1;

	hashEntryTable.PushNew (std::move (item), hashListTable[hashListIndex]);
	hashListTable[hashListIndex] = &hashEntryTable.GetLast ();

	EnsureAverageSeekFactor ();

	return true;
}


template <class Type>
bool	HashSet<Type>::Delete (const Type& item)
{
	NULL_REF_CHECK (item);

		// deleting the entry

	if (hashEntryTable.IsEmpty ())
		return false;

	UIndex hashListIndex = GenerateHashValue (item) % hashListTable.GetSize ();
	HashEntry* hashEntry = hashListTable[hashListIndex];
	HashEntry* prevHashEntry = nullptr;
	while (hashEntry != nullptr) {
		if (hashEntry->item == item)
			break;
		prevHashEntry = hashEntry;
		hashEntry = hashEntry->next;
	}
	if (hashEntry == nullptr)
		return false;

	if (prevHashEntry == nullptr)
		hashListTable[hashListIndex] = hashEntry->next;
	else
		prevHashEntry->next = hashEntry->next;

		// updating sum of seek weights

	UIndex hashListLength = 0;
	HashEntry* listEntry = hashListTable[hashListIndex];
	while (listEntry != nullptr) {
		hashListLength++;
		listEntry = listEntry->next;
	}
	seekWeightSum -= hashListLength + 1;

		// eliminating the hole from the hash entry table

	HashEntry* lastPhysicalEntry = &hashEntryTable.GetLast ();
	if (hashEntry != lastPhysicalEntry) {
		hashEntry->Move (*lastPhysicalEntry);

		UIndex lpeListIndex = GenerateHashValue (hashEntry->item) % hashListTable.GetSize ();
		HashEntry* he = hashListTable[lpeListIndex];
		HashEntry* prevLPE = nullptr;
		while (he != lastPhysicalEntry) {
			prevLPE = he;
			he = he->next;
		}

		if (prevLPE == nullptr)
			hashListTable[lpeListIndex] = hashEntry;
		else
			prevLPE->next = hashEntry;
	}

	hashEntryTable.DeleteLast ();

	if (GetFillFactor () <= minFillFactor / 4)
		IncreaseAverageSeekFactor ();	// decreases capacity of the hashListTable and increases fill factor

	return true;
}


template <class Type>
void	HashSet<Type>::Clear (void)
{
	hashListTable.Clear ();
	hashEntryTable.Clear ();

	seekWeightSum = 0;
}


template <class Type>
void	HashSet<Type>::Swap (HashSet& rightOp)
{
	NULL_REF_CHECK (rightOp);

	if (this == &rightOp)
		return;

	hashListTable.Swap (rightOp.hashListTable);
	hashEntryTable.Swap (rightOp.hashEntryTable);
	SwapPrimitiveMembers (rightOp);
}


template <class Type>
void	HashSet<Type>::SwapPrimitiveMembers (HashSet& rightOp)
{
	NULL_REF_CHECK (rightOp);

	GS::Swap (averageSeekFactorLimit, rightOp.averageSeekFactorLimit);
	GS::Swap (minFillFactor,		  rightOp.minFillFactor);
	GS::Swap (seekWeightSum,		  rightOp.seekWeightSum);
}


template <class Type>
inline USize	HashSet<Type>::GetSize (void) const
{
	return hashEntryTable.GetSize ();
}


template <class Type>
inline bool		HashSet<Type>::IsEmpty (void) const
{
	return (hashEntryTable.IsEmpty ());
}


template <class Type>
inline typename HashSet<Type>::ConstIterator	HashSet<Type>::Enumerate (void) const
{
	return ConstIterator (*this, IteratorState (0));
}


template <class Type>
template <class Function>
inline void	HashSet<Type>::Enumerate (Function&& processor) const
{
	for (UIndex i = 0; i < hashEntryTable.GetSize (); i++)
		processor (const_cast<const Type&> (hashEntryTable[i].item));
}


template <class Type>
template <class Function>
inline void	HashSet<Type>::EnumerateConst (Function&& processor) const
{
	Enumerate (processor);
}


template <class Type>
inline typename HashSet<Type>::ConstIterator	HashSet<Type>::Begin (void) const
{
	return ConstIterator (*this, IteratorState (0));
}


template <class Type>
inline typename HashSet<Type>::ConstIterator	HashSet<Type>::End (void) const
{
	return ConstIterator (*this, IteratorState (hashEntryTable.GetSize ()));
}


template <class Type>
inline typename HashSet<Type>::ConstGeneralIterator		HashSet<Type>::EnumerateGeneral (void) const
{
	return ConstGeneralIterator (*this, IteratorState (0));
}


template <class Type>
inline typename HashSet<Type>::ConstGeneralIterator		HashSet<Type>::BeginGeneral (void) const
{
	return ConstGeneralIterator (*this, IteratorState (0));
}


template <class Type>
inline typename HashSet<Type>::ConstGeneralIterator		HashSet<Type>::EndGeneral (void) const
{
	return ConstGeneralIterator (*this, IteratorState (hashEntryTable.GetSize ()));
}


template <class Type>
inline const Type&	HashSet<Type>::GetTheOnlyItem (void) const
{
	DBASSERT (hashEntryTable.GetSize () == 1);

	return hashEntryTable[0].item;
}


template <class Type>
inline HashSet<Type>::operator typename HashSet<Type>::ConstEnumerator () const
{
	return ConstEnumerator (*this);
}


template <class Type>
inline typename HashSet<Type>::ConversionEnumerator	HashSet<Type>::Convert (void) const
{
	return ConversionEnumerator (*this);
}


template <class Type>
inline auto&	HashSet<Type>::AsConst (void) const
{
	return reinterpret_cast<const HashSet<typename Container<Type>::template MakeConstPtr<Type>::ConstType>&> (*this);
}


template <class Type>
inline bool		HashSet<Type>::Contains (const Type& item) const
{
	NULL_REF_CHECK (item);

	return (AccessEntry (item) != nullptr);
}


template <class Type>
bool	HashSet<Type>::Contains (std::initializer_list<Type> items) const
{
	for (const auto& item : items) {
		if (!Contains (item))
			return false;
	}

	return true;
}


template <class Type>
inline const Type&		HashSet<Type>::Get (const Type& item) const
{
	NULL_REF_CHECK (item);

	HashEntry* hashEntry = AccessEntry (item);
	REF_CHECK (hashEntry != nullptr);

	return hashEntry->item;
}


template <class Type>
inline bool		HashSet<Type>::Get (const Type& item, Type* result) const
{
	NULL_REF_CHECK (item);
	NULL_PTR_CHECK (result);

	HashEntry* hashEntry = AccessEntry (item);
	if (hashEntry == nullptr)
		return false;

	*result = hashEntry->item;
	return true;
}


template <class Type>
inline bool		HashSet<Type>::Get (const Type& item, Type** result)
{
	NULL_REF_CHECK (item);
	NULL_PTR_CHECK (result);

	HashEntry* hashEntry = AccessEntry (item);
	if (hashEntry == nullptr)
		return false;

	*result = &hashEntry->item;
	return true;
}


template <class Type>
inline bool		HashSet<Type>::Get (const Type& item, const Type** result) const
{
	NULL_REF_CHECK (item);
	NULL_PTR_CHECK (result);

	const HashEntry* hashEntry = AccessEntry (item);
	if (hashEntry == nullptr)
		return false;

	*result = &hashEntry->item;
	return true;
}


template <class Type>
void	HashSet<Type>::SetEquivalent (const Type& oldItem, const Type& newItem)
{
	NULL_REF_CHECK (oldItem);
	NULL_REF_CHECK (newItem);

	DBVERIFY (oldItem == newItem);

	HashEntry* hashEntry = AccessEntry (oldItem);
	REF_CHECK (hashEntry != nullptr);

	hashEntry->item = newItem;
}


template <class Type>
inline Type&	HashSet<Type>::ModifyValue (const Type& item)
{
	NULL_REF_CHECK (item);

	HashEntry* hashEntry = AccessEntry (item);
	REF_CHECK (hashEntry != nullptr);

	return hashEntry->item;
}


template <class Type>
bool	HashSet<Type>::operator== (const HashSet& rightOp) const
{
	NULL_REF_CHECK (rightOp);

	if (hashEntryTable.GetSize () != rightOp.hashEntryTable.GetSize ())
		return false;

	for (UIndex i = 0; i < hashEntryTable.GetSize (); ++i) {
		if (!rightOp.Contains (hashEntryTable[i].item))
			return false;
	}

	return true;
}


template <class Type>
bool	HashSet<Type>::operator!= (const HashSet& rightOp) const
{
	NULL_REF_CHECK (rightOp);

	return !operator== (rightOp);
}


template <class Type>
void	HashSet<Type>::Unify (const HashSet& rightOp)
{
	for (const Type& item : rightOp)
		Add (item);
}


template <class Type>
void	HashSet<Type>::Unify (HashSet&& rightOp)
{
	for (const Type& item : rightOp)
		Add (const_cast<Type&&> (item));
}


template <class Type>
void	HashSet<Type>::Intersect (const HashSet& rightOp)
{
	GS::Array<Type> uncommonItems;
	uncommonItems.EnsureCapacity (GetSize ());

	for (const Type& item : *this) {
		if (!rightOp.Contains (item))
			uncommonItems.Push (item);
	}

	for (const Type& item : uncommonItems)
		Delete (item);
}


template <class Type>
void	HashSet<Type>::Subtract (const HashSet& rightOp)
{
	for (const Type& item : rightOp)
		Delete (item);
}


template <class Type>
void	HashSet<Type>::EnsureCapacity (USize minCapacity)
{
	if (minCapacity <= hashEntryTable.GetSize ())
		return;

	USize optimalListTableSize = static_cast<USize> (static_cast<double> (minCapacity) / (2.0 * averageSeekFactorLimit  - 1.0));
	USize newHashListTableSize = GetNextHashCapacity (GetNextHashCapacity (optimalListTableSize));
	if (newHashListTableSize <= hashListTable.GetSize ())
		return;

	Rehash (newHashListTableSize);
}


template <class Type>
inline double	HashSet<Type>::GetAverageSeekFactor (void) const
{
	if (hashEntryTable.IsEmpty ())
		return 0.0;
	else
		return (static_cast<double> (seekWeightSum) / static_cast<double> (hashEntryTable.GetSize ()));		//lint !e414 Possible division by 0
}


template <class Type>
ULong	HashSet<Type>::FindMaxSeekFactor (void) const
{
	ULong maxSeekFactor = 0;
	USize hashListCount = hashListTable.GetSize ();
	for (UIndex i = 0; i < hashListCount; i++) {
		HashEntry* hashEntry = hashListTable[i];
		ULong seekfactor = 0;
		while (hashEntry != nullptr) {
			seekfactor++;
			hashEntry = hashEntry->next;
		}
		if (seekfactor > maxSeekFactor)
			maxSeekFactor = seekfactor;
	}

	return maxSeekFactor;
}


template <class Type>
inline double	HashSet<Type>::GetFillFactor (void) const
{
	if (hashListTable.GetSize () == 0)
		return 0.0;
	else
		return (static_cast<double> (hashEntryTable.GetSize ()) / static_cast<double> (hashListTable.GetSize ()));
}


template <class Type>
ULong	HashSet<Type>::CountEmptyLists (void) const
{
	ULong emptyListCount = 0;
	USize hashListCount = hashListTable.GetSize ();
	for (UIndex i = 0; i < hashListCount; i++) {
		if (hashListTable[i] == nullptr)
			emptyListCount++;
	}

	return emptyListCount;
}


template <class Type>
GSErrCode	HashSet<Type>::Read (IChannel& ic)
{
	NULL_REF_CHECK (ic);

	Clear ();

	GS::InputFrame frame (ic, HashSetPrivate::classInfo);
	if (frame.GetStatus () != NoError)
		return frame.GetStatus ();

	USize newSize;
	GSErrCode errorCode = ic.Read (newSize);

	if (errorCode == NoError)
		EnsureCapacity (newSize);

	Type item = Type ();
	for (UIndex i = 0; i < newSize && errorCode == NoError; i++) {
		errorCode = GS::ReadType (ic, item);
		if (errorCode == NoError)
			Add (item);
	}

	return errorCode;
}


template <class Type>
GSErrCode	HashSet<Type>::Write (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	GS::OutputFrame frame (oc, HashSetPrivate::classInfo);

	USize hashEntryCount = hashEntryTable.GetSize ();

	GSErrCode errorCode = oc.Write (hashEntryCount);

	for (UIndex i = 0; i < hashEntryCount && errorCode == NoError; i++) {
		GS::WriteType (oc, hashEntryTable[i].item);
		errorCode = oc.GetOutputStatus ();
	}

	return errorCode;
}


template <class Type>
void	HashSet<Type>::Print (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	oc << "HashSet size: " << GetSize () << '\n';
	oc << "---------------------\n";

	UIndex i = 0;
	for (ConstIterator it = Enumerate (); it != nullptr; ++it) {
		char index[32];
		::sprintf (index, "[%04lu]  ", (GS::ULongForStdio) i);
		oc << index;

		GS::PrintType (oc, *it);
		oc << '\n';

		i++;
	}

	oc << '\n';

	oc << "Average seek factor: " << GetAverageSeekFactor () << '\n';
	oc << "Maximum seek factor: " << FindMaxSeekFactor () << '\n';
	oc << "Fill factor:         " << GetFillFactor () << '\n';
	oc << "List count:          " << hashListTable.GetSize () << '\n';
	oc << "Empty list count:    " << CountEmptyLists () << '\n';
}


template <class Type>
void	HashSet<Type>::Dump (OChannel& oc) const
{
	NULL_REF_CHECK (oc);

	oc << "hashListTable:          " << &hashListTable << '\n';
	oc << "hashEntryTable:         " << &hashEntryTable << '\n';
	oc << "averageSeekFactorLimit: " << averageSeekFactorLimit << '\n';
	oc << "minFillFactor:          " << minFillFactor << '\n';
	oc << "seekWeightSum:          " << seekWeightSum << '\n';
}


template <class Type>
bool	HashSet<Type>::CheckState (void) const
{
	if (!hashListTable.CheckState ()  ||
		!hashEntryTable.CheckState ())
		return false;

	if (hashEntryTable.GetSize () > seekWeightSum)
		return false;

	USize computedSize = 0;
	ULong computedSeekWeightSum = 0;
	USize hashListCount = hashListTable.GetSize ();
	for (UIndex i = 0; i < hashListCount; i++) {
		HashEntry* hashEntry = hashListTable[i];
		ULong weight = 1;
		while (hashEntry != nullptr) {
			computedSize++;
			computedSeekWeightSum += weight;
			weight++;
			hashEntry = hashEntry->next;
		}
	}

	if (hashEntryTable.GetSize () != computedSize || seekWeightSum != computedSeekWeightSum)
		return false;

	return true;
}


template <class Type>
const Type&		HashSet<Type>::GetAnItem (ULong rnd) const
{
	PRECOND (!hashEntryTable.IsEmpty ());

	return hashEntryTable[rnd % hashEntryTable.GetSize ()].item;
}


template <class Type>
typename HashSet<Type>::HashEntry*	HashSet<Type>::AccessEntry (const Type& item) const
{
	USize hasListCount = hashListTable.GetSize ();
	if (hasListCount == 0)
		return nullptr;

	HashEntry* hashEntry = hashListTable[GenerateHashValue (item) % hasListCount];

	while (hashEntry != nullptr) {
		if (hashEntry->item == item)
			return hashEntry;
		hashEntry = hashEntry->next;
	}

	return nullptr;
}


template <class Type>
inline void		HashSet<Type>::EnsureAverageSeekFactor	(void)
{
	if (GetFillFactor () < minFillFactor)
		return;

	if (GetAverageSeekFactor () <= averageSeekFactorLimit)
		return;

	DecreaseAverageSeekFactor ();
}


template <class Type>
inline void		HashSet<Type>::DecreaseAverageSeekFactor (void)
{
	Rehash (GetNextHashCapacity (hashListTable.GetSize ()));
}


template <class Type>
inline void		HashSet<Type>::IncreaseAverageSeekFactor (void)
{
	if (hashEntryTable.GetSize () == 0)
		Rehash (0);
	else
		Rehash (GetPreviousHashCapacity (hashListTable.GetSize ()));
}


template <class Type>
void	HashSet<Type>::Rehash (USize newHashListTableSize)
{
	hashListTable.SetCapacity (newHashListTableSize);	// if the new capacity is less than old size then this call has no effect
	hashListTable.SetSize (newHashListTableSize);		// SetSize will not waste capacity because of SetCapacity calls
	hashListTable.SetCapacity (newHashListTableSize);	// if the new capacity is greater than old size then this call has no effect
	hashListTable.Fill (nullptr);

	USize newHashListCount = newHashListTableSize;

	USize hashEntryCount = hashEntryTable.GetSize ();
	for (UIndex i = 0; i < hashEntryCount; i++) {
		HashEntry& hashEntry = hashEntryTable[i];
		UIndex newHashListIndex = GenerateHashValue (hashEntry.item) % newHashListCount;		//lint !e414 Possible division by 0
		hashEntry.next = hashListTable[newHashListIndex];
		hashListTable[newHashListIndex] = &hashEntry;
	}

		// recalculating sum of seek weights

	seekWeightSum = 0;
	for (UIndex j = 0; j < newHashListCount; j++) {
		HashEntry* hashEntry = hashListTable[j];
		ULong weight = 1;
		while (hashEntry != nullptr) {
			seekWeightSum += weight;
			weight++;
			hashEntry = hashEntry->next;
		}
	}
}


template <class Type>
inline const Type&  HashSet<Type>::GetCurrent (const IteratorState& state) const
{
	return hashEntryTable[state.current].item;
}


template <class Type>
inline void		HashSet<Type>::Increment (IteratorState& state) const
{
	state.current++;
}


template <class Type>
inline bool		HashSet<Type>::IsEqual (const IteratorState& state1, const IteratorState& state2) const
{
	if (state1.current >= hashEntryTable.GetSize () && state2.current >= hashEntryTable.GetSize ())
		return true;

	return (state1.current == state2.current);
}


template <class Type>
inline bool		HashSet<Type>::IsEnd (const IteratorState& state) const
{
	return (state.current >= hashEntryTable.GetSize ());
}


template <class Type>
inline HashSet<Type>::ConstEnumerator::ConstEnumerator (const HashSet& source):
	source (&source)
{
}


template <class Type>
void	HashSet<Type>::ConstEnumerator::Enumerate (const std::function<void (const Type&)>& processor) const
{
	source->Enumerate (processor);
}


template <class Type>
inline HashSet<Type>::ConversionEnumerator::ConversionEnumerator (const HashSet& source):
	source (&source)
{
}


template <class Type>
void	HashSet<Type>::ConversionEnumerator::Enumerate (const std::function<void (const Type&)>& processor) const
{
	source->Enumerate (processor);
}


// === class HashSetInserter ===========================================================================================

template <class Type>
class HashSetInserter {
	HashSet<Type>* set;

public:
	explicit HashSetInserter (HashSet<Type>* set) :
		set (set)
	{
	}

	void	operator() (const Type& item) const { set->Add (item); }
	void	operator() (Type&& item)	  const	{ set->Add (std::move (item)); }

	typedef Type DecayedFirstArgumentType;
};


// === Global functions ================================================================================================

template <class Type>
inline auto		begin (HashSet<Type>& hashSet)
{
	return hashSet.Begin ();
}


template <class Type>
inline auto		end (HashSet<Type>& hashSet)
{
	return hashSet.End ();
}


template <class Type>
inline auto		begin (const HashSet<Type>& hashSet)
{
	return hashSet.Begin ();
}


template <class Type>
inline auto		end (const HashSet<Type>& hashSet)
{
	return hashSet.End ();
}


template <class Type>
inline HashSetInserter<Type>	GetInserter (HashSet<Type>& destination)
{
	return HashSetInserter<Type> (&destination);
}


template <class Type>
inline GSErrCode	Read (IChannel& ic, HashSet<Type>& obj)
{
	return obj.Read (ic);
}


template <class Type>
inline GSErrCode	Write (OChannel& oc, const HashSet<Type>& obj)
{
	return obj.Write (oc);
}


template <class Type>
inline GSErrCode	Print (OChannel& oc, const HashSet<Type>& obj)
{
	obj.Print (oc);

	return NoError;
}


template <class Type>
HashSet<Type>	Union (const HashSet<Type>& leftOp, const HashSet<Type>& rightOp)
{
	HashSet<Type> result (leftOp);
	result.Unify (rightOp);
	return result;
}


template <class Type>
HashSet<Type>	Intersection (const HashSet<Type>& leftOp, const HashSet<Type>& rightOp)
{
	HashSet<Type> result (leftOp);
	result.Intersect (rightOp);
	return result;
}


template <class Type>
HashSet<Type>	Subtraction (const HashSet<Type>& leftOp, const HashSet<Type>& rightOp)
{
	HashSet<Type> result (leftOp);
	result.Subtract (rightOp);
	return result;
}

}	// namespace GS
// _______________________________ Class HashSet _______________________________


#endif
