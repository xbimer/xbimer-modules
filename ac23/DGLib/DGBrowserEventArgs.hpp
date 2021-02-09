// *********************************************************************************************************************
// File:			DGBrowserEventArgs.hpp
//
// Description:		Browser Control Classes
//
// Project:			GRAPHISOFT Dialog Manager (DGLib)
//
// Contact person:	VM
// *********************************************************************************************************************

#ifndef DGBROWSEREVENTARGS_HPP
#define DGBROWSEREVENTARGS_HPP

// --- Includes --------------------------------------------------------------------------------------------------------

#include "DG.h"
#include "DGDefs.h"
#include "DGEventArgs.hpp"
#include "DGUtility.hpp"

#include "DGBrowserJSHandler.hpp"

// --- Class declarations ----------------------------------------------------------------------------------------------

namespace DG {

// --- BrowserURLChangedArg --------------------------------------------------------------------------------------------

class DG_DLL_EXPORT BrowserURLChangedArg: public DGEventArg
{
public:
	GS::UniString newURL;

	BrowserURLChangedArg (const GS::UniString& _newURL): newURL (_newURL) { }

	virtual ~BrowserURLChangedArg ();
};


// --- BrowserPageSourceArg --------------------------------------------------------------------------------------------

class DG_DLL_EXPORT BrowserPageSourceArg: public DGEventArg
{
public:
	GS::UniString pageSource;
	GS::UniString url;

	BrowserPageSourceArg (const GS::UniString& _pageSource, const GS::UniString& _url): pageSource (_pageSource), url (_url) { }

	virtual ~BrowserPageSourceArg ();
};


// --- BrowserLoadingStateChangeArg --------------------------------------------------------------------------------------------

class DG_DLL_EXPORT BrowserLoadingStateChangeArg: public DGEventArg
	{
	public:
		bool			isLoading;
		bool			canGoBack;
		bool			canGoForward;

		BrowserLoadingStateChangeArg (bool _isLoading, bool _canGoBack, bool _canGoForward):
		isLoading (_isLoading),
		canGoBack (_canGoBack),
		canGoForward (_canGoForward)
		{
		};

		virtual ~BrowserLoadingStateChangeArg ();
	};



// --- BrowserBeforeDownloadArg ----------------------------------------------------------------------------------------

class DG_DLL_EXPORT BrowserBeforeDownloadArg: public DGEventArg
{
public:
	Int32			id;
	GS::UniString	url;
	GS::UniString	suggestedName;
	GS::UniString	mimeType;
	bool			showDefDlg;

public:
	BrowserBeforeDownloadArg () :
	id (0),
	url (),
	suggestedName (),
	mimeType (),
	showDefDlg (true)
	{
	};

	BrowserBeforeDownloadArg (int _id, GS::UniString _url, GS::UniString _suggestedName, GS::UniString _mimeType, bool _showDefDlg) :
	id (_id),
	url (_url),
	suggestedName (_suggestedName),
	mimeType (_mimeType),
	showDefDlg (_showDefDlg)
	{
	};

	virtual ~BrowserBeforeDownloadArg ();
};


// --- BrowserDownloadingArg -------------------------------------------------------------------------------------------

class DG_DLL_EXPORT BrowserDownloadingArg: public DGEventArg
{
public:
	int				id;
	GS::UniString	url;
	GS::UniString	fullPath;
	int64_t			currentSpeed;
	int64_t			totalBytes;
	int				percentComplete;
	bool			isInProgress;
	bool			isComplete;
	bool			isCanceled;

public:
	BrowserDownloadingArg () :
		id (0),
		url (),
		fullPath (),
		currentSpeed (0),
		totalBytes (0),
		percentComplete (0),
		isInProgress (false),
		isComplete (false),
		isCanceled (false)
	{
	};

	BrowserDownloadingArg (int _id, GS::UniString _url, GS::UniString _fullPath, int64_t _currentSpeed, int64_t _totalBytes, int _percentComplete, bool _isInProgress, bool _isComplete, bool _isCanceled) :
		id (_id),
		url (_url),
		fullPath (_fullPath),
		currentSpeed (_currentSpeed),
		totalBytes (_totalBytes),
		percentComplete (_percentComplete),
		isInProgress (_isInProgress),
		isComplete (_isComplete),
		isCanceled (_isCanceled)
	{
	};

	virtual ~BrowserDownloadingArg ();
};


// --- BrowserFindArg ------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT BrowserFinalFindArg: public DGEventArg
{
public:
	int 			count;
	int 			activeMatchOrdinal;

public:
	BrowserFinalFindArg (int inCount, int ordinal) :
		count (inCount),
		activeMatchOrdinal (ordinal)
	{
	};

	virtual ~BrowserFinalFindArg ();
};


// --- KeyboardEventArg ------------------------------------------------------------------------------------------------

class DG_DLL_EXPORT KeyboardEventArg: public DGEventArg
{
public:
	enum EventType {
		KeyUp	= 0,
		KeyDown	= 1
	};

private:
	EventType eventType;
	DG::Key::Code code;
	DG::Key::Modifier modifier;

public:
	KeyboardEventArg (const EventType eventType, const DG::Key::Code code, const DG::Key::Modifier modifier):
		eventType (eventType),
		code (code),
		modifier (modifier)
	{
	};

	virtual ~KeyboardEventArg ();

	EventType			GetEventType () const	{ return eventType; }
	DG::Key::Code		GetCode () const		{ return code; }
	DG::Key::Modifier	GetModifier () const	{ return modifier; }
};


class DG_DLL_EXPORT HeightChangedArg : public DGEventArg
{
private:
	Int32	pixelHeight = 0;

public:
	HeightChangedArg (Int32 pixelH) : pixelHeight (pixelH) {};

	virtual ~HeightChangedArg ();
	Int32	GetContentHeight () const { return pixelHeight; }
};


// --- BrowserNotifyHandler --------------------------------------------------------------------------------------------

class BrowserNotifyHandler
{
public:
	virtual ~BrowserNotifyHandler ();

	virtual void	URLChanged (const BrowserURLChangedArg& /*eventArg*/) = 0;
	virtual bool	URLWillChange (const BrowserURLChangedArg& /*eventArg*/) = 0;
	virtual void	OnPageSourceVisited (const BrowserPageSourceArg& /*eventArg*/) = 0;
	virtual void	OnLoadingStateChange (const BrowserLoadingStateChangeArg& /*eventArg*/) = 0;
	virtual bool	BeforeDownload (const BrowserBeforeDownloadArg&  /*eventArg*/) = 0;
	virtual bool	OnDownloading (const BrowserDownloadingArg&  /*eventArg*/) = 0;
	virtual void	OnFinalFind (const BrowserFinalFindArg& /*eventArg*/) = 0;
	virtual bool	KeyboardEventOccured (const KeyboardEventArg& /*eventArg*/) = 0;
	virtual void	ContentHeightChanged (const HeightChangedArg& pixelHeight) = 0;
};

}	// namespace DG

#endif
