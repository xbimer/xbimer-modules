// *****************************************************************************
// Interface for Providing an Existing AttributeSet
// AttributeManager, platform-independent
//
// Namespaces:					Contact person:
//	IAttributeSetProvider			  JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (IATTRIBUTESETPROVIDER_HPP)
#define IATTRIBUTESETPROVIDER_HPP

#pragma once

#include	"GSRoot.hpp"
#include	"AttributeManagerEnv.hpp"
#include	"ADBGeneralDefinitions.hpp"


namespace ADB {

enum AttributeSetSelector {
	ActiveAttributeSet	= 0,
	MainAttributeSet	= 1,
	LayerAttributeSet	= 2
};


class ATTRIBUTE_DLL_EXPORT IAttributeSetProvider
{
public:
	virtual	ADB::AttributeSetConstRef		GetAttributeSet (AttributeSetSelector type = ActiveAttributeSet) const = 0;

	virtual ~IAttributeSetProvider ();
};

}

#endif
