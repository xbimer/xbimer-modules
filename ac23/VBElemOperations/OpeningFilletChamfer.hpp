#ifndef OPENING_FILLET_CHAMFER_HPP
#define OPENING_FILLET_CHAMFER_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "OpeningTypes.hpp"

namespace VBEO {
namespace Opening {

class ExtrudedGeometry;

void SetModeForFilletChamfer (const VBElem::OpeningRef& opening);
bool ValidateFilletChamfer (const VBElem::OpeningRef& opening);

VB_ELEM_OPERATIONS_DLL_EXPORT
void FilletGeometry (ExtrudedGeometry& geometry, double radius);
VB_ELEM_OPERATIONS_DLL_EXPORT
void ChamferGeometry (ExtrudedGeometry& geometry, double radius);

}
}

#endif
