#ifndef TRANSFORMATION_3D_HPP
#define TRANSFORMATION_3D_HPP

#pragma once

#include "Matrix3.hpp"

namespace Geometry {

class OrthogonalTransformation3D;
class GeneralCoordinateSystem3D;
class Line3D;
class Plane3D;

class GEOMETRY_DLL_EXPORT Transformation3D {
private:
	Matrix33 matrix;
	Vector3D offset;
public:
	static const Transformation3D Identity;
	Transformation3D (void);
	Transformation3D (const OrthogonalTransformation3D& other);

	bool IsSingular () const;
	bool IsIdentity () const;
	bool IsTranslationOnly () const;
	bool HasMirror () const;
	bool HasScaling () const;
	bool HasTranslation () const;

	Matrix33 GetMatrix () const;
	Vector3D GetOffset () const;
	Transformation3D GetInverse () const;

	void SetOffset (const Vector3D& newOffset);
	void SetMatrix (const Matrix33& newMatrix);

	static Transformation3D CreateIdentity ();
	static Transformation3D CreateTranslation (const Vector3D& offset);
	static Transformation3D CreateRotationX (double angle);
	static Transformation3D CreateRotationY (double angle);
	static Transformation3D CreateRotationZ (double angle);
	static Transformation3D CreateRotation (const Line3D& axis, double angle);
	static Transformation3D CreateMirrorXY ();
	static Transformation3D CreateMirrorYZ ();
	static Transformation3D CreateMirrorZX ();
	static Transformation3D CreateMirror (const Plane3D& mirrorPlane);
	static Transformation3D CreateScaling (double xScale, double yScale, double zScale);
	static Transformation3D CreateGlobalToLocal (const GeneralCoordinateSystem3D& cs);
	static Transformation3D CreateLocalToGlobal (const GeneralCoordinateSystem3D& cs);

	Transformation3D& AddTranslation (const Vector3D& offset);
	Transformation3D& AddRotationX (double angle);
	Transformation3D& AddRotationY (double angle);
	Transformation3D& AddRotationZ (double angle);
	Transformation3D& AddRotation (const Line3D& axis, double angle);
	Transformation3D& AddMirrorXY ();
	Transformation3D& AddMirrorYZ ();
	Transformation3D& AddMirrorZX ();
	Transformation3D& AddMirror (const Plane3D& mirrorPlane);
	Transformation3D& AddScaling (double xScale, double yScale, double zScale);
	Transformation3D& Add (const Transformation3D& other);

	Transformation3D operator*(const Transformation3D& rhs) const;

	Point3D Apply (const Point3D& p) const;
};

}
#endif
