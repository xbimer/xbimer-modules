#if ! defined DIRECTED_PART_OF_NURBS_CURVE_HPP
#define DIRECTED_PART_OF_NURBS_CURVE_HPP


#include "NurbsCurve3D.hpp"


namespace Geometry {


struct GEOMETRY_DLL_EXPORT DirectedPartOfNurbsCurve3D {
	NurbsCurve3D curve;
	double lowerParamEnd;
	double higherParamEnd;
	bool reversed;

	double	GetBeginParam () const;
	double	GetEndParam () const;
	double	GetParamAtNormalized (double normalizedParam) const;
	Coord3D	GetDiredBeginPos () const;
	Coord3D	GetDiredEndPos () const;
	Coord3D	GetPosAtNormalized (double normalizedParam) const;
};


}	// namespace Geometry

#endif
