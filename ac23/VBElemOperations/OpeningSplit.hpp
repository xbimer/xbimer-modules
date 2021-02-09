#ifndef OPENING_SPLIT_HPP
#define OPENING_SPLIT_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "OpeningTypes.hpp"
#include "Poly2DTypes.hpp"

namespace Geometry {
class Plane3D;
}

namespace VBEO {
namespace Opening {

VB_ELEM_OPERATIONS_DLL_EXPORT
void SplitOpening (VBElem::OpeningRef& opening, const Geometry::Plane3D& cutPlane, const Coord3D& refCoord);
VB_ELEM_OPERATIONS_DLL_EXPORT
void SplitOpening (VBElem::OpeningRef& opening, const Sector& sector, const Coord& refCoord);

}
}

#endif
