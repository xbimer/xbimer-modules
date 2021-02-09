#ifndef ROOF_OPERATIONS_HPP
#define ROOF_OPERATIONS_HPP

#include "VBElemOperationsExport.hpp"

// from VBElements
#include "PolyRoof.hpp"

// from VBAttributes
#include "BuildingMaterial.hpp"
#include "compwall.hpp"

// from VBElemOperations
#include "GDLGlobalCalculations.hpp"
#include "VBElemSurfaceDescriptor.hpp"
#include "ComponentIndex.hpp"

class VB_ELEM_OPERATIONS_DLL_EXPORT RoofOperations
{
public:
	enum MaterialSelector {
		TopMaterial,
		BotMaterial,
		SideMaterial,
		CutElemMaterial
	};

	template <class RoofType>
	static VBElemSurfaceDescriptor	GetMaterial (const RoofType& roof,
								 const ADB::AttributeSetConstRef& attributeSet,
								 MaterialSelector selector,
								 bool pbcConversion,
								 const ComponentIndex& componentIndex = ComponentIndex::EntireElement);

	template <class RoofType>
	static VBElemSurfaceDescriptor	GetStructureMaterial (const RoofType& roof,
										  const ADB::AttributeSetConstRef& attributeSet,
										  MaterialSelector selector,
										  const ComponentIndex& componentIndex = ComponentIndex::EntireElement);

	static void		GetStandaloneVisibleMaterials (VBElem::RoofConstRef roof, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& materials);

	static UInt32	GetComponentNumber (VBElem::RoofConstRef roof);

	static short	GetComponentFlags (VBElem::RoofConstRef roof, const StructureComponentId& structureComponentId);

	static void		EnumerateExpectedPolyRoofPlanes (const VBElem::PolyRoofConstRef& polyRoof, const std::function<void (UInt32 baseEdgeUnId, UIndex levelIdx)>& enumerator);
	
	static void		EnumerateExistingPolyRoofPlanes (const VBElem::PolyRoofConstRef& polyRoof, const std::function<void (UInt32 baseEdgeUnId, UIndex levelIdx)>& enumerator);

};

template <class RoofType>
VBElemSurfaceDescriptor RoofOperations::GetMaterial (	const RoofType& roof,
									const ADB::AttributeSetConstRef& attributeSet,
									MaterialSelector selector,
									bool pbcConversion,
									const ComponentIndex& componentIndex)
{
	const bool hasValidSideMaterial = (roof->GetElemStructure ().IsBasic () || !componentIndex.IsEntireElement ());

	switch (selector) {
		case TopMaterial:
			if (roof->GetOppositeSideMaterial ().GetOverride () || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (roof->GetOppositeSideMaterial ().GetIndex ());
			break;
		case BotMaterial:
			if (roof->GetReferenceSideMaterial ().GetOverride () || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (roof->GetReferenceSideMaterial ().GetIndex ());
			break;
		case SideMaterial:
			if (roof->GetDefaultSideMaterial ().GetOverride () || !hasValidSideMaterial || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (roof->GetDefaultSideMaterial ().GetIndex ());
			break;
		default:
			DBBREAK ();
			//lint -fallthrough
		case CutElemMaterial:
			if (componentIndex.IsEntireElement () || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (roof->GetDefaultSideMaterial ().GetIndex ());
			break;
	}

	return GetStructureMaterial (roof, attributeSet, selector, componentIndex);
}

template <class RoofType>
VBElemSurfaceDescriptor RoofOperations::GetStructureMaterial (	const RoofType& roof,
												const ADB::AttributeSetConstRef& attributeSet,
												MaterialSelector selector,
												const ComponentIndex& componentIndex)
{
	ADB::AttributeIndex buildMat = 0;

	if (roof->GetElemStructure ().IsBasic ()) {
		buildMat = roof->GetElemStructure ().GetBasicBuildMatIndex ();
	} else {
		VBAttr::CompWallConstRef compWall = VBAttr::GetConstCompWall (attributeSet, roof->GetElemStructure ().GetCompositeIndex ());
		if (compWall != nullptr) {
			if (compWall != nullptr) {
				short skinIndex;
				if (componentIndex.IsEntireElement () || DBERROR (componentIndex.GetSkinIndex () >= compWall->GetSkinNumber ())) {
					skinIndex = (selector == BotMaterial) ? compWall->GetSkinNumber () - 1 : 0;
				} else {
					skinIndex = componentIndex.GetSkinIndex ();
				}
				buildMat = compWall->GetSkinBuildingMaterialIndex (skinIndex);
			}
		}
	}

	return VBElemSurfaceDescriptor::FromBuildingMaterialCutSurface (buildMat, attributeSet);
}

#endif // ROOF_OPERATIONS_HPP
