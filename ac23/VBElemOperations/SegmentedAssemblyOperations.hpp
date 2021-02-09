#pragma once
#ifndef SEGMENTED_ASSEMBLY_OPERATIONS_HPP
#define SEGMENTED_ASSEMBLY_OPERATIONS_HPP

#include "VBElemOperationsExport.hpp"
#include "AssemblySegmentTypes.hpp"
#include "AssemblySegmentShapeTypes.hpp"
#include "SegmentedAssemblyTypes.hpp"
#include "Vector3D.hpp"
#include "TypeSafeUnion.hpp"
#include "EllipseArc3D.hpp"
#include "Sector3DData.h"

namespace SegmentedAssemblyOperations {

// --- Predeclarations

VB_ELEM_OPERATIONS_DLL_EXPORT double GlobalCoordToSegmentParam (const VBElem::AssemblySegmentRef& segmentOnCoord, const Coord3D& coord);

VB_ELEM_OPERATIONS_DLL_EXPORT GS::Optional<VBElem::AssemblySegmentConstRef> FindClosestSegmentWithPointOnCutPlane (const VBElem::SegmentedAssemblyConstRef& segmentedAssembly, const Coord3D& coord);

VB_ELEM_OPERATIONS_DLL_EXPORT VBElem::AssemblySegmentRef FindClosestSegmentToCoord (const VBElem::SegmentedAssemblyRef& segmentedAssembly, const Coord3D& coord);
VB_ELEM_OPERATIONS_DLL_EXPORT GS::Pair<double, double> GetMaxAndMinTotalWidth (const VBElem::SegmentedAssemblyConstRef& segmentedAssembly);
VB_ELEM_OPERATIONS_DLL_EXPORT GS::Pair<double, double> GetMaxAndMinTotalHeight (const VBElem::SegmentedAssemblyConstRef& segmentedAssembly);
VB_ELEM_OPERATIONS_DLL_EXPORT GSUtils::Variant<Sector3D, Geometry::EllipseArc3D> GetReferenceLineGeometry (const VBElem::SegmentedAssemblyConstRef& segmentedAssembly);
// --- End Predeclarations

}

#endif // SEGMENTED_ASSEMBLY_OPERATIONS_HPP