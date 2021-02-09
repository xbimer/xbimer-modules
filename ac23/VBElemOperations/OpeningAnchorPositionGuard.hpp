#ifndef OPENING_ANCHOR_POSITION_GUARD_HPP
#define OPENING_ANCHOR_POSITION_GUARD_HPP
#pragma once

// === Includes ========================================================================================================

#include "OpeningTypes.hpp"
#include "OpeningExtrusionAnchor.hpp"
#include "VBElemOperationsExport.hpp"

//from Geometry
#include "Box2DData.h"

// =====================================================================================================================

namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT AnchorPositionGuard
{
public:
	AnchorPositionGuard (const VBElem::OpeningRef& opening);
	~AnchorPositionGuard ();

private:
	VBElem::OpeningRef opening;
	Box2DData initialBounds;
};

}
}

#endif
