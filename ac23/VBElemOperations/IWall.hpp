// *********************************************************************************************************************
// Description:		Wall interface for NWall junction conversion
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************

#if !defined (VBELEMOPERATIONSWALLINTERFACE_HPP)
#define VBELEMOPERATIONSWALLINTERFACE_HPP

#pragma once

// --- Includes	--------------------------------------------------------------------------------------------------------
#include "VBElements/Wall.hpp"
#include "VBElements/CWConnectionListIterator.hpp"
#include "VBElemOperationsExport.hpp"

namespace VBD {



class VB_ELEM_OPERATIONS_DLL_EXPORT IWall
{
protected:
	class X {};
public:

	virtual const Coord&						GetBegC			() const = 0;
	virtual const Coord&						GetEndC			() const = 0;
	virtual char								GetKind			() const = 0;
	virtual bool								IsWallKindPoly	(void) const = 0;
	virtual bool								IsWallKindLine  (void) const = 0;
	virtual Geometry::IrregularPolygon2D		GetCorePolygon	() const = 0;
	virtual const Geometry::IrregularPolygon2D&	GetCorePolygonRef () const = 0;
	virtual Int32								GetRLinInd		() const = 0;
	virtual Int32								GetRLinEndInd	() const = 0;
	virtual bool								IsFlipped		() const = 0;
	virtual double								GetAngle		() const = 0;
	virtual double								GetAbsoluteAltitude () const = 0;
	virtual double								GetHeight () const = 0;

	virtual const IWall*						operator->		() const = 0;
	virtual bool								operator==		(const X*) const = 0;
	virtual bool								operator!=		(const X* x) const = 0;

	virtual ~IWall ();

};		//class IWall


}		//namespace VBD

#endif