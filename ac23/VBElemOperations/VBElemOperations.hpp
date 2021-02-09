#ifndef VB_ELEM_OPERATIONS__HPP
#define VB_ELEM_OPERATIONS__HPP

#include "VBElemOperationsExport.hpp"
#include "GeneralElem.hpp"
#include "ProfileCoordinateSystem.hpp"
#include "LabelTypes.hpp"
#include "GDLBasedElemTypes.hpp"
#include "VBElemOperationsTypes.hpp"
#include "StructureComponentId.hpp"

// from VBElements
#include "VBElements/ProfileTypes.hpp"

namespace LM {
	class LibrarySet;
}

struct Box3D;

// =====================================================================================================================
//  VBElemOperations
// =====================================================================================================================

class VB_ELEM_OPERATIONS_DLL_EXPORT VBElemOperations
{
public:
	typedef const std::function<void (const EDB::GeneralElemConstRef&)>& GeneralElemConstFn;

	enum SetHeightLocation {
		SHL_Top,
		SHL_Bottom
	};

	enum RefPointLocation {
		RPL_Beg,
		RPL_End
	};


public:
	static void			ForEachSubElem (EDB::GeneralElemConstRef elemRef, GeneralElemConstFn fn);

	static double		GetAbsoluteTop		(EDB::GeneralElemConstRef elemRef);
	static double		GetAbsoluteBottom	(EDB::GeneralElemConstRef elemRef);

	static Box3D		GetRawBounds3D		(EDB::GeneralElemConstRef elemRef);
	static bool			Is3DType			(EDB::GeneralElemConstRef elemRef);

	static short		GetComponentFlags	(EDB::GeneralElemConstRef elemRef, const StructureComponentId& structureCompomemtId);

	static void			SetSlantedHeight	(EDB::GeneralElemRef elemRef,
											 double z,
											 VBElemOperations::SetHeightLocation location);

	static void			GetStandaloneVisibleMaterials (EDB::GeneralElemConstRef elemRef, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& surfaces);
	
	static bool			ElemHasVisibleSkin	(EDB::GeneralElemConstRef elemRef, 
											EffectiveComposite::Structure structure);

	static void					RelinkElemAfterCopy (const EDB::GeneralElemConstRef& originalElem, const EDB::GeneralElemRef& newElem);

	static bool					HasInfoString		(EDB::GeneralElemConstRef elemRef);
	static GS::UniString		GetInfoString		(EDB::GeneralElemConstRef elemRef);
	static void					SetInfoString		(EDB::GeneralElemRef elemRef, const GS::UniString infoString);

	static VBElem::LabelRef		GetListingLabel		(EDB::GeneralElemConstRef generalElem);

	static GS::UniString		GetLibPartNameById	(const LM::LibPartId& libPartId, const LM::LibrarySet* librarySet);
	static GS::UniString		GetLibPartName		(EDB::GeneralElemConstRef gdlBasedElem, const LM::LibrarySet* librarySet);
	static GS::UniString		GetLibPartName		(const EDB::DefaultElem* gdlBasedElemDefault, const LM::LibrarySet* librarySet);

	static GS::Optional<VBEO::ModelElemStructureRef>			GetModelElemStructure			(const EDB::GeneralElemConstRef& elemRef);
	static GS::Array<VBEO::ModelElemStructureRef>				GetModelElemStructures			(const EDB::GeneralElemConstRef& elemRef);
	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const EDB::GeneralElemConstRef& elemRef);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(const EDB::GeneralElemRef& elemRef);

	static VBElem::ParametricProfileIRefList			GetAllIProfiles			(EDB::GeneralElemRef elemRef);
	static VBElem::ParametricProfileConstIRefList		GetAllConstIProfiles	(EDB::GeneralElemConstRef elemRef);
	static GS::Optional<ADB::AttributeIndex>			GetLayer				(const EDB::GeneralElemConstRef& elemRef); //#MV_REFACTOR nem biztos hogy jo, hogy ez optional

	static bool					IsElementEditable (const EDB::GeneralElemConstRef& elemRef);
};

#endif // VB_ELEM_OPERATIONS__HPP
