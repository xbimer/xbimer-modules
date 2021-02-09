#ifndef OPENING_OPERATOR_MODEL_HPP
#define OPENING_OPERATOR_MODEL_HPP
#pragma once


#include "GSFriend.hpp"
#include "Model3D/Model3D.hpp"
#include "OpeningExtrusionLimits.hpp"
#include "OpeningExtrudedGeometry.hpp"


namespace ADB {
	class AttributeSet;
	typedef ODB::ConstRef<AttributeSet> AttributeSetConstRef;	//#KiP_A180_NiceToHave kellene ezeknek a typedef-eknek egy kulon header
}

namespace VBEO {
namespace Opening {
	class ProjectedOperatorModel;
}
}

// =====================================================================================================================

namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT OperatorModel
{
public:
	static OpeningGeometry::InfiniteExtrusion GetInfiniteExtrusion (const VBElem::OpeningConstRef& opening);

	class VB_ELEM_OPERATIONS_DLL_EXPORT ExtrudedGeometry
	{
	public:
		ExtrudedGeometry (const OpeningGeometry::InfiniteExtrusion& infiniteExtrusion, const ExtrusionLimits& limits);

		Geometry::Polygon2D					BasePolygon () const;
		UnitVector_3D						Direction () const;
		OpeningGeometry::ExtrusionBasePlane	BasePlane () const;
		OpeningGeometry::ExtrusionFrame		Frame () const;
		OpeningGeometry::InfiniteExtrusion	InfiniteExtrusion () const;
		ExtrusionLimits						Limits () const;
	
	private:
		OpeningGeometry::InfiniteExtrusion infiniteExtrusion;
		ExtrusionLimits limits;
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT Attributes
	{
	public:
		Modeler::AttributesPtr modelAttributes;
		GS::Optional<GS_RGBColor> color;

		Attributes ();
		Attributes (const Modeler::AttributesPtr& modelAttributes, const GS_RGBColor& color);

		static GS_RGBColor ScreenOnlyModelColor ();
	};


	static OperatorModel Generate (
		const ExtrudedGeometry& geometry,
		const ADB::AttributeSetConstRef& attributeSet,
		const Attributes& attributes = Attributes ()
	);

	bool						IsValid () const;
	Modeler::ConstElemPtr		GetElem () const;
	Modeler::ConstMeshBodyPtr	GetTheOnlyBody () const;

	Modeler::Model3DPtr			GetModel (GS::Friend<ProjectedOperatorModel>) const;
	ADB::AttributeSetConstRef	GetAttributeSet (GS::Friend<ProjectedOperatorModel>) const;


private:
	OperatorModel () = default;

	Modeler::Model3DPtr model;
	ADB::AttributeSetConstRef attributeSet;
};

}
}

#endif
