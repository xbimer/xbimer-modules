// *********************************************************************************************************************
// Field mask for observer classes
//
// Module:			VBUtils
// Namespace:		-
// Contact person:	KG
//
// [SG compatible]
// *********************************************************************************************************************

#if !defined (SUB_FIELD_MASK_ID_HPP_)
#define SUB_FIELD_MASK_ID_HPP_

#pragma once


// --- Includes	--------------------------------------------------------------------------------------------------------

// GSRoot
#include "Definitions.hpp"
#include "GSGuid.hpp"
#include "HashCommon.hpp"

// own
#include "VBUtilsExport.hpp"


// --- class SubFieldMaskId --------------------------------------------------------------------------------------------
/*
 * Sub-fieldmask id for a class, to be used with FieldMask. 
 *
 * Intended for static initialisation, do not instantiate run-time, not thread-safe.
 *
 */

class VBUTILS_DLL_EXPORT SubFieldMaskId
{
private:
	GS::Guid	m_id;

public:
	SubFieldMaskId ();
	SubFieldMaskId (const GS::Guid& id, const GS::ClassInfo* parentClassInfo, const GS::ClassInfo* subObjectClassInfo);

	bool		operator==		(const SubFieldMaskId& rv) const;
	bool		operator!=		(const SubFieldMaskId& rv) const;
    operator    GS::HashValue   () const;
};



// --- class DynamicSubFieldMaskId --------------------------------------------------------------------------------------------
/*
 * Dynamic sub-fieldmask id for a class, to be used with FieldMask. 
 *
 * If your class that you want to use with FieldMask has sub-masks that can be determined only per instance, or run-time.
 *
 */

class VBUTILS_DLL_EXPORT DynamicSubFieldMaskId
{
private:
	GS::Guid					m_id;
	const GS::ClassInfo*		m_subObjectClassInfo;

public:
	DynamicSubFieldMaskId ();
	DynamicSubFieldMaskId (const GS::Guid& id, const GS::ClassInfo* subObjectClassInfo);

	bool						operator==				(const DynamicSubFieldMaskId& rv) const;
	bool						operator!=				(const DynamicSubFieldMaskId& rv) const;
    operator					GS::HashValue			() const;

	const GS::ClassInfo*		GetSubObjectClassInfo	(void) const;
};

#endif		/* SUB_FIELD_MASK_ID_HPP_ */
