// *****************************************************************************
// Interface for Collecting Library Part References Used by Attributes
// AttributeManager, platform-independent
//
// Namespaces:					Contact person:
//	IUsedLibPartCollection			  JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (IUSEDLIBPARTCOLLECTION_HPP)
#define IUSEDLIBPARTCOLLECTION_HPP

#pragma once

#include	"GSRoot.hpp"
#include	"GSUnID.hpp"
#include	"UniString.hpp"

#include	"AttributeManagerEnv.hpp"
#include	"ADBGeneralDefinitions.hpp"


namespace ADB {

class ATTRIBUTE_DLL_EXPORT IUsedLibPartCollection
{
public:
	virtual void ByUniString (const GS::UniString& name, ADB::GeneralAttributeConstRef attrRef) = 0;
	virtual void ByUnIDAndUniString (const GS::UnID& id, const GS::UniString& name, ADB::GeneralAttributeConstRef attrRef) = 0;

	virtual ~IUsedLibPartCollection ();
};

}

#endif
