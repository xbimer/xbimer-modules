#ifndef VBELEM_SPLIT_HPP
#define VBELEM_SPLIT_HPP
#pragma once

#include "VBElementsBase/ModelElementTypes.hpp"
#include "VBElemOperationsExport.hpp"

namespace Geometry {
class Plane3D;
}


namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT VBElemSplit {
public:
	VBElemSplit (const VBElem::ModelElementRef& modelElem);

	bool IsSplitEnabled () const;
	bool Is3DSplitPlaneEnabled () const;
	void Split (const Geometry::Plane3D& cutPlane, const Coord3D& refCoord);
	void Split (const Sector& sector, const Coord& refCoord);
private:
	VBElem::ModelElementRef modelElem;
};

}


#endif