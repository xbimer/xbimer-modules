#ifndef _OPENING_ALIGNMENT_SURFACE_HPP_
#define _OPENING_ALIGNMENT_SURFACE_HPP_
#pragma once


#include "VBElemOperationsExport.hpp"
#include "UnitVector3DClassDeclaration.hpp"

struct TRANMAT;
namespace OpeningGeometry { class ExtrusionFrame; }


namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT AlignmentSurface
{
public:
	virtual ~AlignmentSurface ();

	virtual OpeningGeometry::ExtrusionFrame Align (const OpeningGeometry::ExtrusionFrame& originalFrame) const = 0;

	virtual UnitVector_3D GetAlignedExtrusionDirection (const Point3D& extrusionLinePoint) const = 0;

	virtual void Transform (const TRANMAT& trafo) = 0;	//#KiP_A180_GeometryRefactor ide is OrthogonalTransformation kellene!
};

}
}

#endif
