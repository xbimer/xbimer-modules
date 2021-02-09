// *********************************************************************************************************************
// Description:		Wall interface for NWall junction conversion
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************

#if !defined (WALLADAPTER_HPP)
#define WALLADAPTER_HPP

#pragma once

#include "IWall.hpp"

namespace VBD {


class VB_ELEM_OPERATIONS_DLL_EXPORT WallAdapter : public IWall
{
private:
	VBElem::WallConstRef	wall;
	const IWall*			iWall;

public:

	virtual const Coord&						GetBegC () const override { return wall->GetBegC (); };
	virtual const Coord&						GetEndC () const override { return wall->GetEndC (); };
	virtual char								GetKind () const override { return wall->GetKind (); };
	virtual bool								IsWallKindLine () const override { return wall->IsWallKindLine (); }
	virtual bool								IsWallKindPoly () const override { return wall->IsWallKindPoly (); }
	virtual Geometry::IrregularPolygon2D		GetCorePolygon () const override { return wall->GetCorePolygon (); }
	virtual const Geometry::IrregularPolygon2D&	GetCorePolygonRef () const override { return wall->GetPolyWallCorePolygon (); }
	virtual Int32								GetRLinInd () const override { return wall->GetRLinInd (); };
	virtual Int32								GetRLinEndInd () const override { return wall->GetRLinEndInd (); };
	virtual bool								IsFlipped () const override { return wall->IsFlipped (); };
	virtual double								GetAngle () const override { return wall->GetAngle (); };
	virtual double								GetAbsoluteAltitude () const override { return wall->GetAbsoluteLevel (VBElem::WLT_ReferenceLevel); };
	virtual double								GetHeight () const override { return wall->GetHeight (); };

	virtual const IWall* operator-> () const override { return iWall; }
	virtual ~WallAdapter ();

	WallAdapter () : iWall (nullptr) {}
	WallAdapter (const VBElem::WallConstRef& wall) : wall (wall) { iWall = wall == nullptr ? nullptr : this; }

	WallAdapter (const WallAdapter& other) : wall (other.wall) { iWall = (wall == nullptr) ? other.iWall : this; }

	WallAdapter& operator= (const WallAdapter& other)
	{
		wall = other.wall;
		iWall = (wall == nullptr) ? other.iWall : this;
		return *this;
	}		//operator=

			// When calling this constructor make sure, that implementation of WallAdapter stays alive long enough. Avoid passing "this" as WallAdapter to another WallAdapter
	explicit WallAdapter (const WallAdapter* iWall) : iWall (iWall) {}



	bool operator== (const WallAdapter& other)
	{
		return wall == nullptr ? iWall == other.iWall : wall == other.wall;
	}		//operator==

	bool operator!= (const WallAdapter& other)
	{
		return !operator== (other);
	}		//operator!= 

	virtual bool operator== (const X*) const override
	{
		return iWall == nullptr;
	}		//operator==

	virtual bool operator!= (const X* x) const override
	{
		return !operator== (x);
	}		//operator!= 

	operator GS::HashValue () const
	{
		if (wall == nullptr) {
			return GS::GenerateHashValue (iWall);
		} else {
			return ODB::GenerateHashValue (wall);
		}
	}		//operator GS::HashValue
};		//class WallAdapter



}

#endif