// *****************************************************************************
// Interface for AttributeNewIndexTable
// AttributeManager, platform-independent
//
// Namespaces:					Contact person:
//	IAttributeNewIndexTable			  JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (USEDATTRCOLLECTION_HPP)
#define USEDATTRCOLLECTION_HPP

#pragma once

#include	"GSRoot.hpp"
#include	"AttributeManagerEnv.hpp"
#include	"ADBAttributeIndex.hpp"
#include	"ADBGeneralDefinitions.hpp"


namespace ADB {

class ATTRIBUTE_DLL_EXPORT IAttributeNewIndexTable {
public:
	virtual	GSErrCode	SetNewIndex (ADB::IDAttribute attributeTypeId, const ADB::AttributeIndex& index, const ADB::AttributeIndex& newIndex) = 0;
	virtual	GSErrCode	ClearNewIndex (ADB::IDAttribute attributeTypeId, const ADB::AttributeIndex& index) = 0;
	virtual	void		SetAttributeTypeAsUsed (ADB::IDAttribute attributeTypeId) = 0;
	virtual	void		SetAttributeTypeAsUnused (ADB::IDAttribute attributeTypeId) = 0;

	virtual	const ADB::AttributeIndex		GetMaxIndex (ADB::IDAttribute attributeTypeId) const = 0;
	virtual	const ADB::AttributeIndex		GetNewIndex (ADB::IDAttribute attributeTypeId, const ADB::AttributeIndex& index) const = 0;
	virtual	bool							IsUsedAttributeType (ADB::IDAttribute attributeTypeId) const = 0;
	virtual	bool							HasChanged (bool isMerge = false, Int32 attrFilterFlags = 0) const = 0;

	virtual ADB::AttributeSetConstRef		GetAttributeSet (void) const = 0;

	virtual ~IAttributeNewIndexTable ();
};

}

#endif
