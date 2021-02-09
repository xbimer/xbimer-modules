#ifndef OPENING_CUSTOM_POLYGON_GUARD_HPP
#define OPENING_CUSTOM_POLYGON_GUARD_HPP
#pragma once

// === Includes ========================================================================================================

#include "OpeningTypes.hpp"
#include "Polygon2D.hpp"
#include "VBElemOperationsExport.hpp"
#include "OpeningExtrusionParameters.hpp"

// =====================================================================================================================

namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT CustomBasePolygonGuard 
{
private:
	VBElem::OpeningRef opening;
	OpeningGeometry::ExtrusionParameters::BasePolygonType basePolygonType;
	const Geometry::Polygon2D polygon;

public:
	CustomBasePolygonGuard (const VBElem::OpeningRef& opening);
	~CustomBasePolygonGuard ();
};

}
}

#endif
