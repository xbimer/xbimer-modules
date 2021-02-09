
// *****************************************************************************
//
//	                            Class StructureDisplay
//
// Module:			VBDocument
// Namespace:		-
// Contact person:	VaLa
//
// SG compatible
//
// *****************************************************************************


#if !defined STRUCTUREDISPLAY_HPP
#define STRUCTUREDISPLAY_HPP

#include "VBDocumentExport.hpp"
#include "TWObject.hpp"
#include "EffectiveComposite.hpp"

#include "VBDViewSetting.hpp"



namespace VBD {
class VBDOCUMENT_DLL_EXPORT StructureDisplay : public TW::Object,
											   public ViewSettingBase
{
	DECLARE_DYNAMIC_CLASS_INFO (StructureDisplay)

private:
#define _STRUCTUREDISPLAY_DATA_MEMBERS_					\
	GSTime			lastConnectionTime;					\
	EffectiveComposite::Structure	structureDisplay;
	

	_STRUCTUREDISPLAY_DATA_MEMBERS_

private:
	class IsolatedState : public TW::Object::IsolatedState
	{
	private:
		friend class StructureDisplay;

		_STRUCTUREDISPLAY_DATA_MEMBERS_

	public:
		virtual ~IsolatedState ();

	};

public:
	StructureDisplay ();
	StructureDisplay (const StructureDisplay& source);
	StructureDisplay (GS::ClassInfo* finalClassInfo);
	StructureDisplay (EffectiveComposite::Structure structureDisplay);

	virtual ~StructureDisplay ();

	StructureDisplay*					Clone () const override;

	void								SetStructureDisplay (EffectiveComposite::Structure structureDisplay);
	EffectiveComposite::Structure		GetStructureDisplay (void) const;

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
	static ODB::Association11 <VBD::ViewSetting, StructureDisplay>&		GetViewSettingAssociationID (void);

	static EffectiveComposite::Structure	GetAttribute (ODB::Ref<StructureDisplay> structureDisplay);
	static EffectiveComposite::Structure	GetAttribute (ODB::ConstRef<StructureDisplay> structureDisplay);

	void								SetLastConnectionTime (const GSTime& value);
	const GSTime&						GetLastConnectionTime (void) const { return lastConnectionTime; }
	virtual bool						HasAttribute (void) const override;
	void								SetAttribute (EffectiveComposite::Structure structureDisplay);

};


}
#endif
