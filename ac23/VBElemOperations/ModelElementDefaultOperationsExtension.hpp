#ifndef MODELELEMENTDEFAULT_OPERATIONS_EXTENSION__HPP
#define MODELELEMENTDEFAULT_OPERATIONS_EXTENSION__HPP

// from VBElemOperations
#include "VBElemDefaultOperationsExtension.hpp"

// from VectorImage
#include "StretchData.hpp"
#include "ProfileVectorImage.hpp"

// from Geometry
#include "Box2DData.h"
#include "Box3DData.h"


// from VBElements
#include "ParametricProfile.hpp"
#include "VBElementsBase/ModelElementDefault.hpp"


#include "HashSet.hpp"


// =====================================================================================================================
//  ModelElementDefaultOperationsExtension
// =====================================================================================================================

class VB_ELEM_OPERATIONS_DLL_EXPORT ModelElementDefaultOperationsExtension: public VBElemDefaultOperationsExtension
{
public:
	ModelElementDefaultOperationsExtension (GS::ClassInfo* target) : VBElemDefaultOperationsExtension (target)
	{
	}

	virtual ~ModelElementDefaultOperationsExtension ();

	bool	HasInfoString () const override
	{
		return true;
	}

	virtual GS::UniString	GetInfoString (const EDB::DefaultElem& defaultElem) const override
	{
		return static_cast<const VBElem::ModelElementDefault&> (defaultElem).GetInfoString ();
	}

	virtual void	SetInfoString (EDB::DefaultElem& defaultElem, const GS::UniString& infoString) const override
	{
		static_cast<VBElem::ModelElementDefault&> (defaultElem).SetInfoString (infoString);
	}

	virtual GS::Optional<VBEO::ModelElemStructureRef>			GetModelElemStructure (const EDB::DefaultElem& defaultElem) const override;
	virtual GS::Array<VBEO::ModelElemStructureRef>				GetModelElemStructures (const EDB::DefaultElem& defaultElem) const override;
	virtual GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance (const EDB::DefaultElem& defaultElem) const override;
	virtual GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier (EDB::DefaultElem& defaultElem) const override;

	virtual VBElem::ParametricProfileList		GetIProfiles (EDB::DefaultElem& /*defaultElem*/) const  override
	{
		return VBElem::ParametricProfileList ();
	}

	virtual VBElem::ParametricConstProfileList	GetConstIProfiles (const EDB::DefaultElem& /*defaultElem*/) const override
	{	
		return VBElem::ParametricConstProfileList ();
	}

	virtual Box2DData							GetFloorPlanBox (const EDB::DefaultElem& defaultElem) const;
	virtual Box3D								Get3DBounds (const EDB::DefaultElem& defaultElem) const;
};


#endif // MODELELEMENTDEFAULT_OPERATIONS_EXTENSION__HPP
