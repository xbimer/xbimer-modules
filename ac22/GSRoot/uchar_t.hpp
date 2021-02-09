// *****************************************************************************
//
//                      UTF-16 character definitions/functions
//
// Module: GS
// Namespace: GS
// Contact person: FM
//
// SG compatible
//
// *****************************************************************************

#if !defined UCHART_HPP
#define UCHART_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

// from GSRoot
#include "Definitions.hpp"
#include "Array.hpp"
#include "CH.hpp"

// from Standard C++ library
#if defined (_MSC_VER)
#include <stddef.h>
#include <stdarg.h>
namespace std { using ::va_list; }
#else
#include <cstdarg>
#include <cstddef>
#endif

// --------------------------------- Macros ------------------------------------

#if defined(__APPLE__) || defined (TARGET_RT_MAC_MACHO) || defined (__linux__)
	#define GS_WCHART_IS_NOT_UTF16
#else
	#define GS_WCHART_IS_UTF16
#endif


// Macro L(str), str should be string or character constant: "" or '', that is no variable allowed!
#if defined(GS_WCHART_IS_UTF16)
	#define L(str)	GS::DefineConstantUTF16CharacterOrString(L ## str)
#else
	#define L(str)	GS::DefineConstantUTF16CharacterOrString(u##str)
#endif

#if defined (_MSC_VER)
namespace std {
	using ::size_t;
	typedef unsigned __int64 uintmax_t;
	typedef __int64 intmax_t;
}
#elif defined(__MSL__)
#include <cinttypes>
#else
#include <stdint.h>
namespace std {
	using ::uintmax_t;
	using ::intmax_t;
}
#endif

// =============================== uchar_t functions ========================================

namespace GS {

// Constants
const uchar_t UEOF = 0xFFFF;

// Do not use DefineContantUTF16CharacterOrString, use the L("..") macro instead
GSROOT_DLL_EXPORT	     const uchar_t*	GSCALL	DefineConstantUTF16CharacterOrString (const wchar_t* str);
GSROOT_DLL_EXPORT inline const uchar_t	GSCALL	DefineConstantUTF16CharacterOrString (const wchar_t ch) { return uchar_t (ch) ; }

#if defined (GS_WCHART_IS_NOT_UTF16)
	GSROOT_DLL_EXPORT		 const uchar_t*	GSCALL	DefineConstantUTF16CharacterOrString (const uchar_t* str);
	GSROOT_DLL_EXPORT inline const uchar_t	GSCALL	DefineConstantUTF16CharacterOrString (const uchar_t ch) { return ch ; }

	GSROOT_DLL_EXPORT		 const uchar_t*	GSCALL	DefineConstantUTF16CharacterOrString (const char* str);
	GSROOT_DLL_EXPORT inline const uchar_t	GSCALL	DefineConstantUTF16CharacterOrString (const char ch);

	GSROOT_DLL_EXPORT inline const uchar_t*	GSCALL	DefineConstantUTF16CharacterOrString (const char16_t* ch) { return reinterpret_cast<const GS::uchar_t*>(ch) ; }
	GSROOT_DLL_EXPORT inline const uchar_t	GSCALL	DefineConstantUTF16CharacterOrString (const char16_t ch) { return uchar_t (ch) ; }
#endif

// Unimplemented functions disableing the call of the above functions with a variable
GSROOT_DLL_EXPORT const uchar_t*	GSCALL	DefineConstantUTF16CharacterOrString (const wchar_t*& str);
GSROOT_DLL_EXPORT const uchar_t*	GSCALL	DefineConstantUTF16CharacterOrString (const char*& str);

typedef bool (GSCALL *UScanfHandleTextFunc) (void* pointer, int status, const GS::uchar_t *s, int s_len);
typedef bool (GSCALL *UPrintfStringAppender) (void* ctx, const uchar_t* start, const uchar_t* end);
typedef bool (GSCALL *UPrintfHandleTextFunc) (void* pointer, GS::uchar_t *&s, int& s_len);

GSROOT_DLL_EXPORT int	CCALL	snuprintf (uchar_t* buffer, std::size_t n, const char* format, ...);
GSROOT_DLL_EXPORT int	CCALL	snuprintf (uchar_t* buffer, std::size_t n, const uchar_t* format, ...);
GSROOT_DLL_EXPORT int	GSCALL	vsnuprintf (uchar_t* buffer, std::size_t n, const char* format, std::va_list arg);
GSROOT_DLL_EXPORT int	GSCALL	vsnuprintf (uchar_t* buffer, std::size_t n, const uchar_t* format, std::va_list arg);

GSROOT_DLL_EXPORT int	GSCALL	vsuprintf (void* ref,  UPrintfStringAppender appender, const uchar_t* format, std::va_list arg, UPrintfHandleTextFunc bigTFormatHandler = nullptr);

inline const uchar_t*	GetScanfFormatUStrOf (const char* par)		{ static uchar_t formats [] = {'%','c', 0 ,'%','s', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetScanfFormatUStrOf (const Int16* par)		{ static uchar_t formats [] = {'%','h','d', 0 ,'%','h','i', 0 ,'%','h','n', 0 ,'%','h','o', 0 ,'%','h','x', 0 ,'%','h','X', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetScanfFormatUStrOf (const uchar_t* par)	{ static uchar_t formats [] = {'%','C', 0 ,'%','S', 0 ,'%','l','c', 0 ,'%','l','s', 0 ,'%','h','u', 0 ,'%','h','o', 0 ,'%','h','x', 0 ,'%','h','X', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetScanfFormatUStrOf (const Int32* par)		{ static uchar_t formats [] = {'%','d', 0 ,'%','i', 0 ,'%','n', 0 ,'%','o', 0 ,'%','x', 0 ,'%','X', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetScanfFormatUStrOf (const UInt32* par)	{ static uchar_t formats [] = {'%','u', 0 ,'%','o', 0 ,'%','x', 0 ,'%','X', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetScanfFormatUStrOf (const Int64* par)		{ static uchar_t formats [] = {'%','l','l','d', 0 ,'%','l','l','i', 0 ,'%','l','l','n', 0 ,'%','l','l','o', 0 ,'%','l','l','x', 0 ,'%','l','l','X', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetScanfFormatUStrOf (const UInt64* par)	{ static uchar_t formats [] = {'%','l','l','u', 0 ,'%','l','l','o', 0 ,'%','l','l','x', 0 ,'%','l','l','X', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetScanfFormatUStrOf (const float* par)		{ static uchar_t formats [] = {'%','f', 0 ,'%','F', 0 ,'%','g', 0 ,'%','G', 0 ,'%','e', 0 ,'%','E', 0 ,'%','a', 0 ,'%','A', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetScanfFormatUStrOf (const double* par)	{ static uchar_t formats [] = {'%','l','f', 0 ,'%','l','F', 0 ,'%','l','g', 0 ,'%','l','G', 0 ,'%','l','e', 0 ,'%','l','E', 0 ,'%','l','a', 0 ,'%','l','A', 0 , 0 }; return par == nullptr ? L ("") : formats; }
#if defined (GS_64BIT)
  template<class Type>
  inline const uchar_t*	GetScanfFormatUStrOf (Type** par)			{ static uchar_t formats [] = {'%','p', 0 ,'%','l','l','x', 0 ,'%','l','l','X', 0 , 0 }; return par == nullptr ? L ("") : formats; }
#else
  template<class Type>
  inline const uchar_t*	GetScanfFormatUStrOf (Type** par)			{ static uchar_t formats [] = {'%','p', 0 ,'%','l','x', 0 ,'%','l','X', 0 , 0 }; return par == nullptr ? L ("") : formats; }
#endif

inline const uchar_t*	GetPrintfFormatUStrOf (const char* par)		{ static uchar_t formats [] = {'%','s', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const uchar_t* par)	{ static uchar_t formats [] = {'%','S', 0 ,'%','l','s', 0 , 0 }; return par == nullptr ? L ("") : formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const char)			{ static uchar_t formats [] = {'%','c', 0 ,'%','h','h','d', 0 ,'%','h','h','i', 0 ,'%','h','h','n', 0 , 0}; return formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const unsigned char)	{ static uchar_t formats [] = {'%','h','h','o', 0 ,'%','h','h','u', 0 ,'%','h','h','x', 0 ,'%','h','h','X', 0 , 0}; return formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const Int16)			{ static uchar_t formats [] = {'%','h','d', 0 ,'%','h','i', 0 ,'%','h','n', 0 , 0 }; return formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const uchar_t)		{ static uchar_t formats [] = {'%','C', 0 ,'%','l','c', 0 ,'%','h','o', 0 ,'%','h','u', 0 ,'%','h','x', 0 ,'%','h','X', 0 , 0 }; return formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const Int32)			{ static uchar_t formats [] = {'%','d', 0 ,'%','i', 0 ,'%','n', 0 , 0 }; return formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const UInt32)		{ static uchar_t formats [] = {'%','u', 0 ,'%','o', 0 ,'%','x', 0 ,'%','X', 0 , 0 }; return formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const Int64)			{ static uchar_t formats [] = {'%','l','l','d', 0 ,'%','l','l','i', 0 ,'%','l','l','n', 0 , 0 }; return formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const UInt64)		{ static uchar_t formats [] = {'%','l','l','u', 0 ,'%','l','l','o', 0 ,'%','l','l','x', 0 ,'%','l','l','X', 0 , 0 }; return formats; }
inline const uchar_t*	GetPrintfFormatUStrOf (const double)		{ static uchar_t formats [] = {'%','f', 0 ,'%','F', 0 ,'%','g', 0 ,'%','G', 0 ,'%','e', 0 ,'%','E', 0 ,'%','a', 0 ,'%','A', 0 ,'%','l','f', 0 ,'%','l','F', 0 ,'%','l','g', 0 ,'%','l','G', 0 ,'%','l','e', 0 ,'%','l','E', 0 ,'%','l','a', 0 ,'%','l','A', 0 , 0 }; return formats; }
#if defined (GS_64BIT)
  template<class Type>
  inline const uchar_t*	GetPrintfFormatUStrOf (const Type*)			{ static uchar_t formats [] = {'%','p', 0 ,'%','l','l','x', 0 ,'%','l','l','X', 0 , 0 }; return formats; }
#else
  template<class Type>
  inline const uchar_t*	GetPrintfFormatUStrOf (const Type*)			{ static uchar_t formats [] = {'%','p', 0 ,'%','l','x', 0 ,'%','l','X', 0 , 0 }; return formats; }
#endif

#if defined (GS_64BIT)
 #if defined (__GNUC__)
	inline const uchar_t*	GetScanfFormatUStrOf (const long* par)			{ return GetScanfFormatUStrOf ((Int64*)par); }
	inline const uchar_t*	GetScanfFormatUStrOf (const unsigned long* par)	{ return GetScanfFormatUStrOf ((UInt64*)par); }

	inline const uchar_t*	GetPrintfFormatUStrOf (const long par)			{ return GetPrintfFormatUStrOf ((Int64)par); }
	inline const uchar_t*	GetPrintfFormatUStrOf (const unsigned long par)	{ return GetPrintfFormatUStrOf ((UInt64)par); }
 #else //__GNUC__
	inline const uchar_t*	GetScanfFormatUStrOf (const long* par)			{ return GetScanfFormatUStrOf ((Int32*)par); }
	inline const uchar_t*	GetScanfFormatUStrOf (const unsigned long* par)	{ return GetScanfFormatUStrOf ((UInt32*)par); }

	inline const uchar_t*	GetPrintfFormatUStrOf (const long par)			{ return GetPrintfFormatUStrOf ((Int32)par); }
	inline const uchar_t*	GetPrintfFormatUStrOf (const unsigned long par)	{ return GetPrintfFormatUStrOf ((UInt32)par); }
 #endif //__GNUC__
#else //GS_64BIT
 inline const uchar_t*	GetScanfFormatUStrOf (const int* par)			{ return GetScanfFormatUStrOf ((Int32*)par); }
 inline const uchar_t*	GetScanfFormatUStrOf (const unsigned int* par)	{ return GetScanfFormatUStrOf ((UInt32*)par); }

 inline const uchar_t*	GetPrintfFormatUStrOf (const int par)			{ return GetPrintfFormatUStrOf ((Int32)par); }
 inline const uchar_t*	GetPrintfFormatUStrOf (const unsigned int par)	{ return GetPrintfFormatUStrOf ((UInt32)par); }
#endif //GS_64BIT


enum FormatUStrCheckFlags
{
	FormatUStrFullCheck = 0x00,
	FormatUStrNoCheckNumOfResult = 0x01
};

GSROOT_DLL_EXPORT int	GSCALL	checkedsscanf (FormatUStrCheckFlags chkFlags, const uchar_t* str, const uchar_t* format, const uchar_t** formatStrs, ...);
GSROOT_DLL_EXPORT int	GSCALL	checkedvsscanf (FormatUStrCheckFlags chkFlags, const uchar_t* str, const uchar_t* format, const uchar_t** formatStrs, std::va_list arg, UScanfHandleTextFunc bigTFormatHandler = nullptr);
GSROOT_DLL_EXPORT int	GSCALL	checkedsscanf (FormatUStrCheckFlags chkFlags, const uchar_t* str, const char* format, const uchar_t** formatStrs, ...);
GSROOT_DLL_EXPORT int	GSCALL	checkedvsscanf (FormatUStrCheckFlags chkFlags, const uchar_t* str, const char* format, const uchar_t** formatStrs, std::va_list arg, UScanfHandleTextFunc bigTFormatHandler = nullptr);

template<typename FormatCharType, class Type1>
inline int	csscanf (const uchar_t* str, const FormatCharType* format, Type1* par1)
{
	const uchar_t* formatStrs[2] = {GetScanfFormatUStrOf(par1), nullptr};
	return checkedsscanf (FormatUStrFullCheck, str, format, formatStrs, par1);
}

template<typename FormatCharType, class Type1, class... Types>
inline int	csscanf (const uchar_t* str, const FormatCharType* format, Type1* par1, Types*... pars)
{
	const uchar_t* formatStrs[] = { GetScanfFormatUStrOf (par1), GetScanfFormatUStrOf (pars)..., nullptr };
	return checkedsscanf (FormatUStrFullCheck, str, format, formatStrs, par1, pars...);
}


GSROOT_DLL_EXPORT int	GSCALL	checkedsnprintf (FormatUStrCheckFlags chkFlags, uchar_t* buffer, size_t bufferSize, const uchar_t* format, const uchar_t** formatStrs, ...);
GSROOT_DLL_EXPORT int	GSCALL	checkedvsnprintf (FormatUStrCheckFlags chkFlags, uchar_t* buffer, size_t bufferSize, const uchar_t* format, const uchar_t** formatStrs, std::va_list arg);
GSROOT_DLL_EXPORT int	GSCALL	checkedvsprintf (FormatUStrCheckFlags chkFlags, void* ref,  UPrintfStringAppender appender, const uchar_t* format, const uchar_t** formatStrs, std::va_list arg, UPrintfHandleTextFunc bigTFormatHandler = nullptr);
GSROOT_DLL_EXPORT int	GSCALL	checkedsnprintf (FormatUStrCheckFlags chkFlags, uchar_t* buffer, size_t bufferSize, const char* format, const uchar_t** formatStrs, ...);
GSROOT_DLL_EXPORT int	GSCALL	checkedvsnprintf (FormatUStrCheckFlags chkFlags, uchar_t* buffer, size_t bufferSize, const char* format, const uchar_t** formatStrs, std::va_list arg);
GSROOT_DLL_EXPORT int	GSCALL	checkedvsprintf (FormatUStrCheckFlags chkFlags, void* ref,  UPrintfStringAppender appender, const char* format, const uchar_t** formatStrs, std::va_list arg, UPrintfHandleTextFunc bigTFormatHandler = nullptr);

template<typename FormatCharType, class Type1>
inline int	csnprintf (uchar_t* buffer, size_t bufferSize, const FormatCharType* format, Type1 par1)
{
	const uchar_t* formatStrs[2] = {GetPrintfFormatUStrOf(par1), nullptr};
	return checkedsnprintf (FormatUStrFullCheck, buffer, bufferSize, format, formatStrs, par1);
}

template<typename FormatCharType, class Type1, class... Types>
inline int	csnprintf (uchar_t* buffer, size_t bufferSize, const FormatCharType* format, Type1 par1, Types... pars)
{
	const uchar_t* formatStrs[] = { GetPrintfFormatUStrOf (par1), GetPrintfFormatUStrOf (pars)..., nullptr };
	return checkedsnprintf (FormatUStrFullCheck, buffer, bufferSize, format, formatStrs, par1, pars...);
}

GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, Int16& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(Int16 value, uchar_t* buffer, size_t bufferSize);

GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, UInt16& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(UInt16 value, uchar_t* buffer, size_t bufferSize);

GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, Int32& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(Int32 value, uchar_t* buffer, size_t bufferSize);

GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, UInt32& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(UInt32 value, uchar_t* buffer, size_t bufferSize);

GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, Int64& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(Int64 value, uchar_t* buffer, size_t bufferSize);

GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, UInt64& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(UInt64 value, uchar_t* buffer, size_t bufferSize);

GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, float& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(float value, uchar_t* buffer, size_t bufferSize);

GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, double& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(double value, uchar_t* buffer, size_t bufferSize);

#if defined (GS_64BIT)
 GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, long& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
 GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(long value, uchar_t* buffer, size_t bufferSize);

 GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, unsigned long& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
 GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(unsigned long value, uchar_t* buffer, size_t bufferSize);
#else
 GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, int& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
 GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(int value, uchar_t* buffer, size_t bufferSize);

 GSROOT_DLL_EXPORT bool	GSCALL	strtovalue(const uchar_t* str, unsigned int& value, GS::ToValueMode strict = GS::NotStrict, GS::TrimWSMode trim = GS::TrimWhiteSpaces);
 GSROOT_DLL_EXPORT bool	GSCALL	valuetostr(unsigned int value, uchar_t* buffer, size_t bufferSize);
#endif

GSROOT_DLL_EXPORT long				GSCALL	ucstol(const uchar_t* str, const uchar_t** end, int base);
GSROOT_DLL_EXPORT double			GSCALL	ucstod(const uchar_t* str, const uchar_t** end);

GSROOT_DLL_EXPORT std::size_t		GSCALL	ucslen (const uchar_t* str);
GSROOT_DLL_EXPORT uchar_t*			GSCALL	ucscat (uchar_t* str, const uchar_t* src);
GSROOT_DLL_EXPORT uchar_t*			GSCALL	ucsrev (uchar_t* str);
GSROOT_DLL_EXPORT std::uintmax_t	GSCALL	ucstoumax(const uchar_t* nptr, const uchar_t** endptr, int base);
GSROOT_DLL_EXPORT std::intmax_t		GSCALL	ucstoimax(const uchar_t* nptr, const uchar_t** endptr, int base);
GSROOT_DLL_EXPORT const uchar_t*	GSCALL	umemchr(const uchar_t* s, uchar_t c, std::size_t n);
GSROOT_DLL_EXPORT uchar_t*			GSCALL	umemchr(uchar_t* s, uchar_t c, std::size_t n);

inline UInt32	ucslen32 (const uchar_t* pStr)
{
	return  GS::TruncateTo32Bit (ucslen (pStr));
}


// warning: the following functions are implemented for "C"-locale only
//  for example: is isualpha is 0 for non ASCII chars.

GSROOT_DLL_EXPORT uchar_t*			GSCALL	ucslwr (uchar_t* str);
GSROOT_DLL_EXPORT uchar_t*			GSCALL	ucsupr (uchar_t* str);
GSROOT_DLL_EXPORT int				GSCALL	ucsicmp (const uchar_t* str1, const uchar_t* str2);
GSROOT_DLL_EXPORT int				GSCALL	ucscmp (const uchar_t* str1, const uchar_t* str2);
GSROOT_DLL_EXPORT int				GSCALL	ucsncmp (const uchar_t* str1, const uchar_t* str2, std::size_t n);
GSROOT_DLL_EXPORT int				GSCALL	ucsnicmp (const uchar_t* str1, const uchar_t* str2, std::size_t n);
GSROOT_DLL_EXPORT uchar_t			GSCALL	toulower (uchar_t ch);
GSROOT_DLL_EXPORT uchar_t			GSCALL	touupper (uchar_t ch);
GSROOT_DLL_EXPORT bool				GSCALL	isulower (uchar_t ch);
GSROOT_DLL_EXPORT bool				GSCALL	isudigit (uchar_t ch);
GSROOT_DLL_EXPORT bool				GSCALL	isualpha (uchar_t ch);
GSROOT_DLL_EXPORT bool				GSCALL	isuspace (uchar_t ch);
GSROOT_DLL_EXPORT uchar_t*			GSCALL	ucschr (uchar_t* str, const uchar_t ch);
GSROOT_DLL_EXPORT const uchar_t*	GSCALL	ucschr (const uchar_t* str, const uchar_t ch);
GSROOT_DLL_EXPORT const uchar_t*	GSCALL	ucsrchr (const uchar_t* str, const uchar_t ch);
GSROOT_DLL_EXPORT uchar_t*			GSCALL	ucsncpy (uchar_t* dst, const uchar_t* src, std::size_t n);
GSROOT_DLL_EXPORT uchar_t*			GSCALL	ucscpy (uchar_t* dst, const uchar_t* src);
GSROOT_DLL_EXPORT uchar_t*			GSCALL	ucsstr (uchar_t* str, const uchar_t* pat);
GSROOT_DLL_EXPORT const uchar_t*	GSCALL	ucsstr (const uchar_t* str, const uchar_t* pat);


inline GS::uchar_t *	UCopy (const uchar_t* src, uchar_t* dst)
{
	return  ucscpy (dst, src);
}


inline void	UTruncate (const uchar_t* src, uchar_t* dst, UInt32 destSizeOfBytes /* size_t */)
{
	UInt32 destMaxUChar = destSizeOfBytes / sizeof (uchar_t);
	ucsncpy (dst, src, destMaxUChar);
}


// === Digit grouping and decimal symbol query =========================================================================

class GSROOT_DLL_EXPORT DigitGroupingSpecification {
public:
	class ConstIterator;
	friend class ConstIterator;

	enum RepeatOption {
		DoNotRepeatLast,		// the iteration ends when the array ends
		RepeatLastInfinitely	// the iteration goes through the array, then the last item is repeated infinitely
	};

	DigitGroupingSpecification ();
	DigitGroupingSpecification (const Array<UInt8>& digitGroupSizes, RepeatOption repeatOption);

	void			Set (const Array<UInt8>& newDigitGroupSizes, RepeatOption newRepeatOption);
	bool			IsEmpty (void) const;

	ConstIterator	begin (void) const;
	ConstIterator	end (void) const;

private:
	Array<UInt8>	digitGroupSizes;
	RepeatOption	repeatOption;
};


class GSROOT_DLL_EXPORT DigitGroupingSpecification::ConstIterator {
public:
	ConstIterator (const DigitGroupingSpecification* target, UIndex currentIndex);

	UInt8			operator*  () const;
	ConstIterator&	operator++ ();
	bool			operator== (const ConstIterator& rightOp) const;
	bool			operator!= (const ConstIterator& rightOp) const;

private:
	const DigitGroupingSpecification*	target;
	UIndex								currentIndex;
};


GSROOT_DLL_EXPORT uchar_t						GSCALL	GetDecimalSymbol (void);
GSROOT_DLL_EXPORT uchar_t						GSCALL	GetDigitGroupingSymbol (void);
GSROOT_DLL_EXPORT uchar_t						GSCALL	GetListSeparatorSymbol (void);
GSROOT_DLL_EXPORT DigitGroupingSpecification	GSCALL	GetDigitGroupingSpecification (void);


}

// _______________________________ uchar_t functions _______________________________


#endif
