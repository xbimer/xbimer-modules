#ifndef _OPENING_ALIGNMENT_SURFACE_BASE_HPP_
#define _OPENING_ALIGNMENT_SURFACE_BASE_HPP_
#pragma once

// === Includes ========================================================================================================

#include "OpeningAlignmentSurface.hpp"
#include "Vector3D.hpp"
#include "UnitVector3D.hpp"
#include "VBElemOperationsExport.hpp"

// === Predeclarations =================================================================================================

namespace VBEO {
namespace Opening {
	class FramePosition;
}
}

// =====================================================================================================================

namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT_FOR_TEST AlignmentSurfaceBase : public AlignmentSurface
{
public:
	AlignmentSurfaceBase ();
	virtual ~AlignmentSurfaceBase ();

	virtual OpeningGeometry::ExtrusionFrame Align (const OpeningGeometry::ExtrusionFrame& originalFrame) const override final;

	virtual UnitVector_3D GetAlignedExtrusionDirection (const Point3D& extrusionLinePoint) const override final;

	virtual void Transform (const TRANMAT& trafo) override = 0;

protected:
	class FramePosition
	{
	public:
		Point3D origo;
		UnitVector_3D extrusionDir;

		FramePosition (const Point3D& origo, const UnitVector_3D& extrusionDir) : origo (origo), extrusionDir (extrusionDir) {}
	};

	static void FlipAlignedExtrusionDirIfNecessary (UnitVector_3D& alignedExtrusionDir, const UnitVector_3D& originalExtrusionDir);

	static UnitVector_3D CreatePerpendicularVector (const UnitVector_3D& u);
	static UnitVector_3D GetPerpendicularComponent (const UnitVector_3D& u, const UnitVector_3D& parallelComponentDir);

private:
	virtual FramePosition CalculateAlignedFramePosition (const FramePosition& originalPosition) const = 0;
};

}
}

#endif
