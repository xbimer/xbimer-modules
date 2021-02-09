#ifndef CEIL_OPERATIONS_HPP
#define CEIL_OPERATIONS_HPP

#include "VBElemOperationsExport.hpp"

// from VBAttributes
#include "BuildingMaterial.hpp"
#include "compwall.hpp"

// from VBElements
#include "Ceil.hpp"
#include "CeilDefault.hpp"

// from VBElemOperations
#include "GDLGlobalCalculations.hpp"
#include "VBElemSurfaceDescriptor.hpp"
#include "VBElemOperationsTypes.hpp"
#include "ComponentIndex.hpp"


class VB_ELEM_OPERATIONS_DLL_EXPORT CeilOperations
{
public:
	enum MaterialSelector {
		TopMaterial,
		BottomMaterial,
		SideMaterial,
		CutElemMaterial
	};

	template <class CeilType>
	static VBElemSurfaceDescriptor	GetMaterial (const CeilType& ceil,
								 const ADB::AttributeSetConstRef& attributeSet,
								 MaterialSelector selector,
								 bool pbcConversion,
								 const ComponentIndex& componentIndex = ComponentIndex::EntireElement);

	static VBElemSurfaceDescriptor GetMaterialOfDefaultCeil (const VBElem::CeilDefault* ceil,
										   const ADB::AttributeSetConstRef& attributeSet,
										   MaterialSelector selector,
										   bool pbcConversion,
										   const ComponentIndex& componentIndex = ComponentIndex::EntireElement);

	static void		GetStandaloneVisibleMaterials (VBElem::CeilConstRef ceil, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& materials);

	static bool		HasVisibleSkin				  (VBElem::CeilConstRef elemRef, EffectiveComposite::Structure structure);

	static UInt32	GetComponentNumber (VBElem::CeilConstRef ceil);

	static short	GetComponentFlags (VBElem::CeilConstRef ceil, const StructureComponentId& structureCompomemtId);

	static GS::Optional<VBEO::ModelElemStructureRef>			GetModelElemStructure			(const VBElem::CeilConstRef& ceil);
	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const VBElem::CeilConstRef& ceil);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(const VBElem::CeilRef& ceil);

	static GS::Optional<VBEO::ModelElemStructureRef>			GetModelElemStructure			(const VBElem::CeilDefault& ceilDef);
	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const VBElem::CeilDefault& ceilDef);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(VBElem::CeilDefault& ceilDef);
};

template <class CeilType>
VBElemSurfaceDescriptor CeilOperations::GetMaterial (const CeilType& ceil,
									const ADB::AttributeSetConstRef& attributeSet,
									MaterialSelector selector,
									bool pbcConversion,
									const ComponentIndex& componentIndex /*= ComponentIndex::EntireElement*/)
{
	const bool hasValidSideMaterial = (ceil->GetElemStructure ().IsBasic () || !componentIndex.IsEntireElement ());

	switch (selector) {
		case TopMaterial:
			if (!pbcConversion || ceil->GetTopMaterial ().GetOverride ())
				return VBElemSurfaceDescriptor::FromSurface (ceil->GetTopMaterial ().GetIndex ());
			break;
		case BottomMaterial:
			if (!pbcConversion || ceil->GetBottomMaterial ().GetOverride ())
				return VBElemSurfaceDescriptor::FromSurface (ceil->GetBottomMaterial ().GetIndex ());
			break;
		case CutElemMaterial:
			if (componentIndex.IsEntireElement () || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (ceil->GetDefaultSideMaterial ().GetIndex ());
			break;
		case SideMaterial:
			if (ceil->GetDefaultSideMaterial ().GetOverride () || !hasValidSideMaterial)
				return VBElemSurfaceDescriptor::FromSurface (ceil->GetDefaultSideMaterial ().GetIndex ());
			break;
		default:
			DBBREAK ();
	}

	ADB::AttributeIndex buildMat = 0;

	if (ceil->GetElemStructure ().IsBasic ()) {
		buildMat = ceil->GetElemStructure ().GetBasicBuildMatIndex ();
	} else {
		DBASSERT (ceil->GetElemStructure ().IsComposite ());
		VBAttr::CompWallConstRef compWall = VBAttr::GetConstCompWall (attributeSet, ceil->GetElemStructure ().GetCompositeIndex ());
		if (compWall != nullptr) {
			short skinIndex;
			if (componentIndex.IsEntireElement () || DBERROR (componentIndex.GetSkinIndex () >= compWall->GetSkinNumber ())) {
				skinIndex = (selector == BottomMaterial) ? compWall->GetSkinNumber () - 1 : 0;
			} else {
				skinIndex = componentIndex.GetSkinIndex ();
			}
			buildMat = compWall->GetSkinBuildingMaterialIndex (skinIndex);
		}
	}

	return VBElemSurfaceDescriptor::FromBuildingMaterialCutSurface (buildMat, attributeSet);
}


#endif // CEIL_OPERATIONS_HPP
