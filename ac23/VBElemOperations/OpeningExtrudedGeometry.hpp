#ifndef _OPENING_EXTRUDED_GEOMETRY_HPP_
#define _OPENING_EXTRUDED_GEOMETRY_HPP_
#pragma once


#include "EllipseArc3D.hpp"
#include "VBElementsBase/ModelElementTypes.hpp"
#include "OpeningTypes.hpp"
#include "OpeningExtrusionGeometryData.hpp"
#include "OpeningExtrusionBasePolygon.hpp"
#include "OpeningAlignmentSurface.hpp"

namespace OpeningGeometry { class InfiniteExtrusion; }


namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT VertexGeometry3D
{
public:
	VertexGeometry3D (const Point3D& point);

	operator Point3D () const;

	GS::Optional<VertexGeometry3D> ProjectToPlane (const Geometry::Plane3D& plane, const UnitVector_3D& projectionDir) const;

private:
	Point3D point;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT EdgeGeometry3D
{
private:
	template <typename Type>
	using IsValidType = GS::EnableIf<GS::IsSame<GS::Decay<Type>, Sector3D> || GS::IsSame<GS::Decay<Type>, Geometry::EllipseArc3D>>;

	GSUtils::Variant<Sector3D, Geometry::EllipseArc3D> data;

public:
	EdgeGeometry3D (const Sector3D& sector);
	EdgeGeometry3D (const Geometry::EllipseArc3D& arc);

	template <typename GeometryType, typename = IsValidType<GeometryType>>	
	bool Is () const 
	{ 
		return data.Is<GeometryType> (); 
	}

	template <typename GeometryType, typename = IsValidType<GeometryType>>	
	const GeometryType& Get () const
	{
		return data.Get<GeometryType> ();
	}

	GS::Optional<EdgeGeometry3D> ProjectToPlane (const Geometry::Plane3D& plane, const UnitVector_3D& projectionDir) const;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT ConstExtrudedGeometry
{
public:
	class VB_ELEM_OPERATIONS_DLL_EXPORT CoreDataQuery
	{
	public:
		virtual ~CoreDataQuery ();
		
		virtual OpeningGeometry::ExtrusionGeometryData	GetExtrusionGeometryData () const = 0;
		virtual	VBElem::RelativeCoordinateSystem		GetLocalCoordinateSystem () const = 0;
		virtual Owner<AlignmentSurface>					GetAlignmentSurface () const = 0;
		virtual UnitVector_2D							GetDefaultHorizontalFrameDirection (const Point3D& extrusionLinePoint) const = 0;
		virtual double									GetAnchorAltitudeReferenceLevel () const = 0;
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT CoreDataQueryByLocalCoordSysSourceElem : public CoreDataQuery
	{
	public:
		virtual ~CoreDataQueryByLocalCoordSysSourceElem ();

		virtual OpeningGeometry::ExtrusionGeometryData	GetExtrusionGeometryData () const override = 0;
		virtual	VBElem::RelativeCoordinateSystem		GetLocalCoordinateSystem () const override final;
		virtual Owner<AlignmentSurface>					GetAlignmentSurface () const override final;
		virtual UnitVector_2D							GetDefaultHorizontalFrameDirection (const Point3D& extrusionLinePoint) const override final;
		virtual double									GetAnchorAltitudeReferenceLevel () const override final;
	private:
		virtual VBElem::ModelElementConstRef			GetLocalCoordSysSource () const = 0;
	};

	static GS::Ref<CoreDataQuery> CreateCoreDataQueryFor (const VBElem::OpeningConstRef& opening);

	ConstExtrudedGeometry (const GS::Ref<CoreDataQuery>& coreDataQuery);
	ConstExtrudedGeometry (const VBElem::OpeningConstRef& opening);

	UnitVector_3D							GetExtrusionDirection () const;
	OpeningGeometry::ExtrusionBasePlane		GetBasePlane () const;
	OpeningGeometry::ExtrusionFrame			GetExtrusionFrame () const;
	OpeningGeometry::ExtrusionParameters	GetExtrusionParameters () const;
	Owner<ExtrusionBasePolygon>				GetBasePolygon () const;
	OpeningGeometry::ExtrusionAnchor		GetAnchor () const;
	OpeningGeometry::InfiniteExtrusion		GetInfiniteExtrusion () const;
	GS::Optional<double>					GetAnchorAbsoulteLevel () const;

	void	EnumerateBasePolygonVertices	(const std::function<void (const ExtrusionBasePolygon::VertexId& vertexId, const VertexGeometry3D& vertex)>& enumerator) const;
	void	EnumerateBasePolygonEdges		(const std::function<void (const ExtrusionBasePolygon::EdgeId& edgeId, const EdgeGeometry3D& edge)>& enumerator) const;

	const CoreDataQuery& GetCoreDataQuery () const;

protected:
	OpeningGeometry::ExtrusionFrame	GetGlobalExtrusionFrame () const;
	OpeningGeometry::ExtrusionFrame	TransformLocalToGlobal (const OpeningGeometry::ExtrusionFrame& localExtrusionFrame) const;
	OpeningGeometry::ExtrusionFrame	TransformGlobalToLocal (const OpeningGeometry::ExtrusionFrame& globalExtrusionFrame) const;

private:
	GS::Ref<CoreDataQuery> m_coreDataQuery;

	void EnforceParameterConstraints (OpeningGeometry::ExtrusionFrame& extrusionFrame) const;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT ExtrudedGeometry : public ConstExtrudedGeometry
{
public:
	class VB_ELEM_OPERATIONS_DLL_EXPORT CoreDataModifier
	{
	public:
		virtual ~CoreDataModifier ();

		virtual void SetExtrusionFrameInLocalCoordSys (const OpeningGeometry::ExtrusionFrame& newExtrusionFrame) = 0;
		virtual void SetExtrusionParameters (const OpeningGeometry::ExtrusionParameters& newParameters) = 0;
		virtual void SetCustomBasePolygon (const Geometry::Polygon2D& newCustomBasePolygon) = 0;
	};

	static GS::Ref<CoreDataModifier> CreateCoreDataModifierFor (const VBElem::OpeningRef& opening);

	ExtrudedGeometry (const GS::Ref<CoreDataQuery>& coreDataQuery, const GS::Ref<CoreDataModifier>& coreDataModifier);
	ExtrudedGeometry (const VBElem::OpeningRef& opening);

	void	SetExtrusionDirection (const UnitVector_3D& newExtrusionDirection);
	void	SetBasePlane (const OpeningGeometry::ExtrusionBasePlane& newBasePlane);
	void	SetExtrusionFrame (const OpeningGeometry::ExtrusionFrame& newExtrusionFrame);
	void	SetExtrusionParameters (const OpeningGeometry::ExtrusionParameters& newParameters);
	void	SetExtrusionConstraint (OpeningGeometry::ExtrusionParameters::Constraint newConstraint);
	void	SetWidthHeightLink (OpeningGeometry::ExtrusionParameters::LinkedStatus newLinkedStatus);
	void	SetAnchor (const OpeningGeometry::ExtrusionAnchor& newAnchor);
	void	SetAnchorAlititude (double anchorAbsoluteLevel);
	void	SetupCustomBasePolygon (const Geometry::Polygon2D& polygon);
	void	SetupCustomBasePolygonAndAdjustBasePlaneOrigo (const Geometry::Polygon2D& polygon);

	void    AdjustBasePlaneOrigo (const Box2DData& newBounds);

	CoreDataModifier& GetCoreDataModifier () const;

private:
	GS::Ref<CoreDataModifier> m_coreDataModifier;
};

}
}

#endif
