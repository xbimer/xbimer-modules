// *********************************************************************************************************************
// Description:		Phasing Manager utilities
//
// Module:			Phasing
// Namespace:		PhasingManager
// Contact person:	FP
//
// SG compatible
// *********************************************************************************************************************

#if !defined (PHASINGMANAGERUTILS_HPP)
#define PHASINGMANAGERUTILS_HPP

#pragma once

// --- Includes	--------------------------------------------------------------------------------------------------------
#include	"PhasingExport.hpp"
#include	"PhasingTypes.hpp"

// from ElementManager
#include	"Project.hpp"

// from LibraryManager
#include	"LibrarySet.hpp"

// from VBElements
#include "OpeningInFillTypes.hpp"
#include "OpeningPatchInFillTypes.hpp"
#include "OpeningTypes.hpp"
#include "VBElementsBase/ModelElementTypes.hpp"

// --- Predeclarations	------------------------------------------------------------------------------------------------

namespace PhasingManager {

	// --- Constant definitions	--------------------------------------------------------------------------------------------

	// --- Type definitions	------------------------------------------------------------------------------------------------

	// --- Variable declarations -------------------------------------------------------------------------------------------

	// --- Function declarations -------------------------------------------------------------------------------------------
PHASING_DLL_EXPORT Phasing::PlanTypeRef			FindPlanTypeCategory (EDB::ProjectRef prj);
PHASING_DLL_EXPORT Phasing::PlanTypeConstRef	FindPlanTypeCategory (EDB::ProjectConstRef prj);

PHASING_DLL_EXPORT Phasing::RenovationStatusRef	FindRenovationStatusCategory (EDB::ProjectRef prj);
PHASING_DLL_EXPORT Phasing::RenovationStatusConstRef	FindRenovationStatusCategory (EDB::ProjectConstRef prj);

//--- Plan Type related methods
PHASING_DLL_EXPORT
Phasing::PlanTypeValueConstRef		GetDerivedElemPlanType (const EDB::GeneralElemConstRef& elem, const Phasing::PlanTypeConstRef& planTypeCategory);
PHASING_DLL_EXPORT
bool								IsPlanTypeDependent (const Phasing::PlanTypeValueConstRef& planTypeValue);
PHASING_DLL_EXPORT
Phasing::PlanTypeConstRef			GetPlanTypeCategory (CAT::CategorySetConstRef categorySet);
PHASING_DLL_EXPORT 
Phasing::PlanTypeRef				GetPlanTypeCategory (EDB::ProjectRef prj, bool isMainProject);
PHASING_DLL_EXPORT
Phasing::PlanTypeConstRef			GetPlanTypeCategory (EDB::ProjectConstRef prj, bool isMainProject);
PHASING_DLL_EXPORT
Phasing::PlanTypeValueConstRef		FindPlanTypeValueByName (Phasing::PlanTypeConstRef planTypeCategory, const GS::UniString& planTypeName);
PHASING_DLL_EXPORT
Phasing::PlanTypeValueConstRef		FindPlanTypeValueByNameFailureProof (Phasing::PlanTypeConstRef planTypeCategory, const GS::UniString& planTypeName);
PHASING_DLL_EXPORT
Phasing::PlanTypeValueConstRef		FindFirstValidPlanTypeValue (Phasing::PlanTypeConstRef planTypeCategory);

//--- Renovation Status related methods
PHASING_DLL_EXPORT
Phasing::RenovationStatusType		GetDerivedRenovationStatus (const EDB::GeneralElemConstRef& elem, const Phasing::RenovationStatusConstRef& renovationStatusCategory, 
																const LM::LibrarySet* librarySet, bool* defined);
PHASING_DLL_EXPORT
Phasing::RenovationStatusType			RenovationStatusValueToType (const Phasing::RenovationStatusValueConstRef& renovationStatusValue);
PHASING_DLL_EXPORT
Phasing::RenovationStatusValueConstRef	ProjectRenovationStatusTypeToValue (const Phasing::RenovationStatusType renovationStatusType, const EDB::ProjectConstRef& prj);
PHASING_DLL_EXPORT
Phasing::RenovationStatusType		GetOppositeStatus (Phasing::RenovationStatusType holeStatus);

//--- GraphicOverride related methods
PHASING_DLL_EXPORT
bool								IsOverrideEnabled (const Phasing::RenovationStatusType status, const Phasing::PlanTypeValueConstRef& planTypeValue);

PHASING_DLL_EXPORT
bool								IsStatusVisible (const Phasing::RenovationStatusType status, const Phasing::PlanTypeValueConstRef& planTypeValue);

PHASING_DLL_EXPORT
bool								IsStatusShown (const Phasing::RenovationStatusType status, const Phasing::PlanTypeValueConstRef& planTypeValue);

PHASING_DLL_EXPORT
bool								IsOverrideSymbolic (const Phasing::RenovationStatusType status, const Phasing::PlanTypeValueConstRef& planTypeValue);

PHASING_DLL_EXPORT
bool								IsElemVisible (const EDB::GeneralElemConstRef& elem, const Phasing::PlanTypeValueConstRef& planTypeValue, 
												   const Phasing::PlanTypeConstRef& planTypeCategory, const Phasing::RenovationStatusConstRef& renovationStatusCategory, const LM::LibrarySet* librarySet);
PHASING_DLL_EXPORT
bool								IsElemVisible (const EDB::GeneralElemConstRef& elem, const Phasing::RenovationStatusType elemStatus, const Phasing::PlanTypeValueConstRef& planTypeValue,
												   const Phasing::PlanTypeConstRef& planTypeCategory, const Phasing::RenovationStatusConstRef& renovationStatusCategory, const LM::LibrarySet* librarySet);
PHASING_DLL_EXPORT
bool								IsWindowDoorVisible (const EDB::GeneralElemConstRef& elem, const Phasing::PlanTypeValueConstRef& planTypeValue, 
														const Phasing::PlanTypeConstRef& planTypeCategory, const Phasing::RenovationStatusConstRef& renovationStatusCategory);

PHASING_DLL_EXPORT
bool								IsHoleReplacementVisible (const EDB::GeneralElemConstRef& opening, const Phasing::PlanTypeValueConstRef& planTypeValue, 
															  const Phasing::PlanTypeConstRef& planTypeCategory, const Phasing::RenovationStatusConstRef& renovationStatusCategory);

class PHASING_DLL_EXPORT OpeningPhasingLogic
{
public:
	enum class CovisibilityType 
	{
		AllowPatchAndFrameCovisibility, 
		DisablePatchAndFrameCovisibility
	};

	OpeningPhasingLogic (
		const Phasing::PlanTypeValueConstRef&		actPlanType,
		const Phasing::PlanTypeConstRef&			planTypeCategory,
		const Phasing::RenovationStatusConstRef&	renovationStatusCategory,
		const VBElem::OpeningFrameInFillConstRef&	openingFrameInFill, 
		CovisibilityType							covisibilityOfFrameAndPatchInit = CovisibilityType::AllowPatchAndFrameCovisibility);

	OpeningPhasingLogic (
		const Phasing::PlanTypeValueConstRef&		actPlanType,
		const Phasing::PlanTypeConstRef&			planTypeCategory,
		const Phasing::RenovationStatusConstRef&	renovationStatusCategory,
		const VBElem::OpeningPatchInFillConstRef&	openingPatchInFill,
		CovisibilityType							covisibilityOfFrameAndPatchInit = CovisibilityType::AllowPatchAndFrameCovisibility);

	bool		IsFrameInfillPresent () const;
	bool		IsPatchInfillPresent () const;
	bool		NeedPiercingOfHostElem () const;
	bool		IsFrameInFillShownWithoutOverride () const;

	bool		ExistingStatusVisible () const { return existingStatusDisplay != Phasing::RenovationStatusDisplay::NotDisplay; }
	bool		ToDemolishStatusVisible () const { return toDemolishStatusDisplay != Phasing::RenovationStatusDisplay::NotDisplay; }
	bool		NewStatusVisible () const { return newStatusDisplay != Phasing::RenovationStatusDisplay::NotDisplay; }
	bool		IsRenovationStatusVisible (Phasing::RenovationStatusType type) const;
	bool		IsRenovationStatusShownWithoutOverride (Phasing::RenovationStatusType type) const;

	inline Phasing::RenovationStatusType GetOpeningPatchInFillRenoStatus () const { return openingPatchInFillRenoStatus; }
	inline Phasing::RenovationStatusType GetOpeningFrameInFillRenoStatus () const { return openingFrameInFillRenoStatus; }
	inline Phasing::RenovationStatusType GetOpeningRenoStatus () const { return openingRenoStatus; }
	inline Phasing::RenovationStatusType GetHostElemRenoStatus () const { return hostElemRenoStatus; }

	inline const VBElem::OpeningPatchInFillConstRef& GetOpeningPatchInFill () const { return openingPatchInFill; }
	inline const VBElem::OpeningFrameInFillConstRef& GetOpeningFrameInFill () const { return openingFrameInFill; }
	inline const VBElem::OpeningConstRef& GetOpening () const { return opening; }
	inline const VBElem::ModelElementConstRef& GetHostElem () const { return hostElem; }

private:
	void FinalizeConstr (
		const Phasing::PlanTypeValueConstRef&		actPlanType,
		const Phasing::PlanTypeConstRef&			planTypeCategory,
		const Phasing::RenovationStatusConstRef&	renovationStatusCategory
	);
	CovisibilityType covisibilityOfFrameAndPatch;

	VBElem::OpeningPatchInFillConstRef openingPatchInFill;
	VBElem::OpeningFrameInFillConstRef openingFrameInFill;
	VBElem::OpeningConstRef opening;
	VBElem::ModelElementConstRef hostElem;

	Phasing::RenovationStatusType openingPatchInFillRenoStatus;
	Phasing::RenovationStatusType openingFrameInFillRenoStatus;
	Phasing::RenovationStatusType openingRenoStatus;
	Phasing::RenovationStatusType hostElemRenoStatus;

	bool openingFrameInFillVisibleByPhasing;
	bool openingPatchInFillVisibleByPhasing;
	bool openingVisibleByPhasing;
	bool hostElemVisibleByPhasing;

	Phasing::RenovationStatusDisplay existingStatusDisplay;
	Phasing::RenovationStatusDisplay toDemolishStatusDisplay;
	Phasing::RenovationStatusDisplay newStatusDisplay;


	//reconstruction of usual filters: 
	bool		ProbablyIsAfterDemolition () const;

	bool		isFrameInfillPresent;
	bool		isPatchInfillPresent;
	bool		needPiercingOfHostElem;
};


}	// namespace PhasingManager

#endif //PHASINGMANAGERUTILS_HPP