// *****************************************************************************
// The interface of the attribute manager
// AttributeManager, platform-independent
//
// Namespaces:        Contact person:
//	IAttributeRegister		JG
//	IAttributeNewIndexTable	JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (ATTRIBUTEMANAGER_HPP)
#define ATTRIBUTEMANAGER_HPP

#pragma once

//============================== Includes =================================

#include	"AttributeManagerEnv.hpp"
#include	"AttributeTypeIDs.hpp"
#include	"ADBGeneralDefinitions.hpp"
#include	"GSRoot.hpp"
#include	"FieldMask.hpp"

#include	"ODBIdSpace.hpp"
#include	"ODBObject.hpp"
#include	"ODBReference.hpp"

#include	"TWPermissionIDs.h"

#include	"Array.hpp"

#include	"ADBAttributeIndex.hpp"

//============================= Error Codes ===============================

const Int32	ModuleId = 300;
const Int32	ErrorStart = GS::ErrorFlagMask | (ModuleId << 16);

//-------------------------------------------------------------------------

enum AttributeManagerErrorTypes {
	BadAttributeTypeIdError			= ErrorStart +  0,		// invalid attribute type id
	BadAttributeNameError			= ErrorStart +  1,		// invalid attribute name
	BadAttributeLengthError			= ErrorStart +  2,		// invalid attribute data length
	BadAttributeIndexError			= ErrorStart +  3,		// invalid attribute index
	AttributeNameNotFoundError		= ErrorStart +  4,		// attribute with the given name not found
	DeletedAttributeTypeError		= ErrorStart +  5,		// deleted attribute type
	DeletedAttributeError			= ErrorStart +  6,		// deleted attribute
	DuplicatedAttributeNameError	= ErrorStart +  7,		// duplicated attribute name
	DuplicatedAttributeIndexError	= ErrorStart +  8,		// duplicated attribute index
	ExistAttributeTypeError 		= ErrorStart +  9,		// existing attribute type
	ExistAttributeNameError 		= ErrorStart + 10,		// existing attribute name
	AttributeReferenceNumberError 	= ErrorStart + 11,		// invalid reference number
	AttributeResourceNotFoundError 	= ErrorStart + 12,		// attribute resource not found
	BadAttributeSortRuleError 	 	= ErrorStart + 13,		// invalid attribute sorting rule
	BadAttributeSetIndexError 		= ErrorStart + 14,		// invalid attribute set index
	DeletedAttributeSetError 		= ErrorStart + 15,		// deleted attribute set
	BadAttributeResourceTypeError	= ErrorStart + 16,		// attribute resource name is unknown
	AttributeSetNotExistError		= ErrorStart + 17,		// not existing attribute set
	CallBackFunctionNotExistError	= ErrorStart + 18,		// not existing callback function
	BadAttributeError				= ErrorStart + 19,		// bad attribute (e.g. loaded from plan file)
	BadAttributeReferenceError		= ErrorStart + 20		// invalid attribute reference
};

namespace ADB {

const short AttributeNameLength	= 256;	// ATTRIBUTENAMELENGTH maximum length of an attribute name from model_I.h

enum PercentFillTypes {
	PercentFill25	= 0,
	PercentFill50	= 1,
	PercentFill75	= 2
};
}

//-------------------------------------------------------------------------

typedef GSErrCode	SortFunction (ADB::AttributeSetConstRef	attributeSet,
								  ADB::IDAttribute			attributeTypeId,
								  GS::Array <ADB::AttributeIndex>& indexes);

//--------------------------------------------------------------------------

namespace ADB {

enum SortMethod {
	SortByName			= 1,	// sort by attribute name
	SortByIndex			= 2,	// sort by attribute index
	SortByExtension		= 3,	// sort by attribute name extension
	SortByThisExtension	= 4		// sort by given name extension
};

enum SortOrder {
	SortAscending		= 1,
	SortDescending		= 2
};

enum AttributeTypeFlags {
	MergeableAttribute			= 1,
	ListableAttribute			= 2,
	CopyableAttribute			= 4,
	CopyableFrom3DAttribute		= 8
};

}

//=========================== Class IAttributeRegister =======================
//
// Using IAttributeRegister the attributes of the application can be registered.
// IAttributeRegister generates a dynamic attribute id from the static attribute id (refId).
// The member functions of AttributeSet use the dynamic ids.
//
// IAttributeRegister stores function pointers for converting attribute types from 7.0 to
// actual version and vice versa. IAttributeRegister stores also the creation and initialization
// functions for attributes.
//
//=============================================================================

namespace VBUtil {
	class Owner;
}

namespace ADB {

class ATTRIBUTE_DLL_EXPORT IAttributeRegister {
public:
		// create methods

	virtual IDAttribute		NewAttributeType (GSType						refId,
											  const VBUtil::Owner&			owner,
											  const GS::Guid&				attributeClassId,
											  const GS::Guid&				containerGuid,
											  const GS::UniString&			attributeTypeName = "",
											  TWCB::PermissionID			createPermissionID = TWCB::NoPermission,
											  TWCB::PermissionID			deleteModifyPermissionID = TWCB::NoPermission,
											  GSFlags						attributeTypeFlags = 0) = 0;

		// get methods

	virtual IDAttribute					GetMaxID (void) const = 0;
	virtual	GSType						GetRefId (IDAttribute id) const = 0;
	virtual	const GS::Guid&				GetAttributeClassId (IDAttribute id) const = 0;
	virtual VBUtil::Owner				GetOwner (IDAttribute id) const = 0;
	virtual const GS::Guid&				GetContainerGuid (IDAttribute id) const = 0;
	virtual GS::UniString				GetAttributeTypeName (IDAttribute id) const = 0;
	virtual TWCB::PermissionID			GetCreatePermissionID (IDAttribute id) const = 0;
	virtual TWCB::PermissionID			GetDeleteModifyPermissionID (IDAttribute id) const = 0;

	virtual IDAttribute					GetId (GSType refId) const = 0;
	virtual IDAttribute					GetId (const GS::Guid& attributeClassId) const = 0;

	virtual bool						IsMergeableAttribute (IDAttribute id) const = 0;
	virtual bool						IsListableAttribute (IDAttribute id) const = 0;
	virtual bool						IsCopyableAttribute (IDAttribute id) const = 0;
	virtual bool						IsCopyableFrom3DAttribute (IDAttribute id) const = 0;
	virtual void						SetAttributeMergeable (IDAttribute id, const bool isMergeableAttribute) = 0;

        // destructor
 
	virtual ~IAttributeRegister ();
};

}

//=========== External functions for interface IAttributeRegister =========

ATTRIBUTE_DLL_EXPORT	ADB::IAttributeRegister*	CreateAttributeRegister (void);
ATTRIBUTE_DLL_EXPORT	void						DisposeAttributeRegister (ADB::IAttributeRegister** attributeRegister);


//==================== Attribute Set Container functions ==================

namespace ADB {
	class IAttributeSetProvider;
}

ATTRIBUTE_DLL_EXPORT ADB::AttributeSetRef			InitAttributeSetContainer (const ADB::IAttributeRegister* myAttributeRegister, const ODB::IdSpace* idSpace);
ATTRIBUTE_DLL_EXPORT void							FreeAttributeSetContainer (void);

ATTRIBUTE_DLL_EXPORT const ADB::IAttributeRegister*	GetAttributeRegister (void);
ATTRIBUTE_DLL_EXPORT short							GetCurrentAttributeSetIndex (void);								// deprecated, will be removed - use GetCurrentAttributeSet instead
ATTRIBUTE_DLL_EXPORT ADB::AttributeSetRef			GetCurrentAttributeSet (void);
ATTRIBUTE_DLL_EXPORT ADB::AttributeSetConstRef		GetCurrentConstAttributeSet (void);
ATTRIBUTE_DLL_EXPORT ADB::IAttributeSetProvider*	GetCurrentAttributeSetProvider (void);
ATTRIBUTE_DLL_EXPORT bool							IsValidAttributeSet (ADB::AttributeSetConstRef attributeSet);

ATTRIBUTE_DLL_EXPORT ADB::AttributeSetRef			CreateAttributeSet (const ODB::IdSpace* idSpace = nullptr, const GS::Guid& requiredId = GS::NULLGuid, bool useFixContainerIds = false);
ATTRIBUTE_DLL_EXPORT ADB::AttributeSetRef			CloneAttributeSet (ADB::AttributeSetConstRef attributeSet, ODB::IdSpace* idSpace = nullptr);
ATTRIBUTE_DLL_EXPORT void							DisposeAttributeSet (ADB::AttributeSetRef attributeSet);

ATTRIBUTE_DLL_EXPORT short							NewAttributeSet (const ODB::IdSpace* idSpace = nullptr);		// deprecated, will be removed - use CreateAttributeSet instead
ATTRIBUTE_DLL_EXPORT GSErrCode						FreeAttributeSet (short attributeSetIndex);						// deprecated, will be removed - use DisposeAttributeSet instead

ATTRIBUTE_DLL_EXPORT short							RegisterAttributeSet (ADB::AttributeSetRef attributeSet);
ATTRIBUTE_DLL_EXPORT GSErrCode						DeregisterAttributeSet (ADB::AttributeSetRef attributeSet);

ATTRIBUTE_DLL_EXPORT GSErrCode						SetCurrentAttributeSet (ADB::AttributeSetConstRef attributeSet);
ATTRIBUTE_DLL_EXPORT GSErrCode						SelectAttributeSet (short attributeSetIndex);					// deprecated, will be removed - use SetCurrentAttributeSet instead
ATTRIBUTE_DLL_EXPORT GSErrCode						SwapAttributeSet (short attributeSetIndex);						// deprecated, will be removed - use SetCurrentAttributeSet instead


// ============== Class DisableAccesFromCurrentThreadScope ================

namespace ADB {

class ATTRIBUTE_DLL_EXPORT DisableAccesFromCurrentThreadScope {
public:
	DisableAccesFromCurrentThreadScope ();
	~DisableAccesFromCurrentThreadScope ();
};

}


//======= External functions for interface IAttributeNewIndexTable and IAttributeSelTable =========

namespace ADB {
	class IAttributeNewIndexTable;
	class IAttributeSelTable;
}

ATTRIBUTE_DLL_EXPORT ADB::IAttributeNewIndexTable*		CreateAttributeNewIndexTable ();
ATTRIBUTE_DLL_EXPORT void								DisposeAttributeNewIndexTable (ADB::IAttributeNewIndexTable** attributeNewIndexTable);

ATTRIBUTE_DLL_EXPORT ADB::IAttributeSelTable*			CreateAttributeSelTable ();
ATTRIBUTE_DLL_EXPORT void								DisposeAttributeSelTable (ADB::IAttributeSelTable** attributeSelTable);

ATTRIBUTE_DLL_EXPORT ADB::AttributeIndex				GetNewAttributeIndex (const ADB::IAttributeNewIndexTable* newIndexTable, ADB::IDAttribute id, ADB::AttributeIndex oldIndex);

#endif
