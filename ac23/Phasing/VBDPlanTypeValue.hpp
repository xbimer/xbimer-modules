// *********************************************************************************************************************
// Description:		VBDPlanTypeValue definition
//
// Module:			VBDocument
// Namespace:		VBD
// Contact person:	ZM
//
// SG compatible
// *********************************************************************************************************************

#if !defined (__VBD_VBDPLANTYPEVALUE_HPP__)
#define __VBD_VBDPLANTYPEVALUE_HPP__

#pragma once
#include "PhasingExport.hpp"
#include "TWObject.hpp"
#include "PlanTypeValue.hpp"
#include "VBDViewSetting.hpp"


namespace Phasing {
	class PlanTypeValue;
	typedef ODB::ConstRef<PlanTypeValue>	PlanTypeValueConstRef;
	typedef ODB::Ref<PlanTypeValue>			PlanTypeValueRef;
}

namespace VBD {

class PHASING_DLL_EXPORT PlanTypeValue : public TW::Object,
										 public ViewSettingBase
{
	DECLARE_DYNAMIC_CLASS_INFO (PlanTypeValue)

#define _VBDPLANTYPEVALUE_DATA_MEMBERS_		\
private:									\
	GSTime		lastAttributeTime;			\
	GS::UniString	storedName;				\

	_VBDPLANTYPEVALUE_DATA_MEMBERS_

// ... Transaction management class ....................................................................................
protected:
	class IsolatedState : public TW::Object::IsolatedState {
	private:
		friend class PlanTypeValue;

		_VBDPLANTYPEVALUE_DATA_MEMBERS_

	public:
		virtual ~IsolatedState ();
	};

// ... Constructors ....................................................................................................
public:
	PlanTypeValue ();
	PlanTypeValue (Phasing::PlanTypeValueRef phasingPlanTypeValue);
	PlanTypeValue (const PlanTypeValue& source);
	virtual ~PlanTypeValue ();

public:
	static ODB::Ref<VBD::PlanTypeValue> CreateVBDAttributeWithMissingAttribute (const GS::UniString& value);

private:
	void									InitOwn								(void);
	void									ResetOwn							(void);

// ... operator= .......................................................................................................
public:
	PlanTypeValue&						operator=							(const PlanTypeValue& source) = delete;

// ... Overridden virtual methods (GS::Object) .........................................................................
public:
	virtual PlanTypeValue*				Clone								(void) const override;

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
	GSErrCode								ReadCoreIsolatedStateVersion1		(GS::IChannel& ic, const GS::InputFrame& /*iFrame*/);
	GSErrCode								WriteCoreIsolatedStateVersion1		(GS::OChannel& oc, const GS::OutputFrame& /*oFrame*/) const;
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
	virtual GSErrCode						ReadXML								(GS::XMLIChannel& ix) override;
	virtual GSErrCode						WriteXML							(GS::XMLOChannel& ox) const override;

	// ... Accessors .......................................................................................................
private:
	void									SetStoredName (const GS::UniString& value);

private:
	bool									HasPlanTypeValue (void) const;

	static Phasing::PlanTypeValueConstRef	GetPlanTypeValue (ODB::ConstRef<PlanTypeValue> planTypeValue);
	static Phasing::PlanTypeValueRef		GetPlanTypeValue (ODB::Ref<PlanTypeValue> planTypeValue);

	// ... Link management .............................................................................................
public:
	static ODB::Association11 <VBD::ViewSetting, PlanTypeValue>&		GetViewSettingAssociationID (void);
	GS::UniString						GetStoredName (void) const { return storedName; }
	const GSTime&						GetLastConnectionTime (void) const { return lastAttributeTime; }
	void								SetLastConnectionTime (const GSTime& value);

	static Phasing::PlanTypeValueConstRef	GetAttribute (ODB::ConstRef<PlanTypeValue> planTypeValue);
	static Phasing::PlanTypeValueRef		GetAttribute (ODB::Ref<PlanTypeValue> planTypeValue);

	virtual bool						HasAttribute (void) const override;
	void								SetAttribute (Phasing::PlanTypeValueRef attr);

	//Also needs a constructor with Attribute class as the only argument
};


}		// namespace VBD


#endif /* __VBD_VBDPLANTYPEVALUE_HPP__ */
