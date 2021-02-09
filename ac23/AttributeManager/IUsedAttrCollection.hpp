// *****************************************************************************
// Interface for Collecting Used Attribute References
// AttributeManager, platform-independent
//
// Namespaces:					Contact person:
//	IUsedAttrCollection				  JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (IUSEDATTRCOLLECTION_HPP)
#define IUSEDATTRCOLLECTION_HPP

#pragma once

#include	"GSRoot.hpp"
#include	"AttributeManagerEnv.hpp"
#include	"ADBAttributeIndex.hpp"
#include	"ADBGeneralDefinitions.hpp"


namespace ADB {

class ATTRIBUTE_DLL_EXPORT IUsedAttrCollection
{
public:
	virtual void MarkAttributeAsUsed	(ADB::IDAttribute idAttribute, const ADB::AttributeIndex& attributeIndex) = 0;
	virtual bool IsAttributeUsed		(ADB::IDAttribute idAttribute, const ADB::AttributeIndex& attributeIndex) const = 0;
	virtual bool SkipAttributeType		(ADB::IDAttribute idAttribute) const = 0;

	virtual bool SkipSubObjects			() const;

    virtual ~IUsedAttrCollection ();
};

class ATTRIBUTE_DLL_EXPORT IUsedPenCollection
{
public:
	virtual void MarkPenAsUsed	(short penIndex) = 0;
	virtual bool IsPenUsed		(short penIndex) const = 0;
	virtual ~IUsedPenCollection ();
};

}

#endif
