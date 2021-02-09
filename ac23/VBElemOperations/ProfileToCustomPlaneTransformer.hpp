// Contact person : KiP
#ifndef PROFILE_TO_3D_TRANSFORMER_HPP
#define PROFILE_TO_3D_TRANSFORMER_HPP
#pragma once

// === Includes ========================================================================================================

// from Geometry
#include "Matrix23.hpp"
#include "Plane.hpp"
#include "Plane3D.hpp"
#include "CoordinateSystem3D.hpp"
#include "GenArc2DData.h"

// from VBElements
#include "AssemblySegmentTypes.hpp"
#include "BeamTypes.hpp"
#include "BeamSegmentTypes.hpp"
#include "ColumnTypes.hpp"
#include "ColumnSegmentTypes.hpp"
#include "WallTypes.hpp"
#include "ProfileTypes.hpp"

// from VBElemOperations
#include "VBElemOperationsExport.hpp"

// =====================================================================================================================

namespace VBEO {

enum ProfileTrafoInclusion	//#KiP_Refactor_Profile ennek ProfileTo3DTransformer-en belul kell lennie
{
	IncludeProfileTrafo,
	OmitProfileTrafo
};

class VB_ELEM_OPERATIONS_DLL_EXPORT ProfileTo3DTransformer
{
public:
	static ProfileTo3DTransformer	CreateForSegmentBeginPlane (const VBElem::AssemblySegmentConstRef& segment, ProfileTrafoInclusion profileTrafoInclusion);
	static ProfileTo3DTransformer	CreateForSegmentEndPlane (const VBElem::AssemblySegmentConstRef& segment, ProfileTrafoInclusion profileTrafoInclusion);
	static ProfileTo3DTransformer	CreateForWallBeginPlane (const VBElem::WallConstRef& wall, ProfileTrafoInclusion profileTrafoInclusion);
	static ProfileTo3DTransformer	CreateForWallEndPlane (const VBElem::WallConstRef& wall, ProfileTrafoInclusion profileTrafoInclusion);

	Point3D					To3D (const Point2D& pointInProfile) const;
	Sector3D				To3D (const Sector& sectorInProfile) const;
	Vector3D				To3D (const Vector& vectorInProfile) const;

	GS::Optional<Point2D>	ToProfile (const Point3D& pointIn3D) const;
	GS::Optional<Sector>	ToProfile (const Sector3D& sectorIn3D) const;
	GS::Optional<Vector>	ToProfile_V (const Vector3D& vectorIn3D) const;

	bool					IsOnProfilePlane (const Point3D& point) const;
	Geometry::Plane3D		GetProfilePlane () const;

	ProfileTo3DTransformer	CopyAndOffset (const Vector3D& offset) const;

private:
	Geometry::Matrix23 profileTrafo;
	Geometry::Matrix23 invProfileTrafo;
	Geometry::OrthonormalizedCoordinateSystem3D localCoordSys;

	enum class RefLinePoint { Beg, End };
	enum class XAxisDirection { TowardsOrigo, FromOrigo };

	ProfileTo3DTransformer ();

	static ProfileTo3DTransformer CreateForSegment (const VBElem::AssemblySegmentConstRef& segment, ProfileTrafoInclusion profileTrafoInclusion, RefLinePoint refLinePoint);
	static ProfileTo3DTransformer CreateForWall (const VBElem::WallConstRef& wall, ProfileTrafoInclusion profileTrafoInclusion, RefLinePoint refLinePoint);

	void SetupProfileTrafo (const Geometry::Matrix23& trafo);
	void SetupIdentityProfileTrafo ();

	void SetupCoordinateSystemFromBeamSegment (const VBElem::AssemblySegmentConstRef& segment, const VBElem::BeamConstRef& ownerBeam, RefLinePoint refLinePoint);
	void SetupCoordinateSystemFromColumnSegment (const VBElem::AssemblySegmentConstRef& segment, const VBElem::ColumnConstRef& ownerColumn, RefLinePoint refLinePoint);
	void SetupCoordinateSystemFromWall (const VBElem::WallConstRef& wall, RefLinePoint refLinePoint);
	void SetupCoordinateSystemFromHorizontallyCurvedRefLine (const Point3D& beg, const Point3D& end, double arcAngle, RefLinePoint refLinePoint, XAxisDirection xAxisDirection);
};


class VB_ELEM_OPERATIONS_DLL_EXPORT ProfileToCustomPlaneTransformer
{
public:
	static ProfileToCustomPlaneTransformer	CreateForBeamSegment (const VBElem::BeamSegmentConstRef& segment,
																  const double refLinePointParam /*[0-BegC, 1-EndC]*/,
																  ProfileTrafoInclusion profileTrafoInclusion,
																  const GS::Optional<Vector3D>& customPlaneNormal = GS::NoValue /*useable only for straight beams*/);

	static ProfileToCustomPlaneTransformer	CreateForColumnSegment (const VBElem::ColumnSegmentConstRef& segment,
																	double refLinePointParam /*[0, 1]*/,
																	ProfileTrafoInclusion profileTrafoInclusion,
																	const GS::Optional<Vector3D>& customPlaneNormal = GS::NoValue /*useable only for straight segments*/);

	static ProfileToCustomPlaneTransformer	CreateForBeam (const VBElem::BeamConstRef& beam, 
														   double refLinePointParam /*[0-BegC, 1-EndC]*/,
														   ProfileTrafoInclusion profileTrafoInclusion,
														   const GS::Optional<Vector3D>& customPlaneNormal = GS::NoValue /*useable only for straight beams*/);
	
	static ProfileToCustomPlaneTransformer	CreateForColumn (const VBElem::ColumnConstRef& column, 
															 double refLinePointParam /*[0-BegC, 1-EndC]*/, 
															 ProfileTrafoInclusion profileTrafoInclusion, 
															 const GS::Optional<Vector3D>& customPlaneNormal = GS::NoValue /*useable only for straight columns*/);

	static ProfileToCustomPlaneTransformer	CreateForWall (const VBElem::WallConstRef& wall, 
														   double refLinePointParam /*[0-BegC, 1-EndC]*/, 
														   ProfileTrafoInclusion profileTrafoInclusion,
														   const GS::Optional<Vector3D>& customPlaneNormal = GS::NoValue /*useable only for straight walls*/);

	static ProfileToCustomPlaneTransformer	CreateForBeamSegmentBeginPlane		(const VBElem::BeamSegmentConstRef& segment, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForBeamSegmentEndPlane		(const VBElem::BeamSegmentConstRef& segment, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForColumnSegmentTopPlane		(const VBElem::ColumnSegmentConstRef& segment, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForColumnSegmentBottomPlane	(const VBElem::ColumnSegmentConstRef& segment, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForBeamBeginPlane				(const VBElem::BeamConstRef& beam, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForBeamEndPlane				(const VBElem::BeamConstRef& beam, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForColumnTopPlane				(const VBElem::ColumnConstRef& column, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForColumnBottomPlane			(const VBElem::ColumnConstRef& column, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForWallBeginPlane				(const VBElem::WallConstRef& wall, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);
	static ProfileToCustomPlaneTransformer	CreateForWallEndPlane				(const VBElem::WallConstRef& wall, ProfileTrafoInclusion profileTrafoInclusion = IncludeProfileTrafo);

	Point3D		ProfileToCustomPlane (const Point2D& coordInProfile) const;
	Sector3D	ProfileToCustomPlane (const Sector& sectorInProfile) const;
	Vector3D	ProfileToCustomPlane (const Vector& vectorInProfile) const;

	Point2D		CustomPlaneToProfile (const Point3D& coordOnCustomPlane) const;
	Sector		CustomPlaneToProfile (const Sector3D& sectorOnCustomPlane) const;
	Vector		CustomPlaneToProfile_V (const Vector3D& vectorOnCustomPlane) const;

	Point3D		GetCustomPlaneBasePoint () const;
	Vector3D	GetCustomPlaneNormal () const;

private:
	Geometry::Matrix23 profileTrafo;
	Geometry::Matrix23 invProfileTrafo;
	Geometry::Plane verticalPlane;
	Geometry::Plane customPlane;

	ProfileToCustomPlaneTransformer ();

	void	SetupPlanesForStraightExtrusion 		(const Point3D& refLinePoint, const Vector3D& extrusionVect, const Vector3D& verticalPlaneXAxis, const Vector3D& customPlaneNormal);
	void	SetupPlanesForCurvedExtrusion			(const Point2D& arcBegC, const Point2D& arcEndC, double arcAngle, double zLevel, double refLinePointParam);
	void	SetupPlanesForVerticallyCurvedExtrusion	(const Geometry::GenArc& arc, double slantAngle, double baseElev, double refLinePointParam);
	void	SetupProfileTrafo						(const Geometry::Matrix23& trafo);
	void	SetupIdentityProfileTrafo				();
};

}

#endif
