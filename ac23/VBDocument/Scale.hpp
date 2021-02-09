
// *****************************************************************************
//
//	                            Class Scale
//
// Module:			VBDocument
// Namespace:		-
// Contact person:	VaLa
//
// SG compatible
//
// *****************************************************************************


#if !defined SCALE_HPP
#define SCALE_HPP

#include "VBDocumentExport.hpp"
#include "TWObject.hpp"
#include "VBDViewSetting.hpp"

namespace VBD {
class VBDOCUMENT_DLL_EXPORT Scale : public TW::Object,
									public ViewSettingBase
{
	DECLARE_DYNAMIC_CLASS_INFO (Scale)

private:
#define _SCALE_DATA_MEMBERS_				\
	Int32			scale;					\
	GSTime			lastConnectionTime;		

	_SCALE_DATA_MEMBERS_

private:
	class IsolatedState : public TW::Object::IsolatedState
	{
	private:
		friend class Scale;

		_SCALE_DATA_MEMBERS_

	public:
		virtual ~IsolatedState ();

	};
private:
public:
	Scale ();
	Scale (const Scale& source);
	Scale (GS::ClassInfo* finalClassInfo);
	Scale (Int32 inScale);

	virtual ~Scale ();

	Scale*							Clone () const override;

	void								SetScale (Int32 inScale);
	Int32								GetScale (void) const;

public:
	virtual GSErrCode					StoreIsolatedState (ODB::Object::IsolatedState*		isolatedState) const override;
	virtual GSErrCode					RestoreIsolatedState (const ODB::Object::IsolatedState*	isolatedState) override;
	virtual	ODB::Object::IsolatedState*	CreateIsolatedState (void) const override;
	virtual USize						GetIsolatedSize (void) const override;
	virtual GSErrCode					WriteIsolatedState (GS::OChannel&						oc) const override;
	virtual GSErrCode					ReadIsolatedState (GS::IChannel&						ic) override;

	virtual	GSErrCode					WriteIsolatedStateXML (GS::XMLOChannel& oc) const override;
	virtual	GSErrCode					ReadIsolatedStateXML (GS::XMLIChannel& ic) override;

	virtual	GSErrCode					WriteXML (GS::XMLOChannel& oc) const override;
	virtual	GSErrCode					ReadXML (GS::XMLIChannel& ic) override;

	virtual GSErrCode					ReadTWIsolatedState (GS::IChannel& ic) override;
	virtual GSErrCode					WriteTWIsolatedState (GS::OChannel& oc) const override;
private:
	GSErrCode							ReadCoreIsolatedState (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode							WriteCoreIsolatedState (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	// ... Accessors .......................................................................................................
private:

	// ... Link management .............................................................................................
public:
	static ODB::Association11 <VBD::ViewSetting, Scale>&		GetViewSettingAssociationID (void);

	static Int32						GetAttribute (ODB::ConstRef<Scale> scale);

	void								SetLastConnectionTime (const GSTime& value);
	const GSTime&						GetLastConnectionTime (void) const { return lastConnectionTime; }
	virtual bool						HasAttribute (void) const override;
	void								SetAttribute (Int32 inScale);
};
}
#endif