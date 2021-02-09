#ifndef VBELEM_SPLIT_EXTENSION_HPP
#define	VBELEM_SPLIT_EXTENSION_HPP
#pragma once

#include "Extension.hpp"
#include "VBElemSplit.hpp"
#include "Poly2DTypes.hpp"

namespace Geometry {
class Plane3D;
}

namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT VBElemSplitExtension : public GS::Extension {
public:
	static GS::ExtensionId id;
	VBElemSplitExtension (GS::ClassInfo* target);
	virtual ~VBElemSplitExtension ();

	virtual bool IsSplitEnabled (const VBElem::ModelElementRef& modelElem) const;
	virtual bool Is3DSplitPlaneEnabled (const VBElem::ModelElementRef& modelElem) const;
	virtual void Split (const VBElem::ModelElementRef& modelElem, const Geometry::Plane3D& cutPlane, const Coord3D& refCoord) const;
	virtual void Split (const VBElem::ModelElementRef& modelElem, const Sector& sector, const Coord& refCoord) const;
protected:
	static Geometry::MultiPolygon2D SplitPolygon (const Geometry::Polygon2D& polygon, const HalfPlane2DData& cutline);
};

}

#endif


