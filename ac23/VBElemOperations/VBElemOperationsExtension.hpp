#ifndef VB_ELEM_OPERATIONS_EXTENSION__HPP
#define VB_ELEM_OPERATIONS_EXTENSION__HPP

#include "Extension.hpp"
#include "GeneralElem.hpp"
#include "VBElementsBase/ModelElement.hpp"
#include "VBElemOperations.hpp"
#include "VBElemOperationsExport.hpp"

//from ModelCalculations
#include "ProjectionSurface.hpp"

//from VectorImage
#include "StructureComponentId.hpp"

// =====================================================================================================================
//  VBElemOperationsExtension
// =====================================================================================================================

class VB_ELEM_OPERATIONS_DLL_EXPORT VBElemOperationsExtension: public GS::Extension
{
public:
	static GS::ExtensionId id;

	VBElemOperationsExtension (GS::ClassInfo* target);
	virtual ~VBElemOperationsExtension ();

	virtual void			ForEachSubElem (EDB::GeneralElemConstRef elemRef, VBElemOperations::GeneralElemConstFn fn) const = 0;

	virtual double			GetAbsoluteTop		(EDB::GeneralElemConstRef elemRef) const = 0;
	virtual double			GetAbsoluteBottom	(EDB::GeneralElemConstRef elemRef) const = 0;

	virtual Box3D			GetRawBounds3D		(EDB::GeneralElemConstRef elemRef) const = 0;
	virtual bool			Is3DType			(EDB::GeneralElemConstRef elemRef) const = 0;

	virtual	short			GetComponentFlags	(EDB::GeneralElemConstRef elemRef, const StructureComponentId& structureComponentId) const = 0;

	virtual void			SetSlantedHeight	(EDB::GeneralElemRef elemRef,
												 double z, 
												 VBElemOperations::SetHeightLocation location) const = 0;

	virtual void			GetStandaloneVisibleMaterials	(EDB::GeneralElemConstRef elemRef,
															 bool pbcConversion, 
															 GS::HashSet<ADB::AttributeIndex>& surfaces) const = 0;

	virtual bool				HasInfoString	(EDB::GeneralElemConstRef elemRef) const = 0;
	virtual void				SetInfoString	(EDB::GeneralElemRef elemRef, const GS::UniString& infoString) const = 0;

	virtual bool				HasVisibleSkin	(EDB::GeneralElemConstRef elemRef, EffectiveComposite::Structure structure) const = 0;

	virtual GS::Optional<VBEO::ModelElemStructureRef>			GetModelElemStructure			(const EDB::GeneralElemConstRef& elemRef) const = 0;
	virtual GS::Array<VBEO::ModelElemStructureRef>				GetModelElemStructures			(const EDB::GeneralElemConstRef& elemRef) const;
	virtual GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const EDB::GeneralElemConstRef& elemRef) const = 0;
	virtual GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(const EDB::GeneralElemRef& elemRef) const = 0;

	virtual VBElem::ParametricProfileIRefList		GetAllIProfiles			(EDB::GeneralElemRef elemRef) const;
	virtual VBElem::ParametricProfileConstIRefList	GetAllConstIProfiles	(EDB::GeneralElemConstRef elemRef) const;

	virtual bool				IsElementEditable (const EDB::GeneralElemConstRef& elemRef) const;
};

#endif // VB_ELEM_OPERATIONS_EXTENSION__HPP
