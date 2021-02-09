// *********************************************************************************************************************
// Name:			GSPix.h
//
// Module:			Pattern
// Namespace:		-
// Contact person:	KiP
//
// *********************************************************************************************************************


#ifndef	GSPIX_H
#define	GSPIX_H

#pragma once

#include "Definitions.hpp"
#include "ObjectState.hpp"
#include "PatternExport.h"


namespace GS { class IChannel; }
namespace GS { class OChannel; }
namespace GS { class XMLIChannel; }
namespace GS { class XMLOChannel; }
namespace Gfx { class Color; }


// === struct GS_RGBColor ==============================================================================================
// This struct must be the same, as the RGBColor struct in the Mac Headers

struct GS_RGBColor {
	UShort red;
	UShort green;
	UShort blue;
};


// === GS_RGBColor global functions ====================================================================================

inline bool		operator== (const GS_RGBColor& leftOp, const GS_RGBColor& rightOp)
{
	return leftOp.red   == rightOp.red   &&
		   leftOp.green == rightOp.green &&
		   leftOp.blue  == rightOp.blue;
}


inline bool		operator!= (const GS_RGBColor& leftOp, const GS_RGBColor& rightOp)
{
	return !(leftOp == rightOp);
}


PATTERN_DLL_EXPORT GSErrCode	ReadRGBColor  (GS::IChannel& ic, GS_RGBColor& obj);
PATTERN_DLL_EXPORT GSErrCode	WriteRGBColor (GS::OChannel& oc, const GS_RGBColor& obj);

PATTERN_DLL_EXPORT GSErrCode ReadXMLRGB (GS::XMLIChannel& ic, const char* tagName, GS_RGBColor& rgb);
PATTERN_DLL_EXPORT GSErrCode WriteXMLRGB (GS::XMLOChannel& oc, const char* tagName, const GS_RGBColor& rgb);

PATTERN_DLL_EXPORT GSErrCode	Store (GS::ObjectState& os, const GS_RGBColor& color);
PATTERN_DLL_EXPORT GSErrCode	Restore (const GS::ObjectState& os, GS_RGBColor& color);
// GSErrCode	Read (GS::IChannel& ic, GS_RGBColor& obj) { return ReadRGBColor (ic, obj); }
// GSErrCode	Write (GS::OChannel& oc, const GS_RGBColor& obj) { return WriteRGBColor (oc, obj); }

inline GSErrCode 	ReadXMLRGB (GS::XMLIChannel& ic, GS_RGBColor& rgb)									{ return ReadXMLRGB (ic, "RGBColor", rgb); }
inline GSErrCode 	WriteXMLRGB (GS::XMLOChannel& oc, const GS_RGBColor& rgb)							{ return WriteXMLRGB (oc, "RGBColor", rgb); }

namespace GS {
	inline GSErrCode 	ReadXML (GS::XMLIChannel& ic, GS_RGBColor& rgb)									{ return ReadXMLRGB (ic, rgb); }
	inline GSErrCode 	WriteXML (GS::XMLOChannel& oc, const GS_RGBColor& rgb)							{ return WriteXMLRGB (oc, rgb); }
	inline GSErrCode 	ReadXML (GS::XMLIChannel& ic, const char* tagName, GS_RGBColor& rgb)			{ return ReadXMLRGB (ic, tagName, rgb); }
	inline GSErrCode 	WriteXML (GS::XMLOChannel& oc, const char* tagName, const GS_RGBColor& rgb)		{ return WriteXMLRGB (oc, tagName, rgb); }
}


// === struct GX_ARGBColor =============================================================================================

struct GX_ARGBColor {
	static constexpr UChar MAXVAL = MaxUChar;

	UChar	alpha;
	UChar	red;
	UChar	green;
	UChar	blue;
};


// === struct GSPattern ================================================================================================

struct GSPattern {
	GSPattern ();
	GSPattern (UChar p0, UChar p1, UChar p2, UChar p3, UChar p4, UChar p5, UChar p6, UChar p7);

	bool	IsEmpty () const;
	bool	IsFull () const;

	UChar	pat[8];
};


// === GSPattern inline methods ========================================================================================

inline GSPattern::GSPattern () :
	pat {}
{
}


inline GSPattern::GSPattern (UChar p0, UChar p1, UChar p2, UChar p3, UChar p4, UChar p5, UChar p6, UChar p7) :
	pat { p0, p1, p2, p3, p4, p5, p6, p7 }
{
}


inline bool		GSPattern::IsEmpty () const
{
	return reinterpret_cast<const UInt64&> (pat) == 0;
}


inline bool		GSPattern::IsFull () const
{
	return reinterpret_cast<const UInt64&> (pat) == MaxUInt64;
}


// === struct GSPoint ==================================================================================================

struct GSPoint {
	Int32	v;
	Int32	h;
};


// === struct GSRect ===================================================================================================

struct GSRect {
	Int32	top;
	Int32	left;
	Int32	bottom;
	Int32	right;
};


// === GSRect global functions =========================================================================================

inline bool		operator== (const GSRect& leftOp, const GSRect& rightOp)
{
	return leftOp.top    == rightOp.top    &&
		   leftOp.left   == rightOp.left   &&
		   leftOp.bottom == rightOp.bottom &&
		   leftOp.right  == rightOp.right;
}


inline bool		operator!= (const GSRect& leftOp, const GSRect& rightOp)
{
	return !(leftOp == rightOp);
}


// === enum GSPixelType ================================================================================================

enum GSPixelType {
	GSPT_ARGB	= 0,
	GSPT_ALongRGB,			// internal usage
	GSPT_BlackWhite,
	GSPT_256Gray,
	GSPT_Unknown
};


// === Type definitions ================================================================================================

struct GSPixMap;

using GSPixMapHandle = GSPixMap**;


// === Global functions ================================================================================================

PATTERN_DLL_EXPORT Gfx::Color	GS_RGBColorToGfxColor (const GS_RGBColor& rgbColor);
PATTERN_DLL_EXPORT GS_RGBColor	GfxColorToGS_RGBColor (const Gfx::Color& gfxColor);


#endif
