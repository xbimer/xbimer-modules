// contact person : KiP

#ifndef COORDINATE_SYSTEM_2D_HPP
#define COORDINATE_SYSTEM_2D_HPP

#pragma once

// === Includes ========================================================================================================

// from Geometry
#include "Matrix23.hpp"

// =====================================================================================================================

namespace Geometry {

class CoordinateSystem2D;
class GeneralCoordinateSystem2D;
class OrthogonalTransformation2D;
class Transformation2D;
class OrthonormalizedCoordinateSystem2D;


DECLARE_EXCEPTION_CLASS (CoordinateSystemCreationError, GeometryException, Error, GEOMETRY_DLL_EXPORT)


class GEOMETRY_DLL_EXPORT CoordinateSystem2D
{
public:
	const Point2D&	GetOrigo () const;
	const Vector2D&	GetXAxis () const;
	const Vector2D&	GetYAxis () const;

	Point2D		LocalToGlobal (const Point2D& localCoord) const;
	Point2D		GlobalToLocal (const Point2D& globalCoord) const;
	Vector2D	LocalToGlobal (const Vector2D& localVector) const;
	Vector2D	GlobalToLocal (const Vector2D& globalVector) const;

protected:
	bool IsMirrored () const;

	CoordinateSystem2D (const Point2D& origo, const Vector2D& xAxis, const Vector2D& yAxis);

	const Point2D origo;
	const Vector2D xAxis;
	const Vector2D yAxis;

private:
	Geometry::Matrix23 localToGlobal;
	Geometry::Matrix23 globalToLocal;

	void InitMatrices ();
};


class GEOMETRY_DLL_EXPORT GeneralCoordinateSystem2D : public CoordinateSystem2D
{
public:
	static GeneralCoordinateSystem2D Create (const Point2D& origo, const Vector& xAxis, const Vector& yAxis)		CAN_THROW (CoordinateSystemCreationError);
	
	bool IsOrthonormalized () const;
	GS::Optional<OrthonormalizedCoordinateSystem2D> ToOrthonotmalized () const;

	Transformation2D GetLocalToGlobalTrafo () const;
	Transformation2D GetGlobalToLocalTrafo () const;

private:
	GeneralCoordinateSystem2D (const Point2D& origo, const Vector& xAxis, const Vector& yAxis);
};


class GEOMETRY_DLL_EXPORT OrthonormalizedCoordinateSystem2D : public CoordinateSystem2D
{
public:
	static OrthonormalizedCoordinateSystem2D CreateGlobal ();
	static OrthonormalizedCoordinateSystem2D Create (const Point2D& origo, double xAxisAngle, bool mirrored = false);
	static OrthonormalizedCoordinateSystem2D Create (const Point2D& origo, const Vector& xAxis, bool mirrored = false)		CAN_THROW (CoordinateSystemCreationError);

	GeneralCoordinateSystem2D ToGeneral () const;

	OrthogonalTransformation2D GetLocalToGlobalTrafo () const;
	OrthogonalTransformation2D GetGlobalToLocalTrafo () const;

private:
	OrthonormalizedCoordinateSystem2D (const Point2D& origo, const Vector& xAxis, const Vector& yAxis);
};

}
#endif
