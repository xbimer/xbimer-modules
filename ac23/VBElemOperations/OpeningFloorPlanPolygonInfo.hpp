#ifndef OPENING_FLOORPLAN_POLYGON_INFO_HPP
#define OPENING_FLOORPLAN_POLYGON_INFO_HPP
#pragma once

// === Includes ========================================================================================================

#include "VBElemOperationsExport.hpp"
#include "Polygon2D.hpp"
#include "OpeningExtrusionBasePolygon.hpp"
#include "OpeningSymbolTypes.hpp"
#include "HashSet.hpp"

// =====================================================================================================================

namespace VBEO {
namespace Opening {


template <typename KeyType, typename ValueType>
class FloorPlanPolygonGeometryDataCollection
{
public:
	GS::Array<ValueType> Get (const KeyType& key) const
	{
		if (data.ContainsKey (key)) {
			return data.Get (key);
		} else {
			return {};
		}
	}
	
	void Enumerate (const std::function<void (const KeyType& key, const GS::Array<ValueType>& values)>& enumerator) const 
	{
		data.Enumerate (enumerator);
	}

	void Add (const KeyType& key, const ValueType& value)
	{
		if (!data.ContainsKey (key)) {
			data.Add (key, {});
		}
		data[key].Push (value);
	}
	
	void Append (const FloorPlanPolygonGeometryDataCollection& toAppend)
	{
		for (auto& item : toAppend.data) {
			const KeyType& key = *item.key;
			if (!data.ContainsKey (key)) {
				data.Add (key, {});
			}
			data[key].Append (*item.value);
		}
	}

private:
	GS::HashTable<KeyType, GS::Array<ValueType>> data;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT FloorPlanPolygonInfo
{
public:
	enum class SideSelector { Left, Right };

	using VertexCollection = FloorPlanPolygonGeometryDataCollection<ExtrusionBasePolygon::VertexId, Point2D>;
	using EdgeCollection = FloorPlanPolygonGeometryDataCollection<SideSelector, Sector>;

	enum NotVisibleOpeningSelector { OpeningIsNotVisible };
	enum OutsideOfElementsOpeningSelector { OpeningIsOutsideOfElements };

	FloorPlanPolygonInfo (const VertexCollection& vertices, const EdgeCollection& edges, const Geometry::MultiPolygon2D& intersections, const GS::HashSet<VBElem::OpeningSymbolType>& symbolTypes);
	FloorPlanPolygonInfo (NotVisibleOpeningSelector);
	FloorPlanPolygonInfo (OutsideOfElementsOpeningSelector);
	
	bool IsOpeningVisible () const;
	bool IsOpeningOutsideOfElements () const;

	GS::Array<Point2D> GetBasePolygonVertexDerivatives (const ExtrusionBasePolygon::VertexId& vertexId) const;
	GS::Array<Sector> GetExtrusionSideDerivatives (SideSelector sideSelector) const;
	Geometry::MultiPolygon2D GetIntersections () const;
	GS::HashSet<VBElem::OpeningSymbolType> GetCutTypes () const;

	GS::Optional<Sector> GetAxisLine () const;
	void SetAxisLine (const Sector& axisLine);

private:
	bool isVisible;
	bool isOutsideOfElements;
	VertexCollection vertices;
	EdgeCollection edges;
	GS::Optional<Sector> axisLine;
	Geometry::MultiPolygon2D intersections;
	GS::HashSet<VBElem::OpeningSymbolType> symbolTypes;
};

}
}

#endif
