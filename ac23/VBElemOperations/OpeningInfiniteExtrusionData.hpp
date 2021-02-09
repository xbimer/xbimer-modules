#ifndef OPENING_INFINITE_EXTRUSION_DATA_HPP
#define OPENING_INFINITE_EXTRUSION_DATA_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "Polygon2D.hpp"
#include "OpeningExtrusionFrame.hpp"

namespace OpeningGeometry { class ExtrusionParameters; }


namespace OpeningGeometry {

class VB_ELEM_OPERATIONS_DLL_EXPORT InfiniteExtrusion
{
public:
	InfiniteExtrusion (const Geometry::Polygon2D& basePolygon, const ExtrusionFrame& frame);

	Geometry::Polygon2D		GetBasePolygon () const;
	UnitVector_3D			GetDirection () const;
	ExtrusionBasePlane		GetBasePlane () const;
	ExtrusionFrame			GetFrame () const;
	
private:
	Geometry::Polygon2D basePolygon;
	ExtrusionFrame frame;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT ExtrusionLimitParameters
{
public:
	ExtrusionLimitParameters (const GS::Optional<double>& startOffset, const GS::Optional<double>& finiteLength);
	ExtrusionLimitParameters (const ExtrusionParameters& extrusionParameters);

	GS::Optional<double>	GetStartOffset () const;
	GS::Optional<double>	GetFiniteLength () const;

private:
	GS::Optional<double> startOffset;
	GS::Optional<double> finiteLength;
};

}

#endif
