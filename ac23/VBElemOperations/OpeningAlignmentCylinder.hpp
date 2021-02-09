#ifndef _OPENING_ALIGNMENT_CYLINDER_HPP_
#define _OPENING_ALIGNMENT_CYLINDER_HPP_
#pragma once

// === Includes ========================================================================================================

#include "InfiniteCylinder.hpp"
#include "OpeningAlignmentSurfaceBase.hpp"

// =====================================================================================================================

namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT_FOR_TEST AlignmentCylinder : public AlignmentSurfaceBase
{
public:
	AlignmentCylinder (const Geometry::InfiniteCylinder& cylinder);
	virtual ~AlignmentCylinder ();

	virtual void Transform (const TRANMAT& trafo) override;

private:
	using AlignmentSurfaceBase::FramePosition;
	
	virtual FramePosition CalculateAlignedFramePosition (const FramePosition& originalPosition) const override;

	Geometry::InfiniteCylinder cylinder;
};

}
}

#endif
