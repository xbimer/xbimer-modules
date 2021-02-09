#ifndef TRANSFORMATION_2D_HPP
#define TRANSFORMATION_2D_HPP

#pragma once 

#include "Matrix2.hpp"
#include "CoordTypedef.hpp"

namespace Geometry {

class OrthogonalTransformation2D;
class GeneralCoordinateSystem2D;
class Line2D;

class GEOMETRY_DLL_EXPORT Transformation2D {
private:
	Matrix22 matrix;
	Vector2D offset;

public:
	static const Transformation2D Identity;

	Transformation2D (void);
	Transformation2D (const OrthogonalTransformation2D& other);

	bool IsSingular () const;
	bool IsIdentity () const;
	bool IsTranslationOnly () const;
	bool HasMirror () const;
	bool HasScaling () const;
	bool HasTranslation () const;

	Matrix22 GetMatrix () const;
	Vector2D GetOffset () const;
	Transformation2D GetInverse () const;

	void SetMatrix (const Matrix22& newMatrix);
	void SetOffset (const Vector2D& newOffset);

	static Transformation2D CreateIdentity ();
	static Transformation2D CreateTranslation (const Vector2D& offset);
	static Transformation2D CreateOrigoRotation (double angle);
	static Transformation2D CreateRotation (const Point2D& origo, double angle);
	static Transformation2D CreateMirrorX ();
	static Transformation2D CreateMirrorY ();
	static Transformation2D CreateMirror (const Line2D& mirrorAxis);
	static Transformation2D CreateScaling (double xScale, double yScale);
	static Transformation2D CreateGlobalToLocal (const GeneralCoordinateSystem2D& cs);
	static Transformation2D CreateLocalToGlobal (const GeneralCoordinateSystem2D& cs);

	Transformation2D& AddTranslation (const Vector2D& offset);
	Transformation2D& AddOrigoRotation (double angle);
	Transformation2D& AddRotation (const Point2D& origo, double angle);
	Transformation2D& AddScaling (double xScale, double yScale);
	Transformation2D& AddMirrorX ();
	Transformation2D& AddMirrorY ();
	Transformation2D& AddMirror (const Line2D & mirrorAxis);
	Transformation2D& Add (const Transformation2D& other);

	Transformation2D operator* (const Transformation2D& rhs) const;

	Point2D Apply (const Point2D& p) const;
};

}

#endif