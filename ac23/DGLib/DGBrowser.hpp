// *********************************************************************************************************************
// File:			DGBrowser.hpp
//
// Description:		Browser Control Classes
//
// Project:			GRAPHISOFT Dialog Manager (DGLib)
//
// Contact person:	VM
// *********************************************************************************************************************

#ifndef DGBROWSER_HPP
#define DGBROWSER_HPP


// --- Includes --------------------------------------------------------------------------------------------------------

#include "DGBrowserEventArgs.hpp"
#include "DGDefs.h"
#include "DGEventNotifier.hpp"
#include "DGItemProperty.hpp"
#include "DGPanel.hpp"


// --- Predeclarations -------------------------------------------------------------------------------------------------

namespace DG {
	class Browser;
	class BrowserImpl;
	class JSObject;
	class JSValue;
}

// --- Class declarations ----------------------------------------------------------------------------------------------

namespace DG {

// --- Browser ----------------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT BrowserBase: public BrowserNotifyHandler
{
protected:
	BrowserImpl* impl = nullptr;

	virtual void	URLChanged (const BrowserURLChangedArg& eventArg) override;
	virtual bool	URLWillChange (const BrowserURLChangedArg& eventArg) override;
	virtual void	OnPageSourceVisited (const BrowserPageSourceArg& eventArg) override;
	virtual void	OnLoadingStateChange (const BrowserLoadingStateChangeArg& eventArg) override;
	virtual bool	BeforeDownload (const BrowserBeforeDownloadArg& eventArg) override;
	virtual bool	OnDownloading (const BrowserDownloadingArg& eventArg) override;
	virtual void	OnFinalFind (const BrowserFinalFindArg& eventArg) override;
	virtual bool	KeyboardEventOccured (const KeyboardEventArg& eventArg) override;
	virtual void	ContentHeightChanged (const HeightChangedArg& eventArg) override;

	void			CreateBaseImpl (Item* parent = nullptr);
	GS::UniString	GetJSValueParamString (const DG::JSValue* jsValue);
	GS::UniString	GetJSArrayParamString (const DG::JSArray* jsArray, bool toplevelArray);

public:
	BrowserBase	();
	virtual ~BrowserBase ();

	void	SetUpForNativeWindow (void* window, const DG::Rect& rect);

	void	LoadURL (const GS::UniString& url);
	void	LoadHTML (const GS::UniString& htmlStr);
	void 	GoBack ();
	void 	GoForward ();
	void 	Reload ();
	void	ReloadIgnoreCache ();
	void	DeleteCookies (const GS::UniString& url, const GS::UniString& cookieName);
	void	DeleteAllCookies ();
	void 	Print ();
	void	DisableContextMenu ();
	void	EnableContextMenu ();
	void	SetContextMenuStatus (const bool status);
	void	SetScrollBarVisibility (const bool visible);
	void	DisableNavigation (const bool disabled);
	int		FindStart (int sessionId, const GS::UniString& txt, bool forward, bool matchCase);
	int		FindNext (int sessionId, bool forward);
	void	FindStop (int sessionId);
	bool	ExecuteJS (const GS::UniString& jsCode);
	bool	ExecuteJSFunctionWithParameters (GS::Ref<JSObject> onObject, GS::Ref<JSFunction> jsFunction, GS::Ref<JSBase> parameters);
	bool	RegisterJSObject (GS::Ref<DG::JSObject> obj);
	bool	RegisterAsynchJSObject (GS::Ref<DG::JSObject> obj);

	void	EnableContentHeightChangedEvent (bool enable);
	void*	GetNativeObjectRef ();

	// Eventhandlers
	DGEventNotifier<BrowserBase, BrowserURLChangedArg> urlChanged;
	DGEventNotifier<BrowserBase, BrowserURLChangedArg, bool, true> urlWillChange;
	DGEventNotifier<BrowserBase, BrowserPageSourceArg> onPageSourceVisited;
	DGEventNotifier<BrowserBase, BrowserLoadingStateChangeArg> onLoadingStateChange;
	DGEventNotifier<BrowserBase, BrowserBeforeDownloadArg, bool, true> beforeDownload;
	DGEventNotifier<BrowserBase, BrowserDownloadingArg, bool, true> onDownloading;
	DGEventNotifier<BrowserBase, BrowserFinalFindArg> onFinalFind;
	DGEventNotifier<BrowserBase, KeyboardEventArg, bool, false> keyboardEvent;
	DGEventNotifier<BrowserBase, HeightChangedArg> contentHeightChangedEvent;
};

class DG_DLL_EXPORT Browser: public Item, public BrowserBase {

private:
	void	CreateImpl ();

public:
	Browser	(const Rect& rect);

	Browser	(const Panel& panel, short item);
	Browser	(const Panel& panel, const Rect& rect);
	virtual ~Browser ();
};

class DG_DLL_EXPORT FreeBrowser: public BrowserBase {

public:
	FreeBrowser	();
	virtual ~FreeBrowser ();
};
}	// namespace DG

#endif
