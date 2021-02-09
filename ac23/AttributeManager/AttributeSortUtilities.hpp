// *****************************************************************************
// Sort, filter functions
// AttributeManager, platform-independent
//
// *****************************************************************************

#if !defined (SORTUTILITIES_HPP)
#define SORTUTILITIES_HPP

#pragma once

#include	"AttributeManager.hpp"
#include	"HashTable.hpp"
#include	"IAttributeFilter.hpp"
#include	"generalattribute.hpp"


namespace ADB {

class ATTRIBUTE_DLL_EXPORT AttributeExtensionFilter : public ADB::IAttributeFilter
{
private:
	GS::UniString passThisExtensionOnly;

public:
	AttributeExtensionFilter (const GS::UniString& filterByThisExtension) : passThisExtensionOnly (filterByThisExtension) {}
	virtual bool  Pass (const ADB::GeneralAttributeConstRef& attribute) override;
};

}

ATTRIBUTE_DLL_EXPORT GSErrCode	AttributeSortByName (ADB::AttributeSetConstRef	attributeSet,
													 ADB::IDAttribute			attributeTypeId,
													 GS::Array <ADB::AttributeIndex>& indexes);

ATTRIBUTE_DLL_EXPORT GSErrCode	AttributeSortByExtension (ADB::AttributeSetConstRef	attributeSet,
														  ADB::IDAttribute			attributeTypeId,
														  GS::Array <ADB::AttributeIndex>& indexes);
#endif