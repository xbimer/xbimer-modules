
// *****************************************************************************
//
//                                 Class IndexTable
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	KKr
//
// SG compatible
//
// *****************************************************************************

#if !defined INDEXTABLE_HPP
#define INDEXTABLE_HPP

#pragma once

// --------------------------------- Includes ----------------------------------

#include "Container.hpp"
#include "PagedArray.hpp"
#include "HashTable.hpp"

// =============================== Class IndexTable ===============================

namespace GS {

template <class Type>
class IndexTable : public Container<Type> {
public:
	class TypeRef {
		const Type* value;

	public:
		TypeRef (const Type* item = nullptr);
		bool	operator== (const TypeRef& rightOp) const;
		operator GS::HashValue () const;
	};

private:
	GS::PagedArray<Type>		items;
	GS::HashTable<TypeRef, UInt32> indexTable;

public:
	UInt32	Add (const Type& item);
	void	Clear ();

	GS_FORCE_INLINE Type&			operator[] (UInt32 idx);
	GS_FORCE_INLINE const Type&		operator[] (UInt32 idx) const;
};

template <class Type>
IndexTable<Type>::TypeRef::TypeRef (const Type* item): value (item)
{
}


template <class Type>
bool	IndexTable<Type>::TypeRef::operator == (const TypeRef& rightOp) const
{
	return *value == *rightOp.value;
}


template <class Type>
IndexTable<Type>::TypeRef::operator GS::HashValue () const
{
	return GS::GenerateHashValue (*value);
}



template <class Type>
UInt32	IndexTable<Type>::Add (const Type& item)
{
	if (!indexTable.ContainsKey (&item)) {
		items.Push (item);
		indexTable.Add (&items[items.GetLastIndex ()], items.GetLastIndex ());
	}

	return indexTable[&item];
}


template <class Type>
void	IndexTable<Type>::Clear ()
{
	indexTable.Clear ();
	items.Clear ();
}


template <class Type>
Type&		IndexTable<Type>::operator[] (UInt32 idx)
{
	return items[idx];
}


template <class Type>
const Type&		IndexTable<Type>::operator[] (UInt32 idx) const
{
	return items[idx];
}

} // namespace GS
// _______________________________ Class IndexTable _______________________________


#endif