// *****************************************************************************
// Interface class for Delete and Replace dialogs of attributes.
// AttributeManager, platform-independent
//
// Namespaces:					Contact person:
//	IAttributeUsageReport			  JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (IATTRIBUTEUSAGEREPORT_HPP)
#define IATTRIBUTEUSAGEREPORT_HPP

#pragma once

#include	"GSRoot.hpp"
#include	"Array.hpp"

#include	"AttributeManagerEnv.hpp"
#include	"ADBAttributeIndex.hpp"
#include	"ADBGeneralDefinitions.hpp"


namespace ADB {

enum AttributeUsageTypes {
	UsedInElems			= 0x0001,
	UsedInAttrs			= 0x0002,
	UsedInFavorites		= 0x0004,
	UsedInMVO			= 0x0008,
	UsedInOverrides		= 0x0010,
	NotReserved			= 0x0020,
	ReservedByOther		= 0x0040
};


class ATTRIBUTE_DLL_EXPORT IAttributeUsageReport {
public:
	virtual	Int32		GetUsageCount			(AttributeUsageTypes usageType, ADB::IDAttribute attributeTypeId = 0) const = 0;
	virtual void		GenerateReport			(void) const = 0;

	virtual ~IAttributeUsageReport ();
};

}

#endif
