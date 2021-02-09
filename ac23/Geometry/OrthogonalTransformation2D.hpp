#ifndef ORTHOGAONALTRANSFORMATION_2D_HPP
#define ORTHOGAONALTRANSFORMATION_2D_HPP

#pragma once

#include "Matrix2.hpp"
#include "CoordTypedef.hpp"

namespace Geometry {

class Line2D;
class OrthonormalizedCoordinateSystem2D;

class GEOMETRY_DLL_EXPORT OrthogonalTransformation2D {
private:
	Matrix22 matrix;
	Vector2D offset;

public:
	static const OrthogonalTransformation2D Identity;
	OrthogonalTransformation2D (void);

	bool IsIdentity () const;
	bool IsTranslationOnly () const;
	bool HasMirror () const;
	bool HasTranslation () const;

	Matrix22 GetMatrix () const;
	Vector2D GetOffset () const;
	OrthogonalTransformation2D GetInverse () const;

	void SetOffset (const Vector2D& newOffset);

	static OrthogonalTransformation2D CreateIdentity ();
	static OrthogonalTransformation2D CreateTranslation (const Vector2D& offset);
	static OrthogonalTransformation2D CreateOrigoRotation (double angle);
	static OrthogonalTransformation2D CreateRotation (const Point2D& origo, double angle);
	static OrthogonalTransformation2D CreateMirrorX ();
	static OrthogonalTransformation2D CreateMirrorY ();
	static OrthogonalTransformation2D CreateMirror (const Line2D& mirrorAxis);
	static OrthogonalTransformation2D CreateGlobalToLocal (const OrthonormalizedCoordinateSystem2D& cs);
	static OrthogonalTransformation2D CreateLocalToGlobal (const OrthonormalizedCoordinateSystem2D& cs);

	OrthogonalTransformation2D& AddTranslation (const Vector2D& offset);
	OrthogonalTransformation2D& AddOrigoRotation (double angle);
	OrthogonalTransformation2D& AddRotation (const Point2D& origo, double angle);
	OrthogonalTransformation2D& AddMirrorX ();
	OrthogonalTransformation2D& AddMirrorY ();
	OrthogonalTransformation2D& AddMirror (const Line2D& mirrorAxis);
	OrthogonalTransformation2D& Add (const OrthogonalTransformation2D& other);

	OrthogonalTransformation2D operator* (const OrthogonalTransformation2D& rhs) const;

	Point2D Apply (const Point2D& p) const;
};

}

#endif