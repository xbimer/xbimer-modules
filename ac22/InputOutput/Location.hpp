
// *****************************************************************************
//
//	                              Class Location
//
// Module: IO
// Namespace: IO
// Contact person: MB
//
// SG compatible
//
// *****************************************************************************


#if !defined LOCATION_HPP
#define LOCATION_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

#include "IOBase.hpp"

#include "Address.hpp"
#include "Name.hpp"
#include "RelativeLocation.hpp"
#include "UniString.hpp"
#include "CString.hpp"
#include "FastLogger.hpp"

// Lint is confused if a structure got new members in some cases (cant't handle inconsistent definitions)
#if defined (ACCESS_INPUTOUTPUT_MODULE_PRIVATE) || defined (INPUTOUTPUT_DLL_COMPILE) || defined (_lint)
#define INPUTOUTPUT_MODULE_PRIVATE
#endif


namespace IO { class LocationResolver; }

namespace IO { namespace URI { class URI; } }

#if defined(macintosh)
struct FSRef;                               // native Macintosh file descriptor
typedef const struct __CFURL * CFURLRef;    // native Macintosh file descriptor
#endif


// ============================== Class Location ===============================

namespace IO {

typedef CString<256> Path;	// for the full path representation
typedef CString<256> URL;	// for the URL representation

class IO_DLL_EXPORT Location: public GS::Object {
	DECLARE_CLASS_INFO
public:
	enum NetworkModel { Flat, Hierarchical, SMB, Null, Unknown, URL };	// possible network models

	class IO_DLL_EXPORT PDI: public GS::Object {
	public:
		virtual ~PDI ();

		PDI* Clone () const override = 0;
	};	// base class for platform dependent information

private:

	// !!! DON'T PLACE ANY FIELDS BEFORE networkAddress AND localAddress, BECAUSE DEBUG ADDON (GSDbgAddIn.dll) WON'T WORK !!!!
	Address				networkAddress;	// network domain of the location
	Address				localAddress;	// local domain of the location

	enum Legality { Legal, Illegal, Undetermined };		// legality of the Location

	NetworkModel		model;			// determines the way addresses are interpreted
	URI::URI*			uri;			// URI parameters
	PDI*				pdi;			// platform dependent information (can be nullptr)
	mutable Legality	legality;		// cached result of the IsLegal method
	GSErrCode			status;			// object state (error code saved in constructor)

#if defined (DEBUVERS)
	Int32*				leakDetector;
#endif

	friend class LocationResolver;	// resolvers are allowed to set content of a Location using the
									// corresponding Set method defined in class LocationResolver,
									// which in turn uses the following method
	void	SetContent (const GS::UniChar::Layout* networkNameSeq, const GS::UniChar::Layout* localNameSeq, NetworkModel nm);

		// resolvers can also query and set name attachments and platform dependent information
	GSErrCode	GetNetworkNameAttachment (UIndex idx, Name* result) const;
	GSErrCode	SetNetworkNameAttachment (UIndex idx, const Name& attachment);

	GSErrCode	GetLocalNameAttachment (UIndex idx, Name* result) const;
	GSErrCode	SetLocalNameAttachment (UIndex idx, const Name& attachment);

	void	CreateURI (void);
	void	ClearURI  (void);
	GSErrCode	ToURLFromURI	(GS::UniString*	result) const;
	bool	ResolveURL	(const GS::UniString& path, NetworkModel newModel);
	bool	IsURIHandledScheme (const GS::UniString& scheme) const;

	PDI*	GetPDI	 (void) const;
	void	SetPDI	 (PDI* newPDI);
	void	ClearPDI (void);

	static bool		IsURL      (const GS::UniString& path);
	bool			ResolveURL (const GS::UniString& path);

	inline static GS::UniChar::Layout	GetValue (GS::UniChar::Layout hexaDigit);

	static GS::UniString	ConvertNetworkModelToString (NetworkModel networkModel);
	static bool				ConvertStringToNetworkModel (const GS::UniString& string, NetworkModel* result);

	static GS::UniString&	GetScheme (const GS::UniString& path, GS::UniString* result);
	static bool				IsValidScheme (const GS::UniString& scheme);

	static bool		IsValidUserNameOrPassword (const GS::UniString& scheme);

	GSErrCode	ReadVersion12  (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode	WriteVersion12 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode	ReadVersion3  (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode	WriteVersion3 (GS::OChannel& oc, const GS::OutputFrame& frame) const;

	void		SwapPrimitiveMembers (Location& rightOp);

public:
		// Error codes

	enum {
		ResolveError	= LocationErrors + 0,	// location has not been properly initialized because address resolvation had been failed (possibly illegal address was passed)
		InvalidOperand	= LocationErrors + 1,	// operand of the operation is invalid
		NoScheme		= LocationErrors + 2	// location has not been properly initialized because no scheme was passed
	};

		// Constructors

	Location ();
	explicit Location (const GS::UniString& targetPath);	// Unicode representation of the target
	explicit Location (const char* targetPath);				// ASCII representation of the target

#if defined(macintosh)                                      // native Macintosh representations of the target
	enum UTF8POSIXPathSelector { UTF8POSIXPath };
#ifdef INPUTOUTPUT_MODULE_PRIVATE
	explicit Location (const FSRef& targetRef);
#endif
	explicit Location (const CFURLRef& targetRef);
	explicit Location (UTF8POSIXPathSelector selector, const char* path);
#endif
	Location (const Location& source);
	Location (Location&& source);
	Location (const Location& parent, const Name& name);

	Location&	operator= (const Location& source);
	Location&	operator= (Location&& source);

	~Location ();

	virtual	GS::Object*		Clone (void) const override;

	static GSErrCode	CCALL New (Location** result);
	static GSErrCode	CCALL New (Location** result, const GS::UniString& targetPath);
	static GSErrCode	CCALL New (Location** result, const char* targetPath);
#if defined(macintosh)
#ifdef INPUTOUTPUT_MODULE_PRIVATE
	static GSErrCode	CCALL New (Location** result, const FSRef& targetRef);                                        // DOC
#endif
#endif
	static GSErrCode	CCALL New (Location** result, const Location& source);
	static GSErrCode	CCALL New (Location** result, const Location& parent, const Name& name);

		// State query

	inline GSErrCode	GetStatus (void) const;

		// Global setting / query

	GSErrCode	Set	(const GS::UniString& targetPath);
	GSErrCode	Set	(const char* targetPath);
#if defined(macintosh)
#ifdef INPUTOUTPUT_MODULE_PRIVATE
	GSErrCode	Set	(const FSRef& targetRef);                                       // DOC
#endif
	GSErrCode   Set (const CFURLRef& targetURL);                                    // DOC
	GSErrCode   Set (UTF8POSIXPathSelector selector, const char* posixPath);        // DOC
#endif
	/* obsolete */	GSErrCode	SetObsolete (const GS::UniString& targetPath); /* obsolete: does not assert for HFS on Mac */

	void		Clear	(void);
	bool		IsEmpty (void) const;

		// URI operations

	GS::UniString	GetScheme (void) const;
	void			SetScheme (const GS::UniString& newScheme);

	GS::UniString	GetUserName (void) const;
	void			SetUserName (const GS::UniString& newUserName);
	static bool		IsValidUserName (const GS::UniString& scheme);

	GS::UniString	GetPassword (void) const;
	void			SetPassword (const GS::UniString& newPassword);
	static bool		IsValidPassword (const GS::UniString& scheme);

	unsigned short	GetPort (void) const;
	void			SetPort (unsigned short newPort);

	GS::UniString	GetRawQuery () const;
	void			SetRawQuery (const GS::UniString& newRawQuery);

	GS::UniString	GetRawFragmentIdentifier () const;
	void			SetRawFragmentIdentifier (const GS::UniString& newRawFragmentIdentifier);

		// Network address operations

	GSErrCode	GetNetworkName	   (UIndex idx, Name* result) const;
	GSErrCode	SetNetworkName	   (UIndex idx, const Name& name);
	GSErrCode	GetLastNetworkName (Name* result) const;
	GSErrCode	SetLastNetworkName (const Name& name);

	GSErrCode	AppendToNetwork	(const Name& name);

	GSErrCode	DeleteNetworkName	  (UIndex idx);
	GSErrCode	DeleteLastNetworkName (void);
	GSErrCode	MoveLastNetworkName	  (Name* result);
	void		ClearNetwork		  (void);

	USize		GetNetworkLength (void) const;
	bool		IsLocal			 (void) const;

		// Local address operations

	GSErrCode	GetLocalName	 (UIndex idx, Name* result) const;
	GSErrCode	SetLocalName	 (UIndex idx, const Name& name);
	GSErrCode	GetLastLocalName (Name* result) const;
	GSErrCode	SetLastLocalName (const Name& name);

	GSErrCode	AppendToLocal (const Name& name);
	GSErrCode	AppendToLocal (const RelativeLocation& relLoc);

	GSErrCode	GetDifference (const Location& rightOp, RelativeLocation* result) const;

	GSErrCode	DeleteLocalName		(UIndex idx);
	GSErrCode	DeleteLastLocalName (void);
	GSErrCode	MoveLastLocalName	(Name* result);
	void		ClearLocal			(void);

	USize		GetLocalLength (void) const;
	bool		IsRoot		   (void) const;

	GSErrCode	EnumerateLocalNames (NameEnumerator* enumerator) const;

		// Network model query

	NetworkModel	GetNetworkModel (void) const;

		// Comparing

	bool		operator==   (const Location& rightOp) const;
	bool		operator!=   (const Location& rightOp) const;
	bool		operator<    (const Location& rightOp) const;

	bool	IsAncestorOf (const Location& rightOp) const;

		// Conversions

	GSErrCode	ToPath	 (GS::UniString* result) const;
	GSErrCode	ToPath	 (Path* result, GSCharCode charCode = CC_Default) const;

#if defined (macintosh)
#ifdef INPUTOUTPUT_MODULE_PRIVATE
	GSErrCode	ToFSRef     (FSRef* result) const;			// DOC
#endif
	GSErrCode   ToCFURL     (CFURLRef* result) const;		// DOC
	GSErrCode   ToUTF8POSIXPath (Path* result) const;		// DOC
#endif

	GSErrCode	ToURL	 (GS::UniString* result) const;
	GSErrCode	ToURL	 (IO::URL* result) const;

	GS::UniString	ToDisplayText (void) const;

	GS::UniString	ToLogText (void) const;

	static void		EncodeURLName (Name* name);
	static void		DecodeURLName (Name* name);

		// Legality

	bool	IsLegalInNativeFileSystem		  (void) const;
	bool	IsMultiplatformInNativeFileSystem (void) const;

		// I/O

	virtual	GSErrCode	Read  (GS::IChannel& ic) override;
	virtual	GSErrCode	Write (GS::OChannel& oc) const override;
	virtual	GSErrCode	ReadXML  (GS::XMLIChannel& ic, const char* tagName = nullptr);
	virtual	GSErrCode	WriteXML (GS::XMLOChannel& oc, const char* tagName = nullptr) const;

		// Other

	ULong	GenerateHashValue (void) const;

		// Debug

	virtual void	Print (GS::OChannel& oc) const override;
	void			PrintDebug () const;
};


IO_DLL_EXPORT void	Append (GS::FastLogger::LogComposer& logComposer, const IO::Location& location);

}	// namespace IO

namespace GS {
	inline ULong	GenerateHashValue (const IO::Location& target);
}	// namespace GS



inline GSErrCode	IO::Location::GetStatus  (void) const
{
	if (networkAddress.GetStatus () != NoError)
		return networkAddress.GetStatus ();
	else if (localAddress.GetStatus () != NoError)
		return localAddress.GetStatus ();
	else
		return status;
}


inline ULong	GS::GenerateHashValue (const IO::Location& target)
{
	return target.GenerateHashValue ();
}
// ______________________________ Class Location _______________________________


#endif
