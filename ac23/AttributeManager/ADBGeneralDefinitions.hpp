// *********************************************************************************************************************
// Attribute references
//
// Module:			Attribute Manager
// Namespace:		ADB
// Contact person:	JG
//
// SG compatible
// *********************************************************************************************************************

#if !defined (ADBGENERALDEFINITIONS_HPP)
#define ADBGENERALDEFINITIONS_HPP

#pragma once

// --- Includes	--------------------------------------------------------------------------------------------------------

// from ODB
#include "ODBReference.hpp"
#include "ODBRefList.hpp"

// --- Predeclarations	------------------------------------------------------------------------------------------------
namespace ADB {

class GeneralAttribute;

typedef ODB::Ref<GeneralAttribute>								GeneralAttributeRef;
typedef ODB::ConstRef<GeneralAttribute>							GeneralAttributeConstRef;

typedef ODB::RefList<ADB::GeneralAttribute>						GeneralAttributeRefList;
typedef ODB::ConstRefList<ADB::GeneralAttribute>				GeneralAttributeConstRefList;

//--------------------------------------------------------------------

class AttributeContainer;

typedef ODB::Ref<AttributeContainer>							AttributeContainerRef;
typedef ODB::ConstRef<AttributeContainer>						AttributeContainerConstRef;

typedef ODB::RefList<ADB::AttributeContainer>					AttributeContainerList;
typedef ODB::ConstRefList<ADB::AttributeContainer>				AttributeContainerConstList;

//--------------------------------------------------------------------

class AttributeSet;

typedef ODB::Ref<AttributeSet>									AttributeSetRef;
typedef ODB::ConstRef<AttributeSet>								AttributeSetConstRef;

//--------------------------------------------------------------------

class IUsedAttrCollection;
class IUsedPenCollection;

//--------------------------------------------------------------------

class IAttributeSelTable;

//--------------------------------------------------------------------

class IAttributeNewIndexTable;

//--------------------------------------------------------------------

class AttributeIndex;

//--------------------------------------------------------------------

typedef short	IDAttribute;

}		// namespace ADB

#endif
