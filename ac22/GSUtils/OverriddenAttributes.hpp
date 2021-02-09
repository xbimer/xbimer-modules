// *********************************************************************************************************************
// Description:		OverriddenAttribute and descendant classes
//
// Module:			GSUtils
// Namespace:		GSUtil
// Contact person:	FLA, KV
//
// SG compatible
// *********************************************************************************************************************

#if !defined (OVERRIDDENATTRIBUTES_HPP)
#define OVERRIDDENATTRIBUTES_HPP

#pragma once


// --- Includes	--------------------------------------------------------------------------------------------------------
#include "GSUtilsExport.h"
#include "ExtendedPen.hpp"
#include "GSUtilsDefs.h"
#include "XMLChannel.hpp"

// --- Predeclarations	------------------------------------------------------------------------------------------------

// --- Constant definitions	--------------------------------------------------------------------------------------------

#define		XMLTag_OverriddenAttribute			"OverriddenAttribute"
#define		XMLTag_Index						"Index"
#define		XMLTag_Overridden					"Overridden"

// --- Type definitions	------------------------------------------------------------------------------------------------

namespace VBAttr {

	enum OverrideMode {
		UseThisAttribute		= 1,
		DoNotUseThisAttribute	= 0
	};

	enum AttributeCompareMode
	{
		OnlyOverride			= 0,
		OnlyIndex				= 1,
		OverrideAndIndex		= 2
	};

	class GSUTILS_DLL_EXPORT_CPP OverriddenAttributePrivate {
	public:
		static GS::ClassInfo	classInfo;
	};

	template <class AttributeType = short>
	class OverriddenAttribute
	{
	public:
		OverriddenAttribute ();
		OverriddenAttribute (const OverriddenAttribute& source);
		OverriddenAttribute (OverrideMode overrideMode, AttributeType attributeIndex);
		virtual ~OverriddenAttribute ();

		void			Set				(OverrideMode overrideMode, AttributeType attributeIndex);
		void			Set				(const OverriddenAttribute& source, AttributeCompareMode mode);

		void			SetOverride		(bool attributeOverride);
		void			SetIndex		(AttributeType attributeIndex);

		bool			GetOverride		() const;
		AttributeType	GetIndex		() const;

		bool			IsEqual			(const OverriddenAttribute& source, AttributeCompareMode mode) const;

		bool			operator==		(const OverriddenAttribute& source) const;
		bool			operator!=		(const OverriddenAttribute& source) const;

		GSErrCode		Read (GS::IChannel& ic);
		GSErrCode		Write (GS::OChannel& oc) const;
		GSErrCode		QuickRead (GS::IChannel& ic);
		GSErrCode		QuickWrite (GS::OChannel& oc) const;

		GSErrCode		ReadXML (GS::XMLIChannel& ic, const char* tag);
		GSErrCode		WriteXML (GS::XMLOChannel& oc, const char* tag) const;

		GSErrCode		ReadXML (GS::XMLIChannel& ic);
		GSErrCode		WriteXML (GS::XMLOChannel& oc) const;


	protected:
		GSErrCode		ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
		GSErrCode		WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;

		bool			overridden;
		AttributeType	index;
	};


	template <class AttributeType>
	OverriddenAttribute<AttributeType>::OverriddenAttribute ()
	{
		Set (DoNotUseThisAttribute, AttributeType ());
	}

	template <class AttributeType>
	OverriddenAttribute<AttributeType>::OverriddenAttribute (const OverriddenAttribute<AttributeType>& source)
	{
		Set (source.overridden ? UseThisAttribute : DoNotUseThisAttribute, source.index);
	}

	template <class AttributeType>
	OverriddenAttribute<AttributeType>::OverriddenAttribute (OverrideMode overrideMode, AttributeType attributeIndex)
	{
		Set (overrideMode, attributeIndex);
	}

	template <class AttributeType>
	OverriddenAttribute<AttributeType>::~OverriddenAttribute ()
	{

	}

	template <class AttributeType>
	void OverriddenAttribute<AttributeType>::Set (OverrideMode overrideMode, AttributeType attributeIndex)
	{
		SetOverride (overrideMode == UseThisAttribute);
		SetIndex (attributeIndex);
	}

	template <class AttributeType>
	void OverriddenAttribute<AttributeType>::Set (const OverriddenAttribute<AttributeType>& source, AttributeCompareMode mode)
	{
		switch (mode) {
		case OnlyOverride:
			overridden = source.overridden;
			break;
		case OnlyIndex:
			index = source.index;
			break;
		case OverrideAndIndex:
			overridden = source.overridden;
			index = source.index;
			break;
		default:
			DBBREAK_STR ("Invalid Compare Mode.");
			break;
		}
	}

	template <class AttributeType>
	void OverriddenAttribute<AttributeType>::SetOverride (bool attributeOverride)
	{
		overridden = attributeOverride;
	}

	template <class AttributeType>
	void OverriddenAttribute<AttributeType>::SetIndex (AttributeType attributeIndex)
	{
		index = attributeIndex;
	}

	template <class AttributeType>
	bool OverriddenAttribute<AttributeType>::GetOverride () const
	{
		return overridden;
	}

	template <class AttributeType>
	AttributeType OverriddenAttribute<AttributeType>::GetIndex () const
	{
		return index;
	}

	template <class AttributeType>
	bool OverriddenAttribute<AttributeType>::IsEqual (const OverriddenAttribute<AttributeType>& source, AttributeCompareMode mode) const
	{
		switch (mode) {
		case OnlyOverride:
			return overridden == source.overridden;
			break;
		case OnlyIndex:
			return index == source.index;
			break;
		case OverrideAndIndex:
			return overridden == source.overridden && index == source.index;
			break;
		default:
			DBBREAK_STR ("Invalid Compare Mode.");
			break;
		}

		return false;
	}

	template <class AttributeType>
	bool OverriddenAttribute<AttributeType>::operator== (const OverriddenAttribute<AttributeType>& source) const
	{
		return overridden == source.overridden && index == source.index;
	}

	template <class AttributeType>
	bool OverriddenAttribute<AttributeType>::operator!= (const OverriddenAttribute<AttributeType>& source) const
	{
		return overridden != source.overridden || index != source.index;
	}

	template <class AttributeType>
	GSErrCode OverriddenAttribute<AttributeType>::Read (GS::IChannel& ic)
	{
		GS::InputFrame frame (ic, OverriddenAttributePrivate::classInfo);
		if (frame.GetStatus () != NoError) {
			return frame.GetStatus ();
		}

		GSErrCode err = NoError;
		switch (frame.GetMainVersion ()) {
		case 1: err = ReadVersion1 (ic, frame); break;
		default:
			DBPRINTF ("VBAttr::OverriddenAttribute::Read: Unsupported object version\n");
			break;
		}
		return err;
	}

	template <class AttributeType>
	GSErrCode OverriddenAttribute<AttributeType>::Write (GS::OChannel& oc) const
	{
		GS::OutputFrame frame (oc, OverriddenAttributePrivate::classInfo);
		if (oc.GetOutputStatus () != NoError) {
			return oc.GetOutputStatus ();
		}

		GSErrCode err = NoError;
		switch (frame.GetMainVersion ()) {
		case 1: err = WriteVersion1 (oc, frame); break;
		default:
			DBPRINTF ("VBAttr::OverriddenAttribute::Write: Unsupported object version\n");
			break;
		}
		return err;
	}

	template <class AttributeType>
	GSErrCode OverriddenAttribute<AttributeType>::QuickRead (GS::IChannel& ic)
	{
		DBASSERT_STR (OverriddenAttributePrivate::classInfo.GetVersion ().GetMainVersion () == 1, "Must update OverriddenAttribute::QuickRead on change!");
		
		GSErrCode err = NoError;

		err = ic.Read (overridden);
		if (err != NoError) {
			return err;
		}

		err = GS::Read (ic, index);
		if (err != NoError) {
			return err;
		}

		return err;
	}

	template <class AttributeType>
	GSErrCode OverriddenAttribute<AttributeType>::QuickWrite (GS::OChannel& oc) const
	{
		DBASSERT_STR (OverriddenAttributePrivate::classInfo.GetVersion ().GetMainVersion () == 1, "Must update OverriddenAttribute::QuickWrite on change!");
		
		GSErrCode err = NoError;

		err = oc.Write (overridden);
		if (err != NoError) {
			return err;
		}

		err = GS::Write (oc, index);
		if (err != NoError) {
			return err;
		}

		return err;
	}

	template <class AttributeType>
	GSErrCode	OverriddenAttribute<AttributeType>::ReadXML (GS::XMLIChannel& ic, const char* tagStr)
	{
		GS::XMLITag tag (ic, tagStr);

		GSErrCode err = GS::ReadXML (ic, XMLTag_Index, index);
		if (err == NoError)
			err = GS::ReadXML (ic, XMLTag_Overridden, overridden);

		return err;
	}

	template <class AttributeType>
	GSErrCode	OverriddenAttribute<AttributeType>::WriteXML (GS::XMLOChannel& oc, const char* tagStr) const
	{
		GS::XMLTag tag (oc, tagStr);

		GSErrCode err = GS::WriteXML (oc, XMLTag_Index, index);
		if (err == NoError)
			err = GS::WriteXML (oc, XMLTag_Overridden, overridden);

		return err;
	}

	template <class AttributeType>
	GSErrCode	OverriddenAttribute<AttributeType>::ReadXML (GS::XMLIChannel& ic)
	{
		return ReadXML (ic, XMLTag_OverriddenAttribute);
	}

	template <class AttributeType>
	GSErrCode	OverriddenAttribute<AttributeType>::WriteXML (GS::XMLOChannel& oc) const
	{
		return WriteXML (oc, XMLTag_OverriddenAttribute);
	}

	template <class AttributeType>
	GSErrCode OverriddenAttribute<AttributeType>::ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& /*frame*/)
	{
		GSErrCode err = NoError;

		err = ic.Read (overridden);
		if (err != NoError) {
			return err;
		}

		err = GS::Read (ic, index);
		if (err != NoError) {
			return err;
		}

		return err;
	}

	template <class AttributeType>
	GSErrCode OverriddenAttribute<AttributeType>::WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& /*frame*/) const
	{
		GSErrCode err = NoError;

		err = oc.Write (overridden);
		if (err != NoError) {
			return err;
		}

		err = GS::Write (oc, index);
		if (err != NoError) {
			return err;
		}

		return err;
	}

	// === Overridden classes ====================================================

	class GSUTILS_DLL_EXPORT_CPP OverriddenPen : public OverriddenAttribute<short>
	{
	public:
		OverriddenPen ();
		OverriddenPen (const OverriddenPen& source);
		OverriddenPen (OverrideMode overrideMode, short attributeIndex);
		virtual ~OverriddenPen ();
	};

	class GSUTILS_DLL_EXPORT_CPP OverriddenExtendedPen : public OverriddenAttribute<ExtendedPen>
	{
	public:
		OverriddenExtendedPen ();
		explicit OverriddenExtendedPen (const OverriddenPen& source);
		OverriddenExtendedPen (const OverriddenExtendedPen& source);
		OverriddenExtendedPen (OverrideMode overrideMode, ExtendedPen attribute);
		virtual ~OverriddenExtendedPen ();
	};

	class GSUTILS_DLL_EXPORT_CPP OverriddenMaterial : public OverriddenAttribute<short>
	{
	public:
		static const OverriddenMaterial InvalidOverriddenMaterial;

		OverriddenMaterial ();
		OverriddenMaterial (const OverriddenMaterial& source);
		OverriddenMaterial (OverrideMode overrideMode, short attributeIndex);
		virtual ~OverriddenMaterial ();
	};

	class GSUTILS_DLL_EXPORT_CPP OverriddenLine : public OverriddenAttribute<short>
	{
	public:
		OverriddenLine ();
		OverriddenLine (const OverriddenLine& source);
		OverriddenLine (OverrideMode overrideMode, short attributeIndex);
		virtual ~OverriddenLine ();
	};

	class GSUTILS_DLL_EXPORT_CPP OverriddenFill : public OverriddenAttribute<short>
	{
	public:
		OverriddenFill ();
		OverriddenFill (const OverriddenFill& source);
		OverriddenFill (OverrideMode overrideMode, short attributeIndex);
		virtual ~OverriddenFill ();
	};


	template <class AttributeType>
	bool IsEqualAsCombinedValue (const OverriddenAttribute<AttributeType>& a, const OverriddenAttribute<AttributeType>& b)
	{
		if (a.GetOverride () != b.GetOverride ()) {
			return false;
		}

		if (!a.GetOverride () && !b.GetOverride ()) {
			return true;
		}

		return a.GetIndex () == b.GetIndex ();
	}


	AttributeCompareMode GSUTILS_DLL_EXPORT_CPP		GetAttributeCompareMode (bool overrideChanged, bool indexChanged);
	bool GSUTILS_DLL_EXPORT_CPP						IsEqualOverriddenMaterials (const OverriddenMaterial& a, const OverriddenMaterial& b);

} // namespace

namespace GS {

	template <class AttributeType>
	GSErrCode Read (GS::IChannel& ic, VBAttr::OverriddenAttribute<AttributeType>& overriddenAttribute)
	{
		return overriddenAttribute.Read (ic);
	}

	template <class AttributeType>
	GSErrCode Write (GS::OChannel& oc, const VBAttr::OverriddenAttribute<AttributeType>& overriddenAttribute)
	{
		return overriddenAttribute.Write (oc);
	}

} // namespace

#endif