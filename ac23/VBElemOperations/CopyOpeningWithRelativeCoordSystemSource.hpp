#ifndef COPY_OPENING_WITH_RELATIVE_COORD_SYSTEM_SOURCE_HPP
#define COPY_OPENING_WITH_RELATIVE_COORD_SYSTEM_SOURCE_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "OpeningTypes.hpp"


namespace VBEO {
namespace Opening {

VB_ELEM_OPERATIONS_DLL_EXPORT
VBElem::OpeningRef CopyOpeningWithConnectionsAndRelativeCoordSystemSource (const VBElem::OpeningRef& opening);

}
}

#endif 