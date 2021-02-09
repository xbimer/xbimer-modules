// *****************************************************************************
// Attribute Index Reference
// AttributeManager, platform-independent
//
// Namespaces:        Contact person:
//		ADB					JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (ADBATTRIBUTEINDEX_HPP)
#define ADBATTRIBUTEINDEX_HPP

#pragma once

// from GSRoot
#include "Object.hpp"
#include "HashCommon.hpp"

// from GSUtils
#include "GSUtilsDefs.h"

// from VBAttributes
#include "AttributeManagerEnv.hpp"

// from ObjectDatabase
#include "ODBFieldType.hpp"


namespace ADB {

class ATTRIBUTE_DLL_EXPORT AttributeIndex : public GS::Object
{
	DECLARE_DYNAMIC_CLASS_INFO (AttributeIndex)

private:
	GSAttributeIndex		attributeIndex;

public:
	AttributeIndex ();
	AttributeIndex (const AttributeIndex& source);
	AttributeIndex (const GSAttributeIndex& gsAttrInd);
	AttributeIndex (const GS::UniString& value);

	virtual ~AttributeIndex ();

	AttributeIndex&			operator= (const AttributeIndex& source);
	AttributeIndex&			operator= (const GSAttributeIndex& value);
	AttributeIndex&			operator= (const GS::UniString& value);
	AttributeIndex&			operator++ ();
	AttributeIndex			operator++ (int);

	AttributeIndex			operator+ (const AttributeIndex& rightOp) const		{	return ADB::AttributeIndex (attributeIndex + rightOp.ToGSAttributeIndex ());	}
	AttributeIndex			operator- (const AttributeIndex& rightOp) const		{	return ADB::AttributeIndex (attributeIndex - rightOp.ToGSAttributeIndex ());	}

	inline bool				operator== (const AttributeIndex& rightOp) const	{	return (attributeIndex == rightOp.attributeIndex);	}
	inline bool				operator!= (const AttributeIndex& rightOp) const	{	return !(attributeIndex == rightOp.attributeIndex);	}
	inline bool				operator<  (const AttributeIndex& rightOp) const	{	return (attributeIndex < rightOp.attributeIndex);	}
	inline bool				operator>  (const AttributeIndex& rightOp) const	{	return (attributeIndex > rightOp.attributeIndex);	}
	inline bool				operator<= (const AttributeIndex& rightOp) const	{	return (attributeIndex <= rightOp.attributeIndex);	}
	inline bool				operator>= (const AttributeIndex& rightOp) const	{	return (attributeIndex >= rightOp.attributeIndex);	}

	GSAttributeIndex		ToGSAttributeIndex (void) const;
	GS::UniString			ToUniString (void) const;

	ULong					GenerateHashValue (void) const;

	// GS::Object methods
	virtual GSErrCode		Read (GS::IChannel& ic) override;
	virtual GSErrCode		Write (GS::OChannel& oc) const override;
    virtual GS::ErrorStatus	Store   (GS::ObjectState& os) const override;
    virtual GS::ErrorStatus	Restore (const GS::ObjectState& os) override;

	// Compatibility methods
	GSErrCode				ReadAsShortInteger (GS::IChannel& ic);
	GSErrCode				WriteAsShortInteger (GS::OChannel& oc) const;

	// XML I/O methods
	virtual GSErrCode		ReadXML (GS::XMLIChannel& ic, const char* tagName);
	virtual GSErrCode		WriteXML (GS::XMLOChannel& oc, const char* tagName) const;
	virtual GSErrCode		ReadXMLAttr (GS::XMLIChannel& ic, const char* tagName);
	virtual GSErrCode		WriteXMLAttr (GS::XMLOChannel& oc, const char* tagName) const;

	// Debug
	virtual void			Print (GS::OChannel& oc) const override;
};


ATTRIBUTE_DLL_EXPORT extern const AttributeIndex	InvalidAttributeIndex;

ATTRIBUTE_DLL_EXPORT AttributeIndex	GenerateUniqueAttributeIndex (const GS::HashSet<AttributeIndex>&	indicesAlreadyUsed,
																  const AttributeIndex&					searchStartsFromIndex = InvalidAttributeIndex);
}	// namespace ADB


namespace ODB {

template<>	ATTRIBUTE_DLL_EXPORT const ODB::FieldType&		GetFieldType<ADB::AttributeIndex> (void);

}	// namespace ODB


namespace GS {

inline ULong	GenerateHashValue (const ADB::AttributeIndex& attributeIndex);

}	// namespace GS


inline ULong	GS::GenerateHashValue (const ADB::AttributeIndex& attributeIndex)
{
	return GS::GenerateHashValue (attributeIndex.GenerateHashValue ());
}

#endif
