#ifndef _OPENING_ALIGNMENT_PLANE_HPP_
#define _OPENING_ALIGNMENT_PLANE_HPP_
#pragma once

// === Includes ========================================================================================================

#include "Plane3D.hpp"
#include "OpeningAlignmentSurfaceBase.hpp"

// =====================================================================================================================

namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT_FOR_TEST AlignmentPlane : public AlignmentSurfaceBase
{
public:
	AlignmentPlane (const Geometry::Plane3D& plane);
	virtual ~AlignmentPlane ();

	virtual void Transform (const TRANMAT& trafo) override;

private:
	using AlignmentSurfaceBase::FramePosition;
	
	virtual FramePosition CalculateAlignedFramePosition (const FramePosition& originalPosition) const override;

	Geometry::Plane3D plane;
};

}
}

#endif
