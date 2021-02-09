// contact person : KiP

#ifndef COORDINATE_SYSTEM_3D_HPP
#define COORDINATE_SYSTEM_3D_HPP

#pragma once

// === Includes ========================================================================================================

// from Geometry
#include "Matrix34.hpp"
#include "UnitVector3D.hpp"

// from GSRoot
#include "ChecksumUpdater.hpp"

// =====================================================================================================================

namespace Geometry {

class CoordinateSystem3D;
class GeneralCoordinateSystem3D;
class OrthogonalTransformation3D;
class Transformation3D;
class OrthonormalizedCoordinateSystem3D;

class GEOMETRY_DLL_EXPORT CoordinateSystem3D
{
public:
	class GEOMETRY_DLL_EXPORT CreationFailed : public GeometryException 
	{
	public:
		virtual ~CreationFailed ();
	};

	virtual ~CoordinateSystem3D ();

	Point3D		LocalToGlobal (const Point3D& localCoord) const;
	Point3D		GlobalToLocal (const Point3D& globalCoord) const;
	Vector3D	LocalToGlobal_V (const Vector3D& localVector) const;
	Vector3D	GlobalToLocal_V (const Vector3D& globalVector) const;

	bool											IsOrthonormalized () const;
	GS::Optional<OrthonormalizedCoordinateSystem3D> ToOrthonotmalized () const;
	GeneralCoordinateSystem3D						ToGeneral () const;

	void		WriteCoreContentForChecksum (GS::ChecksumUpdater& updater) const;
protected:
	CoordinateSystem3D (const Point3D& origo, const Vector3D& xAxis, const Vector3D& yAxis, const Vector3D& zAxis);

	Coord3D		GetOrigoPrivate () const;	
	Vector3D	GetXAxisPrivate () const;
	Vector3D	GetYAxisPrivate () const;
	Vector3D	GetZAxisPrivate () const;

private:
	Coord3D origo;
	Vector3D xAxis;
	Vector3D yAxis;
	Vector3D zAxis;

	Matrix34 localToGlobalTrafo;
	Matrix34 globalToLocalTrafo;
};


class GEOMETRY_DLL_EXPORT GeneralCoordinateSystem3D : public CoordinateSystem3D
{
public:
	GeneralCoordinateSystem3D (const Coord3D& origo, const Vector3D& xAxis, const Vector3D& yAxis, const Vector3D& zAxis)	CAN_THROW (CoordinateSystem3D::CreationFailed);
	virtual ~GeneralCoordinateSystem3D ();

	Coord3D		GetOrigo () const;	
	Vector3D	GetXAxis () const;
	Vector3D	GetYAxis () const;
	Vector3D	GetZAxis () const;

	Transformation3D GetLocalToGlobalTrafo () const;
	Transformation3D GetGlobalToLocalTrafo () const;

};


class GEOMETRY_DLL_EXPORT OrthonormalizedCoordinateSystem3D : public CoordinateSystem3D
{
public:
	static OrthonormalizedCoordinateSystem3D CreateGlobal ();
	static OrthonormalizedCoordinateSystem3D Create (const Coord3D& origo, const Vector3D& zAxisDir, const Vector3D& xAxisCandidate, bool mirrored = false)		CAN_THROW (CoordinateSystem3D::CreationFailed);
	static OrthonormalizedCoordinateSystem3D Create (const Coord3D& origo, const UnitVector_3D& xAxis, const UnitVector_3D& yAxis, const UnitVector_3D& zAxis)	CAN_THROW (CoordinateSystem3D::CreationFailed);

	virtual ~OrthonormalizedCoordinateSystem3D ();

	Coord3D			GetOrigo () const;
	UnitVector_3D	GetXAxisUnitVector () const;
	UnitVector_3D	GetYAxisUnitVector () const;
	UnitVector_3D	GetZAxisUnitVector () const;

	OrthogonalTransformation3D GetLocalToGlobalTrafo () const;
	OrthogonalTransformation3D GetGlobalToLocalTrafo () const;

private:
	OrthonormalizedCoordinateSystem3D (const Coord3D& origo, const UnitVector_3D& xAxis, const UnitVector_3D& yAxis, const UnitVector_3D& zAxis);
};

}
#endif
