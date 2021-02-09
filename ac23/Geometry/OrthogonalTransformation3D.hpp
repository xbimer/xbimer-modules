#ifndef ORTHOGONALTRANSFORMATION_3D_HPP
#define ORTHOGONALTRANSFORMATION_3D_HPP

#pragma once

#include "Matrix3.hpp"

namespace Geometry {

class OrthonormalizedCoordinateSystem3D;
class Line3D;
class Plane3D;

class GEOMETRY_DLL_EXPORT OrthogonalTransformation3D {

private:
	Matrix33 matrix;
	Vector3D offset;

public:

	static const OrthogonalTransformation3D Identity;
	OrthogonalTransformation3D (void);

	bool IsIdentity () const;
	bool IsTranslationOnly () const;
	bool HasMirror () const;
	bool HasTranslation ()const;

	Matrix33 GetMatrix () const;
	Vector3D GetOffset () const;
	OrthogonalTransformation3D GetInverse () const;

	void SetOffset (const Vector3D& newOffset);

	static OrthogonalTransformation3D CreateIdentity ();
	static OrthogonalTransformation3D CreateTranslation (const Vector3D& offset);
	static OrthogonalTransformation3D CreateRotationX (double angle);
	static OrthogonalTransformation3D CreateRotationY (double angle);
	static OrthogonalTransformation3D CreateRotationZ (double angle);
	static OrthogonalTransformation3D CreateRotation (const Line3D& axis, double angle);
	static OrthogonalTransformation3D CreateMirrorXY ();
	static OrthogonalTransformation3D CreateMirrorYZ ();
	static OrthogonalTransformation3D CreateMirrorZX ();
	static OrthogonalTransformation3D CreateMirror (const Plane3D& mirrorPlane);
	static OrthogonalTransformation3D CreateGlobalToLocal (const OrthonormalizedCoordinateSystem3D& cs);
	static OrthogonalTransformation3D CreateLocalToGlobal (const OrthonormalizedCoordinateSystem3D& cs);

	OrthogonalTransformation3D& AddTranslation (const Vector3D& offset);
	OrthogonalTransformation3D& AddRotationX (double angle);
	OrthogonalTransformation3D& AddRotationY (double angle);
	OrthogonalTransformation3D& AddRotationZ (double angle);
	OrthogonalTransformation3D& AddRotation (const Line3D& axis, double angle);
	OrthogonalTransformation3D& AddMirrorXY ();
	OrthogonalTransformation3D& AddMirrorYZ ();
	OrthogonalTransformation3D& AddMirrorZX ();
	OrthogonalTransformation3D& AddMirror (const Plane3D& mirrorPlane);
	OrthogonalTransformation3D& Add (const OrthogonalTransformation3D& other);

	OrthogonalTransformation3D operator*(const OrthogonalTransformation3D& rhs) const;

	Point3D Apply (const Point3D& p) const;
	Vector3D Apply_V (const Vector3D& v) const;
};
}

#endif