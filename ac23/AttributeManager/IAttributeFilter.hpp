// *****************************************************************************
// Interface for Filtering Attributes
// AttributeManager, platform-independent
//
// Namespaces:					Contact person:
//	IAttributeFilter				  JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (IATTRIBUTEFILTER_HPP)
#define IATTRIBUTEFILTER_HPP

#pragma once

#include	"GSRoot.hpp"
#include	"AttributeManagerEnv.hpp"
#include	"ADBGeneralDefinitions.hpp"


namespace ADB {

class ATTRIBUTE_DLL_EXPORT IAttributeFilter
{
public:
	virtual bool	Pass (const ADB::GeneralAttributeConstRef&) = 0;

    virtual ~IAttributeFilter ();
};

}

#endif
