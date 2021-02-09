// contact person : KiP
#ifndef INFINITE_CONE_HPP
#define INFINITE_CONE_HPP
#pragma once

// === Includes ========================================================================================================

#include "Line3D.hpp"
#include "CoordinateSystem3D.hpp"

// =====================================================================================================================

namespace Geometry {

DECLARE_EXCEPTION_CLASS (InfiniteConeCreationError, GeometryException, Error, GEOMETRY_DLL_EXPORT);

class GEOMETRY_DLL_EXPORT InfiniteCone
{
public:
	InfiniteCone (const Point3D& apex, const UnitVector_3D& axisDir, double openingAngle);
	virtual ~InfiniteCone ();
	
	Point3D						GetApex () const;
	UnitVector_3D				GetAxisDirection () const;
	// Ray3D					GetAxis () const;	#KiP_A180_GeometryRefactor Ray3D class kellene
	double						GetOpeningAngle () const;
	double						GetHalfOpeningAngle () const;

	bool						IsOnSurface (const Point3D& point) const;
	bool						IsAxisPoint (const Point3D& point) const;
	bool						ClosestPointIsApex (const Point3D& point) const;
	GS::Optional<Point3D>		GetClosestPoint (const Point3D& point) const;		// result is NoValue for axis points (except for apex)
	double						GetDistance (const Point3D& point) const;			// result is NoValue for axis points (except for apex)
	
	GS::Optional<UnitVector_3D>	GetNormalAtPoint (const Point3D& conePoint) const;

private:
	OrthonormalizedCoordinateSystem3D lcs;
	double halfAngle;
};

}
#endif
