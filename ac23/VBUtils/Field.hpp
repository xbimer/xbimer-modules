// *********************************************************************************************************************
// Field mask for observer classes
//
// Module:			VBUtils
// Namespace:		-
// Contact person:	KG
//
// [SG compatible]
// *********************************************************************************************************************

#if !defined (FIELD_HPP_)
#define FIELD_HPP_

#pragma once


// --- Includes	--------------------------------------------------------------------------------------------------------

// GSRoot
#include "Definitions.hpp"
#include "GSGuid.hpp"
#include "HashCommon.hpp"

// own
#include "VBUtilsExport.hpp"
#include "FieldGroup.hpp"

// --- class Field -----------------------------------------------------------------------------------------------------
/*
 * Field for a class, to be used with FieldMask. 
 *
 * Intended for static initialisation, do not instantiate run-time, not thread-safe.
 *
 */

class VBUTILS_DLL_EXPORT Field
{
private:
	GS::Guid	m_id;

public:
	Field (const GS::Guid& id, const GS::ClassInfo* classInfo, const FieldGroup& group = CoreFieldGroup);
	~Field ();

	bool		operator==		(const Field& rv) const;
	bool		operator!=		(const Field& rv) const;

    operator    GS::HashValue   () const;

	FieldGroup	GetGroup		(void) const;
};

// --- class DynamicField ----------------------------------------------------------------------------------------------
/*
 * Dynamic field for a class, to be used with FieldMask. 
 *
 * If your class that you want to use with FieldMask has fields that can be determined only per instance, or run-time.
 *
 */

class VBUTILS_DLL_EXPORT DynamicField
{
private:
	GS::Guid	m_id;
	FieldGroup	m_group;

public:
	explicit DynamicField (const GS::Guid& id, const FieldGroup& group = CoreFieldGroup);
	~DynamicField ();

	bool		operator==		(const DynamicField& rv) const;
	bool		operator!=		(const DynamicField& rv) const;

    operator    GS::HashValue   () const;

	FieldGroup	GetGroup		(void) const;
};

#endif		/* FIELD_HPP_ */
