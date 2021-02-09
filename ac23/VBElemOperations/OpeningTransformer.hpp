#ifndef _OPENING_TRANSFORMER_HPP
#define _OPENING_TRANSFORMER_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "OpeningTypes.hpp"

struct TRANMAT;

namespace VBEO {
namespace Opening {

class ExtrudedGeometry;

enum TransformMode { TransformTogetherWithLocalCoordSysSource, TransformWithoutLocalCoordSysSource };


VB_ELEM_OPERATIONS_DLL_EXPORT
void TransformGeometry (ExtrudedGeometry& geometry, const TRANMAT& orthogonalTransformation, TransformMode transformMode);

VB_ELEM_OPERATIONS_DLL_EXPORT
void ResizeGeometry (ExtrudedGeometry& geometry, double scale, const Point3D& resizeOrigo, TransformMode transformMode);

}
}

#endif

