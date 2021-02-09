#ifndef COMPONENTINDEX_HPP
#define COMPONENTINDEX_HPP
#pragma once

#include "VBElemOperationsExport.hpp"

class  VB_ELEM_OPERATIONS_DLL_EXPORT ComponentIndex {
private:
	enum class Type {		
		CompositeSkinIndex,
		ProfilePolygonIndex,
		EntireElement,
		BasicStructure,
		OtherColumnWrapping,
		FinishColumnWrapping
	};

	Type type;
	UInt32 index;

	ComponentIndex (Type type, UInt32 index) : type (type), index (index)
	{
	}

public:	

	bool IsEntireElement () const
	{
		return *this == EntireElement;
	}

	UInt32 GetProfilePolygonIndex () const
	{
		DBASSERT (type == Type::ProfilePolygonIndex);
		return index;
	}

	short GetSkinIndex () const
	{
		DBASSERT (type == Type::CompositeSkinIndex);
		return static_cast<short> (index);
	}

	static ComponentIndex FromProfilePolygonIndex (UInt32 index)
	{
		return ComponentIndex (Type::ProfilePolygonIndex, index);
	}

	static ComponentIndex FromCompositeSkinIndex (short index)
	{
		DBASSERT (index >= 0);
		return ComponentIndex (Type::CompositeSkinIndex, index);
	}

	bool operator == (const ComponentIndex& other) const
	{
		return type == other.type && index == other.index;
	}

	bool operator != (const ComponentIndex& other) const
	{
		return !operator==(other);
	}

	static const ComponentIndex EntireElement;
	static const ComponentIndex BasicStructure;
	static const ComponentIndex OtherColumnWrapping;
	static const ComponentIndex FinishColumnWrapping;
};

#endif