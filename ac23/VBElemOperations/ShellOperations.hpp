#ifndef SHELL_OPERATIONS_HPP
#define SHELL_OPERATIONS_HPP

#include "VBElemOperationsExport.hpp"

// from VBElements
#include "Shell.hpp"

// from VBAttributes
#include "BuildingMaterial.hpp"
#include "compwall.hpp"

// from VBElemOperations
#include "GDLGlobalCalculations.hpp"
#include "VBElemSurfaceDescriptor.hpp"
#include "ComponentIndex.hpp"

class VB_ELEM_OPERATIONS_DLL_EXPORT ShellOperations
{
public:
	enum MaterialSelector {
		ReferenceSideMaterial,
		OppositeSideMaterial,
		SideMaterial,
		CutElemMaterial,
		InnerReferenceSideMaterial,
		InnerOppositeSideMaterial
	};

	template <class ShellType>
	static VBElemSurfaceDescriptor GetMaterial (const ShellType& shell,
								 const ADB::AttributeSetConstRef& attributeSet,
								 MaterialSelector selector,
								 bool pbcConversion,
								 const ComponentIndex& componentIndex = ComponentIndex::EntireElement);

	template <class ShellType>
	static VBElemSurfaceDescriptor	GetStructureMaterial (const ShellType& shell,
										  const ADB::AttributeSetConstRef& attributeSet,
										  MaterialSelector selector,
										  const ComponentIndex& componentIndex = ComponentIndex::EntireElement);

	static void		GetStandaloneVisibleMaterials (VBElem::ShellConstRef shell, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& materials);

	static UInt32	GetComponentNumber (VBElem::ShellConstRef shell);

	static short	GetComponentFlags (VBElem::ShellConstRef shell, const StructureComponentId& structureComponentId);
};

template <class ShellType>
VBElemSurfaceDescriptor ShellOperations::GetMaterial (	const ShellType& shell,
										const ADB::AttributeSetConstRef& attributeSet,
										MaterialSelector selector,
										bool pbcConversion,
										const ComponentIndex& componentIndex)
{
	const bool hasValidSideMaterial = (shell->GetElemStructure ().IsBasic () || !componentIndex.IsEntireElement ());

	switch (selector) {
		case ReferenceSideMaterial:
			if (shell->GetReferenceSideMaterial ().GetOverride () || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (shell->GetReferenceSideMaterial ().GetIndex ());
			break;
		case OppositeSideMaterial:
			if (shell->GetOppositeSideMaterial ().GetOverride () || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (shell->GetOppositeSideMaterial ().GetIndex ());
			break;
		case SideMaterial:
			if (shell->GetDefaultSideMaterial ().GetOverride () || !hasValidSideMaterial || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (shell->GetDefaultSideMaterial ().GetIndex ());
			break;
		default:
			DBBREAK ();
			//lint -fallthrough
		case InnerReferenceSideMaterial:
		case InnerOppositeSideMaterial:
		case CutElemMaterial:
			if (componentIndex.IsEntireElement () || !pbcConversion)
				return VBElemSurfaceDescriptor::FromSurface (shell->GetDefaultSideMaterial ().GetIndex ());
			break;
	}

	return GetStructureMaterial (shell, attributeSet, selector, componentIndex);
}

template <class ShellType>
VBElemSurfaceDescriptor ShellOperations::GetStructureMaterial (	const ShellType& shell,
												const ADB::AttributeSetConstRef& attributeSet,
												MaterialSelector selector,
												const ComponentIndex& componentIndex)
{
	ADB::AttributeIndex buildMat = 0;

	if (shell->GetElemStructure ().IsBasic ()) {
		buildMat = shell->GetElemStructure ().GetBasicBuildMatIndex ();
	} else {
		VBAttr::CompWallConstRef compWall = VBAttr::GetConstCompWall (attributeSet, shell->GetElemStructure ().GetCompositeIndex ());
		if (compWall != nullptr) {
			short skinIndex;
			if (componentIndex.IsEntireElement () || DBERROR (componentIndex.GetSkinIndex () >= compWall->GetSkinNumber ())) {
				skinIndex = (selector == OppositeSideMaterial || selector == InnerOppositeSideMaterial) ? compWall->GetSkinNumber () - 1 : 0;
			} else {
				skinIndex = componentIndex.GetSkinIndex ();
			}
			buildMat = compWall->GetSkinBuildingMaterialIndex (skinIndex);
		}
	}

	return VBElemSurfaceDescriptor::FromBuildingMaterialCutSurface (buildMat, attributeSet);
}

#endif // SHELL_OPERATIONS_HPP
