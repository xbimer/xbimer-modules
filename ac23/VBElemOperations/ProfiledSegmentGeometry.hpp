// Contact person : KiP
#ifndef PROFILED_SEGMENT_GEOMETRY_HPP
#define PROFILED_SEGMENT_GEOMETRY_HPP
#pragma once

// === Includes ========================================================================================================

#include "Line2D.hpp"
#include "Line3D.hpp"
#include "Ellipse3D.hpp"
#include "ProfileGeometry3D.hpp"

// =====================================================================================================================

namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT ProfiledSegmentGeometry
{
public:
	using Corners = ProfileGeometry3D::Corners;
	using Sides = ProfileGeometry3D::Sides;

	enum class ProjectionPreference { CloserToBeg, CloserToEnd };

	ProfiledSegmentGeometry (const VBElem::AssemblySegmentConstRef& segment);
	ProfiledSegmentGeometry (const VBElem::WallConstRef& profiledWall);
	ProfiledSegmentGeometry (const ProfileGeometry3D& beg, const ProfileGeometry3D& end, const GS::Optional<Geometry::Line3D>& arcCenter);

	bool								IsCurved () const;
	GS::Optional<Geometry::Line3D>		GetArcCenter () const;
	const ProfileGeometry3D&			GetBegProfileGeometry () const;
	const ProfileGeometry3D&			GetEndProfileGeometry () const;

	GS::Optional<Point3D>				ProjectPointToPlane (const Point2D& pointOnBegProfile, const Point2D& pointOnEndProfile, const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Geometry::Line3D>		ProjectLineToPlane (const Geometry::Line2D& lineOnBegProfile, const Geometry::Line2D& lineOnEndProfile, const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Geometry::Ellipse3D>	ProjectEllipseToPlane (const Geometry::Ellipse& ellipseOnBegProfile, const Geometry::Ellipse& ellipseOnEndProfile, const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Sector3D>				ProjectEdgeToPlane (const PVI::HatchEdgeId& edgeId, const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Corners>				ProjectNominalBoxCornersToPlane (const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Sides>					ProjectNominalBoxSidesToPlane (const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Corners>				ProjectTotalBoxCornersToPlane (const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Sides>					ProjectTotalBoxSidesToPlane (const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;

	bool								ProjectedSectorIsStraight (const Sector& sectorOnBegProfile, const Sector& sectorOnEndProfile, const Geometry::Plane3D& plane) const;

private:
	bool								ProjectedSectorIsStraight (const Sector3D& edgeOnBeg3D, const Sector3D& edgeOnEnd3D, const Geometry::Plane3D& plane) const;
	GS::Optional<Point3D>				ProjectToPlane (const Point3D& pointOnBeg3D, const Point3D& pointOnEnd3D, const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Sector3D>				ProjectToPlane (const Sector3D& sectorOnBeg3D, const Sector3D& sectorOnEnd3D, const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Corners>				ProjectToPlane (const Corners& cornersOnBeg3D, const Corners& cornersOnEnd3D, const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;
	GS::Optional<Sides>					ProjectToPlane (const Sides& sidesOnBeg3D, const Sides& sidesOnEnd3D, const Geometry::Plane3D& plane, ProjectionPreference projectionPreference) const;

	GS::Optional<Geometry::Line3D> arcCenter;
	GS::Ref<ProfileGeometry3D> beg;
	GS::Ref<ProfileGeometry3D> end;
};

}

#endif
