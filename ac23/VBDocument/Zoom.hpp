
// *****************************************************************************
//
//	                            Class Zoom
//
// Module:			VBDocument
// Namespace:		-
// Contact person:	VaLa
//
// SG compatible
//
// *****************************************************************************


#if !defined VBD_ZOOM_HPP
#define VBD_ZOOM_HPP

#include "VBDocumentExport.hpp"
#include "TWObject.hpp"
#include "Box2DDataWithRotation.hpp"
#include "VBDViewSetting.hpp"

namespace VBD {
class VBDOCUMENT_DLL_EXPORT Zoom : public TW::Object,
								   public ViewSettingBase
{
	DECLARE_DYNAMIC_CLASS_INFO (Zoom)

private:
#define _ZOOM_DATA_MEMBERS_																								\
	GSTime			lastConnectionTime;																					\
	bool			hasZoomInfo;			/* indicates whether zoom is set */											\
	GS::UniString	zoomName;				/* Name of a saved zoom */													\
	Box2DDataWithRotation		zoom;		/* zoom */																	\
																														\
	bool			hasTransform;			/* indicates whether global transformation (view orientation) is set */		\
																														\
	bool			hasFitInWindow;																						\
	bool			fitInWindow;			/* indicates whether fit in window has to be used when opening the view */	\
																														\
	bool			hasIgnoreSavedZoomSet;																				\
	bool			ignoreSavedZoom;		/* if true, when the view is opened the saved zoom settings are not used */

	_ZOOM_DATA_MEMBERS_

private:
	class IsolatedState : public TW::Object::IsolatedState
	{
	private:
		friend class Zoom;

		_ZOOM_DATA_MEMBERS_

	public:
		virtual ~IsolatedState ();

	};

public:
	Zoom ();
	Zoom (const Zoom& source);
	Zoom (GS::ClassInfo* finalClassInfo);
	Zoom (Box2DDataWithRotation zoom);

	virtual ~Zoom ();

	Zoom*					Clone () const override;

	bool					HasZoomInfo (void) const;
	void					SetNamedZoom (const GS::UniString& zoomName);
	bool					HasNamedZoom (void) const;
	void					GetNamedZoom (GS::UniString* zoomName) const;

	void					SetZoom (const Box2DDataWithRotation& newZoom);
	void					ClearZoom (void);
	Box2DDataWithRotation	GetZoom (void) const;											// Stored zoom only

	void					ClearTransform (void);
	bool					HasTransform (void) const;
	void					SetTransform (const SL::Transformation2D& newTr);
	SL::Transformation2D	GetTransform (void) const;										// Stored transform only

	void		SetFitInWindow (bool fit);
	void		ClearFitInWindow (void);
	bool		HasFitInWindow (void) const;
	bool		GetFitInWindow (void) const;

	void		SetIgnoreSavedZoom (bool ignore);
	void		ClearIgnoreSavedZoom (void);
	bool		HasIgnoreSavedZoomSet (void) const;
	bool		GetIgnoreSavedZoom (void) const;

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
	void								SetStoredName (const GS::UniString& value);

private:
	// ... Link management .............................................................................................
public:
	void								SetLastConnectionTime (const GSTime& value);
	static ODB::Association11 <VBD::ViewSetting, Zoom>&		GetViewSettingAssociationID (void);
	const GSTime&						GetLastConnectionTime (void) const { return lastConnectionTime; }

	static Box2DDataWithRotation		GetAttribute (ODB::ConstRef<Zoom> zoom);

	virtual bool						HasAttribute (void) const override;
	void								SetAttribute (const Box2DDataWithRotation& zoom);

	//Also needs a constructor with Attribute class as the only argument
};

}	// namespace VBD

#endif