
// *****************************************************************************
//
//                                 Class UniChar
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB
//
// SG compatible
//
// *****************************************************************************


#if !defined UNICHAR_HPP
#define UNICHAR_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

#include "CH.hpp"



// =============================== Class UniChar ===============================

namespace GS {

class GSROOT_DLL_EXPORT UniChar {
public:
	typedef uchar_t	Layout;		// defines the physical layout of Unicode characters

private:
	Layout	uniChar;	// stores a Unicode character

	inline static Layout	CharToUnicode (char c);
	inline static char		UnicodeToChar (Layout u);

	class Type1 {};		// for the documentation generator
	class Type2 {};		// for the documentation generator

	enum NormalizationFormType {
		NormalizationFormD,			// Canonical Decomposition
		NormalizationFormC,			// Canonical Decomposition, followed by Canonical Composition
		NormalizationFormKD,		// Compatibility Decomposition
		NormalizationFormKC			// Compatibility Decomposition, followed by Canonical Composition
	};

	static void			ConvertToUnicodeNormalizationForm (const Layout* source, USize sourceLength, NormalizationFormType nf, Layout** result, USize* resultLength);

public:
		// Constructors

	inline UniChar ();
	inline UniChar (Layout u);
		   UniChar (char c);
		   UniChar (unsigned char c);
	inline UniChar (const UniChar& source);
	inline UniChar&		operator= (Layout u);
		   UniChar&		operator= (char c);
		   UniChar&		operator= (unsigned char c);
	inline UniChar&		operator= (const UniChar& source);

		// Conversion

	inline operator Layout () const;
		   operator char () const;
		   operator unsigned char () const;

	enum OnConversionError { ReplaceWithFillers, IndicateError };

	static USize		GetUnicodeLength (const char* str, USize length, GSCharCode charCode = CC_Default);
	static bool	GSCALL	ANSIToUnicode	 (const char* source, USize sourceLength, Layout* result, USize resultLength, GSCharCode charCode = CC_Default);

	static USize GSCALL	GetANSILength	 (const Layout* str, USize length, GSCharCode charCode = CC_Default, OnConversionError onConversionError = ReplaceWithFillers);
	static bool	GSCALL	UnicodeToANSI	 (const Layout* source, USize sourceLength, char* result, USize resultLength, GSCharCode charCode = CC_Default, OnConversionError onConversionError = ReplaceWithFillers);

	static void			ConvertToDecomposed				 (const Layout* source, USize sourceLength, Layout** result, USize* resultLength);
	static void			ConvertToComposed				 (const Layout* source, USize sourceLength, Layout** result, USize* resultLength);
	static void			ConvertToCompatibilityDecomposed (const Layout* source, USize sourceLength, Layout** result, USize* resultLength);
	static void			ConvertToCompatibilityComposed	 (const Layout* source, USize sourceLength, Layout** result, USize* resultLength);

	inline static bool	IsUnicodeCompatible (char c);
	inline static bool	IsANSICompatible    (Layout u);

	   // Comparing

	GSROOT_DLL_EXPORT friend inline bool	operator== (const UniChar& leftOp, const UniChar& rightOp);
	GSROOT_DLL_EXPORT friend inline bool	operator== (const UniChar& leftOp, Layout rightOp);
	GSROOT_DLL_EXPORT friend inline bool	operator== (Layout leftOp, const UniChar& rightOp);
	GSROOT_DLL_EXPORT friend		bool	operator== (const UniChar& leftOp, char rightOp);
	GSROOT_DLL_EXPORT friend		bool	operator== (char leftOp, const UniChar& rightOp);
	GSROOT_DLL_EXPORT friend		bool	operator== (const UniChar& leftOp, unsigned char rightOp);
	GSROOT_DLL_EXPORT friend		bool	operator== (unsigned char leftOp, const UniChar& rightOp);

	GSROOT_DLL_EXPORT friend inline bool	operator!= (const UniChar& leftOp, const UniChar& rightOp);
	GSROOT_DLL_EXPORT friend inline bool	operator!= (const UniChar& leftOp, Layout rightOp);
	GSROOT_DLL_EXPORT friend inline bool	operator!= (Layout leftOp, const UniChar& rightOp);
	GSROOT_DLL_EXPORT friend		bool	operator!= (const UniChar& leftOp, char rightOp);
	GSROOT_DLL_EXPORT friend		bool	operator!= (char leftOp, const UniChar& rightOp);
	GSROOT_DLL_EXPORT friend		bool	operator!= (const UniChar& leftOp, unsigned char rightOp);
	GSROOT_DLL_EXPORT friend		bool	operator!= (unsigned char leftOp, const UniChar& rightOp);

	enum CaseComparison { CaseSensitive, CaseInsensitive };
	enum CompareResult { Less, Equal, Greater };
	static CompareResult	Compare (const Layout* leftOp, USize leftOpLength, const Layout* rightOp, USize rightOpLength, CaseComparison caseComparison = CaseSensitive);

	   // Classification

	bool	IsASCII				(void) const;
	bool	IsDigit				(void) const;
	bool    IsASCIIDigit		(void) const;
	bool	IsASCIIHexaDigit	(void) const;
	bool	IsAlpha				(void) const;
	bool	IsAlphaNumeric		(void) const;
	bool	IsWhiteSpace		(void) const;
	bool	IsPunctuation		(void) const;
	bool	IsControl			(void) const;
	bool	IsLower				(void) const;
	bool	IsASCIILower		(void) const;
	bool	IsUpper				(void) const;
	bool	IsASCIIUpper		(void) const;


		// Conversion

	int     GetASCIIDigitValue (void) const;
	int		GetASCIIHexaDigitValue (void) const;

		// Manipulation

	enum Case { Lower, Upper };
	static USize	ComputeCaseConvertedLength (const Layout* str, USize length, Case cs);
	static bool		ConvertCase (const Layout* source, USize sourceLength, Layout* result, USize resultLength, Case cs);
	enum Width { HalfWidth, FullWidth };
	static USize	ComputeWidthConvertedLength (const Layout* str, USize length, Width wd);
	static bool		ConvertWidth (const Layout* source, USize sourceLength, Layout* result, USize resultLength, Width wd);
};

}	// namespace GS



inline GS::UniChar::UniChar ():
	uniChar (0)
{
}


inline GS::UniChar::UniChar (Layout u):
	uniChar (u)
{
}


inline GS::UniChar::UniChar (const UniChar& source):
	uniChar (source.uniChar)
{
}


inline GS::UniChar&		GS::UniChar::operator= (Layout u)
{
	uniChar = u;

	return *this;
}


inline GS::UniChar&		GS::UniChar::operator= (const UniChar& source)
{
	uniChar = source.uniChar;

	return *this;
}


inline GS::UniChar::operator GS::UniChar::Layout () const
{
	return uniChar;
}


inline bool	GS::UniChar::IsUnicodeCompatible (char c)
{
#if defined (WINDOWS)
	return (c >= 0);
#endif

#if defined macintosh
	return (c >= 0 && c != '\\');		// in japanese code page won character is used in place of backslash character which is mapped to different Unicode character value
#endif

#if defined (__linux__)
	return (c >= 0);
#endif
}


inline bool	GS::UniChar::IsANSICompatible (Layout u)
{
#if defined (WINDOWS)
	return (u <= 127);
#endif

#if defined macintosh
	return (u <= 127 && u != '\\');		// backslash character is mapped to different ANSI character value on japanese code page
#endif

#if defined (__linux__)
	return (u <= 127);
#endif
}


inline bool		GS::operator== (const UniChar& leftOp, const UniChar& rightOp)
{
	return (leftOp.uniChar == rightOp.uniChar);
}


inline bool		GS::operator== (const UniChar& leftOp, UniChar::Layout rightOp)
{
	return (leftOp.uniChar == rightOp);
}


inline bool		GS::operator== (UniChar::Layout leftOp, const UniChar& rightOp)
{
	return (leftOp == rightOp.uniChar);
}


inline bool		GS::operator!= (const UniChar& leftOp, const UniChar& rightOp)
{
	return (leftOp.uniChar != rightOp.uniChar);
}


inline bool		GS::operator!= (const UniChar& leftOp, UniChar::Layout rightOp)
{
	return (leftOp.uniChar != rightOp);
}


inline bool		GS::operator!= (UniChar::Layout leftOp, const UniChar& rightOp)
{
	return (leftOp != rightOp.uniChar);
}
// _______________________________ Class UniChar _______________________________


#endif
