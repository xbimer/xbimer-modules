#ifndef _LINE_2D_HPP_
#define _LINE_2D_HPP_
#pragma once

// === Includes ========================================================================================================

// from Geometry
#include "GeometricDefinitions.h"
#include "Point2D.hpp"
#include "UnitVector2D.hpp"

// =====================================================================================================================

namespace Geometry {

class GEOMETRY_DLL_EXPORT Line2D
{
public:
	static Line2D CreateFromPoints (const Point2D& linePoint1, const Point2D& linePoint2)	CAN_THROW (UnitVector_2D::UnitVectorCreationError);

	Line2D (const Point2D& linePoint, const UnitVector_2D& direction);
	virtual ~Line2D ();

	Point2D			GetAnOnLinePoint () const;
	UnitVector_2D	GetDirection () const;

	bool			IsPointOf (const Point2D& point) const;
	Point2D			ProjectToLine (const Point2D& point) const;
	double			GetDistance (const Point2D& point) const;

	bool			IsParallelWith (const Line2D& other) const;
	bool			IsColinearWith (const Line2D& other) const;					// directions may be opposite
	bool			IsEqualWith (const Line2D& other) const;					// colinear and directions point to same direction

	static bool		AreParallel (const Line2D& line1, const Line2D& line2);
	static bool		AreColinear (const Line2D& line1, const Line2D& line2);		// directions may be opposite
	static bool		AreEqual (const Line2D& line1, const Line2D& line2);		// colinear and directions point to same direction

																						//#KiP_Note ide meg kellhetenek muveletek

private:
	Line2D () = default;

	Point2D m_linePoint;
	UnitVector_2D m_direction;
};

}
#endif
