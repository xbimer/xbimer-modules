#ifndef _VBEO_OPENING_EXTRUSION_BASEPOLYGON_HPP_
#define _VBEO_OPENING_EXTRUSION_BASEPOLYGON_HPP_
#pragma once


#include "TypeSafeUnion.hpp"
#include "Polygon2DClassDefinition.hpp"
#include "VBElemOperationsExport.hpp"

namespace OpeningGeometry {	class ExtrusionParameters; }


namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT ExtrusionBasePolygon
{
public:
	using VertexGeometry = Point2D;
	using EdgeGeometry = GSUtils::Variant<Sector, GenArc>;


	class VB_ELEM_OPERATIONS_DLL_EXPORT RectangleVertexId
	{
	public:
		static const RectangleVertexId BottomLeft;
		static const RectangleVertexId BottomRight;
		static const RectangleVertexId TopRight;
		static const RectangleVertexId TopLeft;

		enum class VerticalSelector { Bottom, Top };
		enum class HorizontalSelector { Left, Right };

		RectangleVertexId (VerticalSelector vertical, HorizontalSelector horizontal);

		bool IsTop () const;
		bool IsBottom () const;
		bool IsLeft () const;
		bool IsRight () const;

		RectangleVertexId GetHorizontallyOpposite () const;
		RectangleVertexId GetVerticallyOpposite () const;
		RectangleVertexId GetDiagonallyOpposite () const;

		Geometry::PolyId GetPolyId () const;

		operator GS::HashValue () const; 
		bool operator== (const RectangleVertexId& other) const;
		bool operator!= (const RectangleVertexId& other) const;

		GS::UniString GetDebugInfo () const;

	private:
		VerticalSelector vertical;
		HorizontalSelector horizontal;

		static VerticalSelector GetOpposite (VerticalSelector selector);
		static HorizontalSelector GetOpposite (HorizontalSelector selector);
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT RectangleEdgeId
	{
	public:
		static const RectangleEdgeId Bottom;
		static const RectangleEdgeId Right;
		static const RectangleEdgeId Top;
		static const RectangleEdgeId Left;

		bool IsHorizontal () const;
		bool IsVertical () const;

		RectangleEdgeId GetOpposite () const;

		Geometry::PolyId GetPolyId () const;

		operator GS::HashValue () const;
		bool operator== (const RectangleEdgeId& other) const;
		bool operator!= (const RectangleEdgeId& other) const;

	private:
		RectangleEdgeId (UIndex index);

		UIndex index;
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT CircularVertexId
	{
	public:
		static const CircularVertexId Left;
		static const CircularVertexId Bottom;
		static const CircularVertexId Right;
		static const CircularVertexId Top;

		CircularVertexId GetOpposite () const;

		Geometry::PolyId GetPolyId () const;

		operator GS::HashValue () const;
		bool operator== (const CircularVertexId& other) const;
		bool operator!= (const CircularVertexId& other) const;

		GS::UniString GetDebugInfo () const;

	private:
		CircularVertexId (UIndex index);

		UIndex index;
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT CircularEdgeId
	{
	public:
		static const CircularEdgeId BottomLeftQuadrant;
		static const CircularEdgeId BottomRightQuadrant;
		static const CircularEdgeId TopRightQuadrant;
		static const CircularEdgeId TopLeftQuadrant;

		Geometry::PolyId GetPolyId () const;

		operator GS::HashValue () const;
		bool operator== (const CircularEdgeId& other) const;
		bool operator!= (const CircularEdgeId& other) const;

	private:
		CircularEdgeId (UIndex index);

		UIndex index;
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT CustomPolygonVertexId
	{
	public:
		CustomPolygonVertexId (UIndex vertexIndex);

		operator GS::HashValue () const;
		bool operator== (const CustomPolygonVertexId& other) const;
		bool operator!= (const CustomPolygonVertexId& other) const;
		
		UIndex GetIndex () const;
		GS::Optional<Geometry::Polygon2D::ConstVertexIterator> GetVertexFrom (const Geometry::Polygon2D& polygon) const;

		GS::UniString GetDebugInfo () const;

	private:
		UIndex index;
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT CustomPolygonEdgeId
	{
	public:
		CustomPolygonEdgeId (UIndex edgeIndex);

		operator GS::HashValue () const;
		bool operator== (const CustomPolygonEdgeId& other) const;
		bool operator!= (const CustomPolygonEdgeId& other) const;

		UIndex GetIndex () const; 
		GS::Optional<Geometry::Polygon2D::ConstEdgeIterator> GetEdgeFrom (const Geometry::Polygon2D& polygon) const;

		GS::UniString GetDebugInfo () const;

	private:
		UIndex index;
	};


	class VB_ELEM_OPERATIONS_DLL_EXPORT VertexId
	{
	public:
		VertexId (const RectangleVertexId& rectangleVertexId);
		VertexId (const CircularVertexId& circularVertexId);
		VertexId (const CustomPolygonVertexId& customPolygonVertexId);

		GS::Optional<RectangleVertexId> GetRectangle () const;
		GS::Optional<CircularVertexId> GetCircular () const;
		GS::Optional<CustomPolygonVertexId> GetCustomPolygonVertexId () const;

		operator GS::HashValue () const;
		bool operator== (const VertexId& other) const;
		bool operator!= (const VertexId& other) const;

		GS::UniString GetDebugInfo () const;

	private:
		GS::Optional<RectangleVertexId> rectangleVertexId;
		GS::Optional<CircularVertexId> circularVertexId;
		GS::Optional<CustomPolygonVertexId> customPolygonVertexId;
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT EdgeId
	{
	public:
		EdgeId (const RectangleEdgeId& rectangleEdgeId);
		EdgeId (const CircularEdgeId& circularEdgeId);
		EdgeId (const CustomPolygonEdgeId& customPolygonEdgeId);

		GS::Optional<RectangleEdgeId> GetRectangle () const;
		GS::Optional<CircularEdgeId> GetCircular () const;
		GS::Optional<CustomPolygonEdgeId> GetCustomPolygonEdgeId () const;

		operator GS::HashValue () const;
		bool operator== (const EdgeId& other) const;
		bool operator!= (const EdgeId& other) const;

	private:
		GS::Optional<RectangleEdgeId> rectangleEdgeId;
		GS::Optional<CircularEdgeId> circularEdgeId;
		GS::Optional<CustomPolygonEdgeId> customPolygonEdgeId;
	};


	static Owner<ExtrusionBasePolygon> Create (
		const OpeningGeometry::ExtrusionParameters& extrusionParameters, 
		const GS::Optional<Geometry::Polygon2D>& customPolygon
	);

	virtual ~ExtrusionBasePolygon ();

	virtual Geometry::Polygon2D				GetPolygon () const = 0;
	virtual	GS::HashSet<VertexId>			GetAllVertexIds () const = 0;
	virtual GS::HashSet<EdgeId>				GetAllEdgeIds () const = 0;
	virtual	GS::Optional<VertexGeometry>	GetVertex (const VertexId& vertexId) const = 0;
	virtual GS::Optional<EdgeGeometry>		GetEdge (const EdgeId& edgeId) const = 0;

	virtual	GS::Optional<VertexId>			GetBeginVertexOf (const EdgeId& edge) const = 0;
	virtual	GS::Optional<VertexId>			GetEndVertexOf (const EdgeId& edge) const = 0;
	virtual	GS::Optional<EdgeId>			GetNextEdge (const VertexId& vertex) const = 0;
	virtual	GS::Optional<EdgeId>			GetPreviousEdge (const VertexId& vertex) const = 0;
};

}
}

#endif
