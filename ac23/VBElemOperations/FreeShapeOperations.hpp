#ifndef FREESHAPE_OPERATIONS_HPP
#define FREESHAPE_OPERATIONS_HPP

// from VBElements
#include "FreeShapeTypes.hpp"

// from VBElemOperations
#include "VBElemOperationsExport.hpp"
#include "VBElemOperationsTypes.hpp"
#include "VBElemSurfaceDescriptor.hpp"

// from GSUtils
#include "GSUtilsDefs.h"

// from VBAttributes
#include "ADBAttributeIndex.hpp"


class VB_ELEM_OPERATIONS_DLL_EXPORT FreeShapeOperations
{
public:
	enum MaterialSelector {
		SideMaterial,
		CutElemMaterial
	};

	static VBElemSurfaceDescriptor GetMaterial (const VBElem::FreeShapeConstRef& freeShape,
												const ADB::AttributeSetConstRef& attributeSet,
												MaterialSelector selector,
												bool pbcConversion);

	static void		GetStandaloneVisibleMaterials (VBElem::FreeShapeConstRef freeShape, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& materials);

	static GS::Optional<VBEO::ModelElemStructureRef>			GetModelElemStructure			(const VBElem::FreeShapeConstRef& freeShape);
	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const VBElem::FreeShapeConstRef& freeShape);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(const VBElem::FreeShapeRef& freeShape);

	static GS::Optional<VBEO::ModelElemStructureRef>			GetModelElemStructure			(const VBElem::FreeShapeDefault& freeShapeDef);
	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const VBElem::FreeShapeDefault& freeShapeDef);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(VBElem::FreeShapeDefault& freeShapeDef);
};

#endif // FREESHAPE_OPERATIONS_HPP
