// *********************************************************************************************************************
// Field mask for observer classes
//
// Module:			VBUtils
// Namespace:		-
// Contact person:	KG
//
// [SG compatible]
// *********************************************************************************************************************

#if !defined (FIELD_GROUP_HPP_)
#define FIELD_GROUP_HPP_

#pragma once


// --- Includes	--------------------------------------------------------------------------------------------------------

// GSRoot
#include "Definitions.hpp"
#include "GSGuid.hpp"
#include "HashCommon.hpp"

// own
#include "VBUtilsExport.hpp"

// --- class Field -----------------------------------------------------------------------------------------------------

class VBUTILS_DLL_EXPORT FieldGroup
{
private:
	GS::Guid	m_id;

public:
	FieldGroup (const GS::Guid& id);

	bool		operator==		(const FieldGroup& rv) const;
	bool		operator!=		(const FieldGroup& rv) const;

    operator    GS::HashValue   () const;
};


VBUTILS_DLL_EXPORT extern const FieldGroup CoreFieldGroup;


#endif		/* FIELD_GROUP_HPP_ */
