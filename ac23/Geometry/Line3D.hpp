#ifndef _LINE_3D_HPP_
#define _LINE_3D_HPP_
#pragma once

// === Includes ========================================================================================================

// from Geometry
#include "GeometricDefinitions.h"
#include "UnitVector3D.hpp"

// =====================================================================================================================

namespace Geometry {

class GEOMETRY_DLL_EXPORT Line3D
{
public:
	static Line3D CreateFromPoints (const Coord3D& c1, const Coord3D& c2)	CAN_THROW (UnitVectorCreationError);
	
	Line3D (const Coord3D& linePoint, const UnitVector_3D& direction);
	virtual ~Line3D ();

	Coord3D			GetAnOnLinePoint	() const;
	UnitVector_3D	GetDirection		() const;

	bool			IsPointOf			(const Coord3D& c) const;
	Coord3D			ProjectToLine		(const Coord3D& c) const;
	double			GetDistance			(const Coord3D& c) const;

	bool			IsParallelWith		(const Line3D& other) const;
	bool			IsColinearWith		(const Line3D& other) const;					// directions may be opposite
	bool			IsEqualWith			(const Line3D& other) const;					// colinear and directions point to same direction

	static bool		AreParallel			(const Line3D& line1, const Line3D& line2);
	static bool		AreColinear			(const Line3D& line1, const Line3D& line2);		// directions may be opposite
	static bool		AreEqual			(const Line3D& line1, const Line3D& line2);		// colinear and directions point to same direction


	//#KiP_Note ide meg kellhetenek muveletek

	static Coord3D ProjectToLine (const Coord3D& coord, const Vector3D& lineDir, const Coord3D& linePoint);	//#KiP_Note ez megmaradt, mert sokat hasznaltak kivulrol

private:
	Line3D () = default;

	Coord3D m_linePoint;
	UnitVector_3D m_direction;
};

GEOMETRY_DLL_EXPORT bool IsHorizontalLine (const Geometry::Line3D& line);

GEOMETRY_DLL_EXPORT bool IsVerticalLine (const Geometry::Line3D& line);

}
#endif
