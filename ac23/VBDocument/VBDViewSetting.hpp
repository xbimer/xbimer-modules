// *********************************************************************************************************************
// Description:		ViewSetting definition
//
// Module:			VBD
// Namespace:		VBD
// Contact person:	VaLa
//
// SG compatible
// *********************************************************************************************************************

#if !defined (__VBD_VIEWSETTING_HPP__)
#define __VBD_VIEWSETTING_HPP__

#pragma once

#include "VBDocumentExport.hpp"
#include "TWObject.hpp"
#include "ODBReference.hpp"
#include "ODBAssociation11.hpp"
#include "ODBRefList.hpp"

namespace SRM {
	struct LastViewModiTimes;
}

namespace ViewSettingAC20 {
	class ACViewSetting;
}

class VBView;

typedef ODB::Ref<VBView> VBViewRef;
typedef ODB::ConstRef<VBView> VBViewConstRef;


namespace VBD {

	enum AttributeType {
		NotSet,
		Standard,
		Custom
	};

class ViewSetting;


typedef ODB::Ref<ViewSetting>				ViewSettingRef;
typedef ODB::ConstRef<ViewSetting>			ViewSettingConstRef;

class View;

typedef ODB::Ref<View>						ViewRef;
typedef ODB::ConstRef<View>					ViewConstRef;

// === ViewSetting class ===============================================================================================

// --- Declaration -----------------------------------------------------------------------------------------------------

class VBDOCUMENT_DLL_EXPORT ViewSetting : public TW::Object
{
	DECLARE_DYNAMIC_CLASS_INFO (ViewSetting)

#define _VIEWSETTING_DATA_MEMBERS_		\
private:								\
Int32			lastAttributeTime;		// Number of nested time disable
	_VIEWSETTING_DATA_MEMBERS_

// ... Transaction management class ....................................................................................
protected:
	class IsolatedState : public TW::Object::IsolatedState {
	private:
		friend class ViewSetting;

		_VIEWSETTING_DATA_MEMBERS_

	public:
		virtual ~IsolatedState ();
	};

// ... Constructors ....................................................................................................
public:
	ViewSetting ();
	ViewSetting (const ViewSetting& source);
	virtual ~ViewSetting ();

// ... operator= .......................................................................................................
private:
	ViewSetting&							operator=							(const ViewSetting& source);

// ... Overridden virtual methods (GS::Object) .........................................................................
public:
	virtual ViewSetting*					Clone								(void) const override;

	void	CopyAttributesFrom (ODB::ConstRef<VBD::ViewSetting> source, bool overwrite = true);
	void	ClearAllAttributes (void);
	void	Merge (ODB::ConstRef<VBD::ViewSetting> source);
	bool	IsEmpty () const;
	// ... Transaction management methods ..................................................................................
public:
	virtual GSErrCode						StoreIsolatedState					(ODB::Object::IsolatedState* iState) const override;
	virtual GSErrCode						RestoreIsolatedState				(const ODB::Object::IsolatedState* iState) override;
	virtual void							Isolate								(void) override;
	virtual ODB::Object::IsolatedState*		CreateIsolatedState					(void) const override;
	virtual USize							GetIsolatedSize						(void) const override;
	virtual GSErrCode						ReadIsolatedState					(GS::IChannel& ic) override;
	virtual GSErrCode						WriteIsolatedState					(GS::OChannel& oc) const override;
private:
	GSErrCode								ReadIsolatedStateVersion1			(GS::IChannel& ic, const GS::InputFrame& iFrame);
	GSErrCode								WriteIsolatedStateVersion1			(GS::OChannel& oc, const GS::OutputFrame& oFrame) const;
	GSErrCode								ReadCoreIsolatedStateVersion1		(GS::IChannel& /*ic*/, const GS::InputFrame& /*iFrame*/);
	GSErrCode								WriteCoreIsolatedStateVersion1		(GS::OChannel& /*oc*/, const GS::OutputFrame& /*oFrame*/) const;
public:
	virtual GSErrCode						ReadTWIsolatedState					(GS::IChannel& ic) override;
	virtual GSErrCode						WriteTWIsolatedState				(GS::OChannel& oc) const override;
private:
	GSErrCode								ReadTWIsolatedStateVersionLast		(GS::IChannel& ic, const GS::InputFrame& iFrame);
	GSErrCode								WriteTWIsolatedStateVersionLast		(GS::OChannel& oc, const GS::OutputFrame& oFrame) const;

// ... XML serialization ...............................................................................................
public:
	virtual GSErrCode						ReadIsolatedStateXML				(GS::XMLIChannel& ix) override;
	virtual GSErrCode						WriteIsolatedStateXML				(GS::XMLOChannel& ox) const override;

// ... Attribute management methods ....................................................................................
public:
	template<class VBDAttributeClass>
	bool	HasVBDAttribute (void) const
	{
		return VBDAttributeClass::GetViewSettingAssociationID ().IsConnectedAsLeft (this);
	}

public:
	template<class VBDAttributeClass>
	static bool	HasAttribute (ODB::ConstRef<ViewSetting> viewSetting)
	{
		if (viewSetting == nullptr) {
			return false;
		} else {
			return viewSetting->HasAttribute<VBDAttributeClass> ();
		}
	}

public:
	template<class VBDAttributeClass>
	static bool	HasVBDAttribute (ODB::ConstRef<ViewSetting> viewSetting)
	{
		if (viewSetting == nullptr) {
			return false;
		} else {
			return viewSetting->HasVBDAttribute<VBDAttributeClass> ();
		}
	}

public:
	template<class VBDAttributeClass>
	static ODB::ConstRef<VBDAttributeClass> GetVBDAttribute (ODB::ConstRef<ViewSetting> viewSetting)
	{
		if (DBVERIFY (viewSetting != nullptr && viewSetting->HasVBDAttribute<VBDAttributeClass> ())) {
			return VBDAttributeClass::GetViewSettingAssociationID ().GetRight (viewSetting);
		} else {
			return nullptr;
		}
	}

public:
	template<class VBDAttributeClass>
	static ODB::Ref<VBDAttributeClass> GetVBDAttribute (ODB::Ref<ViewSetting> viewSetting)
	{
		if (DBVERIFY (viewSetting != nullptr && viewSetting->HasVBDAttribute<VBDAttributeClass> ())) {
			return VBDAttributeClass::GetViewSettingAssociationID ().GetRight (viewSetting);
		} else {
			return nullptr;
		}
	}

public:
	template<class VBDAttributeClass>
	static auto	GetAttribute (ODB::ConstRef<ViewSetting> viewSetting)
	{
		return VBDAttributeClass::GetAttribute (GetVBDAttribute<VBDAttributeClass> (viewSetting));
	}
	
public:
	template<class VBDAttributeClass>
	static auto	GetAttribute (ODB::Ref<ViewSetting> viewSetting)
	{
		return VBDAttributeClass::GetAttribute (GetVBDAttribute<VBDAttributeClass> (viewSetting));
	}

public:
	template<class VBDAttributeClass>
	void SetVBDAttribute (ODB::Ref<VBDAttributeClass> vbdAttribute)
	{
		DBVERIFY (vbdAttribute != nullptr);
		VBDAttributeClass::GetViewSettingAssociationID ().Reconnect (this, vbdAttribute);

		if (lastAttributeTime == 0) {
			vbdAttribute()->SetLastConnectionTime (TIGetTime ());
		}
	}

private:
	template<class VBDAttributeClass>
	void	SetNewVBDAttributeFromAttribute (decltype (VBDAttributeClass::GetAttribute (std::declval<ODB::Ref<VBDAttributeClass>> ())) attribute)
	{
		ODB::Ref<VBDAttributeClass> newVBDAttribute = new VBDAttributeClass (attribute);
		SetVBDAttribute<VBDAttributeClass> (newVBDAttribute);
	}

private:
	template<class VBDAttributeClass>
	void	SetNewVBDAttributeFromAttribute (decltype (VBDAttributeClass::GetAttribute (std::declval<ODB::Ref<VBDAttributeClass>> ())) attribute, AttributeType attributeType)
	{
		ODB::Ref<VBDAttributeClass> newVBDAttribute = new VBDAttributeClass (attribute, attributeType);
		SetVBDAttribute<VBDAttributeClass> (newVBDAttribute);
	}

public:
	template<class VBDAttributeClass>
	void	SetNewVBDAttribute (ODB::ConstRef<VBDAttributeClass> vbdAttribute)
	{
		DBVERIFY (vbdAttribute != nullptr);
		ODB::Ref<VBDAttributeClass> newVBDAttribute = vbdAttribute->Clone();
		SetVBDAttribute<VBDAttributeClass> (newVBDAttribute);
	}

public:
	template<class VBDAttributeClass>
	bool	HasAttribute (void) const
	{
		if (HasVBDAttribute<VBDAttributeClass> ()) {
			return GetVBDAttribute<VBDAttributeClass> (this)->HasAttribute ();
		} else {
			return false;
		}
	}


public:
	template<class VBDAttributeClass>
	void	SetAttribute (decltype (VBDAttributeClass::GetAttribute (std::declval<ODB::Ref<VBDAttributeClass>> ())) attribute)
	{
		if (HasVBDAttribute<VBDAttributeClass> ()) {
			GetVBDAttribute<VBDAttributeClass> (ODB::StaticCast<ViewSetting> (thisRef))()->SetAttribute (attribute);
		} else {
			SetNewVBDAttributeFromAttribute<VBDAttributeClass> (attribute);
		}
	}


	template<class VBDAttributeClass>
	void	SetAttribute (decltype (VBDAttributeClass::GetAttribute (std::declval<ODB::Ref<VBDAttributeClass>> ())) attribute, AttributeType attributeType)
	{
		if (HasVBDAttribute<VBDAttributeClass> ()) {
			GetVBDAttribute<VBDAttributeClass> (ODB::StaticCast<ViewSetting> (thisRef))()->SetAttribute (attribute, attributeType);
		} else {
			SetNewVBDAttributeFromAttribute<VBDAttributeClass> (attribute, attributeType);
		}
	}



public:
	template<class VBDAttributeClass>
	GS::UniString	GetAttributeName (void) const
	{
		if (DBVERIFY (HasVBDAttribute<VBDAttributeClass> ())) {
			if (HasAttribute<VBDAttributeClass> ()) {
				return GetAttribute<VBDAttributeClass> (this)->GetName ();
			} else {
				return GetVBDAttribute<VBDAttributeClass> (this)->GetStoredName ();
			}
		} else {
			return "???";
		}
	}

public:
	template<class VBDAttributeClass>
	void	AddNewVBDAttributeWithMissingAttribute (const GS::UniString& value)
	{
		if (DBVERIFY (!HasVBDAttribute<VBDAttributeClass> ())) { 
			ODB::Ref<VBDAttributeClass> vbdAttributeWithMissingAttribute = VBDAttributeClass::CreateVBDAttributeWithMissingAttribute (value);
			SetVBDAttribute<VBDAttributeClass> (vbdAttributeWithMissingAttribute);
		}
	}


public:
	template<class VBDAttributeClass>
	GSTime	GetAttributeLastConnectionTime (void) const
	{
		if (DBVERIFY (HasVBDAttribute<VBDAttributeClass> ())) {
			return GetVBDAttribute<VBDAttributeClass> (this)->GetLastConnectionTime ();
		} else {
			return GSTime ();
		}
	}

public:
	template<class VBDAttributeClass>
	void	SetAttributeLastConnectionTime (const GSTime& value)
	{
		GetVBDAttribute<VBDAttributeClass> (ODB::StaticCast<ViewSetting> (thisRef))()->SetLastConnectionTime (value);
	}

public:
	template<class VBDAttributeClass>
	void	ClearAttribute (void)
	{
		if (HasVBDAttribute<VBDAttributeClass> ())
			VBDAttributeClass::GetViewSettingAssociationID ().DisconnectAsLeft (this);
	}
public:
	template<class VBDAttributeClass>
	VBD::AttributeType	GetAttributeType (void) const
	{
		if (DBVERIFY (HasVBDAttribute<VBDAttributeClass> ()))
			return GetVBDAttribute<VBDAttributeClass> (this)->GetType ();
		else
			return VBD::AttributeType::NotSet;
	}
public:
	template<class VBDAttributeClass>
	static VBD::AttributeType	GetAttributeType (VBD::ViewSettingConstRef viewSetting)
	{
		return viewSetting->GetAttributeType<VBDAttributeClass> ();
	}
};

class VBDOCUMENT_DLL_EXPORT ViewSettingBase {
public:
	virtual ~ViewSettingBase ();
	virtual bool HasAttribute (void) const = 0;
};

}		// namespace VBD


#endif /* __VBD_VIEWSETTING_HPP__ */
