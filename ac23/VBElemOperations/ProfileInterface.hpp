// *********************************************************************************************************************
// Description:		Profile interface for accessing its components
//
// Module:			VBElemOperations
// Namespace:		VBElemOperations
// Contact person:	KV
//
// SG compatible
// *********************************************************************************************************************

#ifndef PROFILEINTERFACE_HPP
#define PROFILEINTERFACE_HPP

#include "Definitions.hpp"
#include "VBElemOperationsExport.hpp"
#include "ComponentIndex.hpp"
#include "SyHatchObject.hpp"
#include "ADBAttributeIndex.hpp"

class ProfileVectorImage;

class VB_ELEM_OPERATIONS_DLL_EXPORT ProfileInterface
{
public:
	virtual ~ProfileInterface ();

	virtual bool	IsValid () const = 0;
	virtual bool	GetComponentBuildingMaterialIndex (const ComponentIndex& componentIndex, ADB::AttributeIndex& buildMatIndex) const = 0;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT VectorImageProfileInterface : public ProfileInterface
{
public:
	VectorImageProfileInterface (const ProfileVectorImage& vectorImage);

	virtual bool			IsValid () const override;
	virtual ULong			GetComponentCount () const;
	virtual bool			GetComponentBuildingMaterialIndex (const ComponentIndex& componentIndex, ADB::AttributeIndex& buildMatIndex) const override;

	const HatchObject&		GetHatchOfComponent (Int32 componentIndex) const;
private:
	GS::Array<HatchObject> hatches;
};

#endif // PROFILEINTERFACE_HPP
