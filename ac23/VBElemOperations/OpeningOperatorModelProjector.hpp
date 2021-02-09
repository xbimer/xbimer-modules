#ifndef OPENING_OPERATOR_MODEL_PROJECTOR_HPP
#define OPENING_OPERATOR_MODEL_PROJECTOR_HPP
#pragma once

#include "Sector2DData.h"
#include "Polygon2D.hpp"
#include "VBElemOperationsExport.hpp"
#include "ODBReference.hpp"
#include "ExtendedPen.hpp"
#include "GSUtilsDefs.h"


namespace Modeler {
	class Camera;
}

namespace ADB {
	class AttributeSet;
	typedef ODB::ConstRef<AttributeSet> AttributeSetConstRef;	//#KiP_A180_NiceToHave kellene ezeknek a typedef-eknek egy kulon header
}

namespace VBEO {
namespace Opening {
	class OperatorModel;
}
}


namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT ProjectedOperatorModel
{
public:
	struct VB_ELEM_OPERATIONS_DLL_EXPORT Line
	{
		Sector geometry;
		VBAttr::ExtendedPen pen;
		GSAttributeIndex pattern;
	};

	struct VB_ELEM_OPERATIONS_DLL_EXPORT Fill
	{
		Geometry::Polygon2D geometry;
	};

	using Lines = GS::Array<Line>;
	using Fills = GS::Array<Fill>;

	static ProjectedOperatorModel Generate (const OperatorModel& operatorModel, const Modeler::Camera& camera);
	static ProjectedOperatorModel GenerateOnFloorPlan (const OperatorModel& operatorModel);

	static Modeler::Camera GetFloorPlanCamera ();

	const Lines& GetLines () const;
	const Fills& GetFills () const;

private:
	Lines lines;
	Fills fills;
};

}
}

#endif
