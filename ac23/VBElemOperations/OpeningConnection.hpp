#ifndef _OPENING_CONNECTION_HPP
#define _OPENING_CONNECTION_HPP
#pragma once


#include "Line3D.hpp"
#include "UnitVector2DClassDeclaration.hpp"
#include "VBElementsBase/ModelElementTypes.hpp"
#include "OpeningTypes.hpp"
#include "VBElemOperationsExport.hpp"

namespace OpeningGeometry { class ExtrusionFrame; }
namespace VBEO { namespace Opening { class AlignmentSurface; }}


namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT OpeningConnection
{
public:
	static bool CanBeConnectedToOpening (const VBElem::ModelElementConstRef& modelElem);	
	
	static void Connect (const VBElem::OpeningRef& opening, const VBElem::ModelElementRef& modelElem);
	static void Disconnect (const VBElem::OpeningRef& opening, const VBElem::ModelElementRef& modelElem);
	static void	ResetRelativeCoordinateSystemSource (const VBElem::OpeningRef& opening, const VBElem::ModelElementRef& newCoordinateSystemSource);

	static GS::Array<Point3D> CalculateApproximateIntersections (const VBElem::ModelElementConstRef& modelElem, const Geometry::Line3D& line);
	
	static Owner<Opening::AlignmentSurface> CreateAlignmentSurface (const VBElem::ModelElementConstRef& modelElem);

	static UnitVector_2D GetDefaultHorizontalFrameDirection (const VBElem::ModelElementConstRef& modelElem, const Point3D& extrusionLinePoint);
};

}

#endif
