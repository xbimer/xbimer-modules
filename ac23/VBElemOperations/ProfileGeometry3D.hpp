// Contact person : KiP
#ifndef PROFILE_GEOMETRY_3D_HPP
#define PROFILE_GEOMETRY_3D_HPP
#pragma once

// === Includes ========================================================================================================

#include "ProfileToCustomPlaneTransformer.hpp"
#include "ParameterAppliedProfileGeometry.hpp"

// from VBElements
#include "VBElements/ParametricProfileWrapper.hpp"

// =====================================================================================================================

namespace PVI {
	class HatchEdgeId;
}

// =====================================================================================================================

namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT ProfileGeometry3D
{
public:
	struct Corners
	{
		Point3D bottomLeft;
		Point3D bottomRight;
		Point3D topRight;
		Point3D topLeft;
	};

	struct Sides
	{
		Sector3D bottom;
		Sector3D right;
		Sector3D top;
		Sector3D left;
	};

	ProfileGeometry3D (const VBElem::ConstParametricProfileWrapper& profile, const ProfileTo3DTransformer& transformer);

	Geometry::Plane3D								GetProfilePlane () const;
	VBElem::ConstParametricProfileWrapper			GetProfile2D () const;
	VBElem::ConstParameterAppliedProfileGeometryRef	GetGeometry2D () const;
	ProfileTo3DTransformer							GetTransformer () const;

	Point3D					PointTo3D (const Point2D& pointInProfile) const;
	Sector3D				EdgeTo3D (const Sector& edgeInProfile) const;
	GS::Optional<Sector3D>	EdgeTo3D (const PVI::HatchEdgeId& edgeId) const;
	Corners					NominalBoxCornersTo3D () const;
	Sides					NominalBoxSidesTo3D () const;
	Corners					TotalBoxCornersTo3D () const;
	Sides					TotalBoxSidesTo3D () const;

private:
	Corners					BoxCornersTo3D (const Box2DData& bounds) const;
	Sides					BoxSidesTo3D (const Box2DData& bounds) const;

	VBElem::ConstParametricProfileWrapper profile;
	VBElem::ConstParameterAppliedProfileGeometryRef	geometry;
	ProfileTo3DTransformer transformer;
};

}

#endif
