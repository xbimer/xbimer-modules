// *****************************************************************************
// Interface for AttributeSelTable
// AttributeManager, platform-independent
//
// Namespaces:					Contact person:
//	IAttributeSelTable				  JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (IATTRIBUTESELTABLE_HPP)
#define IATTRIBUTESELTABLE_HPP

#pragma once

#include	"GSRoot.hpp"
#include	"AttributeManagerEnv.hpp"
#include	"ADBAttributeIndex.hpp"
#include	"ADBGeneralDefinitions.hpp"


namespace ADB {

class ATTRIBUTE_DLL_EXPORT IAttributeSelTable {
public:
	virtual	GSErrCode	Select (ADB::IDAttribute attributeTypeId, const ADB::AttributeIndex& index) = 0;
	virtual	GSErrCode	DeSelect (ADB::IDAttribute attributeTypeId, const ADB::AttributeIndex& index) = 0;

	virtual	bool		IsSelected (ADB::IDAttribute attributeTypeId, const ADB::AttributeIndex& index) const = 0;

	virtual ~IAttributeSelTable ();
};

}

#endif
