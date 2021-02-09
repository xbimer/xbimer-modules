// *********************************************************************************************************************
// Description:		View definition
//
// Module:			VBD
// Namespace:		VBD
// Contact person:	VaLa
//
// SG compatible
// *********************************************************************************************************************

#if !defined (__VBD_VIEW_HPP__)
#define __VBD_VIEW_HPP__

#pragma once

#include "VBDocumentExport.hpp"
#include "TWObject.hpp"
#include "Project.hpp"

namespace VBD {

class ViewSetting;
typedef ODB::Ref<ViewSetting>				ViewSettingRef;
typedef ODB::ConstRef<ViewSetting>			ViewSettingConstRef;
typedef ODB::RefList<ViewSetting>			ViewSettingRefList;
typedef ODB::ConstRefList<ViewSetting>		ViewSettingConstRefList;

class View;
typedef ODB::Ref<View>						ViewRef;
typedef ODB::ConstRef<View>					ViewConstRef;
typedef ODB::RefList<View>					ViewRefList;
typedef ODB::ConstRefList<View>				ViewConstRefList;


// === View class ======================================================================================================

// --- Declaration -----------------------------------------------------------------------------------------------------

class VBDOCUMENT_DLL_EXPORT View : public TW::Object
{
	DECLARE_DYNAMIC_CLASS_INFO (View)

	friend class MergedViewSettingUpdater;

#define _VIEW_DATA_MEMBERS_		\
private:							\
	GS::UniString		m_name;		\

	_VIEW_DATA_MEMBERS_

// ... Transaction management class ....................................................................................
protected:
	class IsolatedState : public TW::Object::IsolatedState {
	private:
		friend class View;

		_VIEW_DATA_MEMBERS_

	public:
		virtual ~IsolatedState ();
	};

// ... Constructors ....................................................................................................
private:
	View (const View& source);
public:
	View ();
	virtual ~View ();

// ... Creator .........................................................................................................
public:
	static ViewRef							Create		(const GS::UniString& name, EDB::ProjectRef project, bool ownedByProject, ViewSettingRef viewSetting = nullptr);


// ... operator= .......................................................................................................
private:
	View&									operator=							(const View& source);

// ... Overridden virtual methods (GS::Object) .........................................................................
public:
	virtual View*							Clone								(void) const override;

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

// ... Accessors .......................................................................................................
public:
	const GS::UniString&					GetName					(void) const	{ return m_name;}
	void									SetName					(const GS::UniString& value);

	void									SetViewSetting			(VBD::ViewSettingRef viewSetting);

	void									ConnectToProject		(EDB::ProjectRef project);
	void									ConnectToOwnerProject	(EDB::ProjectRef project);
	bool									IsOwnedByProject		(void) const;

	void									SetParentView			(VBD::ViewConstRef parent);
	VBD::ViewConstRef						GetParentView			(void) const;
	void									ClearParentView			();
	static	VBD::ViewConstRefList			GetChildViews			(VBD::ViewConstRef viewRef);
	static  VBD::ViewRefList				GetChildViews			(VBD::ViewRef viewRef);
	static	VBD::ViewSettingConstRef		GetViewSetting			(VBD::ViewConstRef viewRef);
	static	VBD::ViewSettingRef				GetViewSetting			(VBD::ViewRef viewRef);
	static  VBD::ViewSettingConstRef		GetOwnViewSetting		(VBD::ViewConstRef viewRef);
	static  VBD::ViewSettingRef				GetOwnViewSetting		(VBD::ViewRef viewRef);

	void									AddSourceView			(VBD::ViewRef view);
	void									DisconnectSourceViews	(void);
	void									DisconnectSourceView	(VBD::ViewRef view);
	VBD::ViewConstRefList					GetSourceViews			(void) const;
	VBD::ViewConstRef						GetViewFromSourceView	(void) const;
	static const ODB::Id&					GetSourceViewsAssocId	(void);
private:
	static	VBD::ViewSettingConstRef		GetMergedViewSetting	(VBD::ViewConstRef viewRef);
	static	VBD::ViewSettingRef				GetMergedViewSetting	(VBD::ViewRef viewRef);
	
public:
	static	EDB::ProjectConstRef			GetConnectedProject		(VBD::ViewConstRef viewRef);
	static	EDB::ProjectRef					GetConnectedProject		(VBD::ViewRef viewRef);
	static	EDB::ProjectConstRef			GetOwnerProject			(VBD::ViewConstRef viewRef);
	static	EDB::ProjectRef					GetOwnerProject			(VBD::ViewRef viewRef);
	static	VBD::ViewConstRef				GetFromViewSetting		(VBD::ViewSettingConstRef vsRef);
	static	VBD::ViewRef					GetFromViewSetting		(VBD::ViewSettingRef vsRef);
	
	static  void							MergeViewSetting		(VBD::ViewRef viewRef);

	bool									IsOwnViewSettingEmpty		(void) const;
	bool									IsOwnViewSettingConnected	(void) const;
	bool									HasViewSettingConnected     (void) const;

	static bool								IsViewConnected			(VBD::ViewSettingConstRef viewSetting);

private:
	bool									IsMergedViewSettingConnected	(void) const;

	static void								CalculateMergedViewSetting		(VBD::ViewRef viewRef);
	static void								DoCalculateMergedViewSetting	(VBD::ViewRef viewRef);
};

}		// namespace VBD


#endif /* __VBD_VIEW_HPP__ */
