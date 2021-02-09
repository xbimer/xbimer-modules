// *****************************************************************************
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB
//
// SG compatible
//
// *****************************************************************************

#if !defined STRINGCONVERSION_HPP
#define STRINGCONVERSION_HPP

#pragma once


#include "UniString.hpp"
#include "String.hpp"
#include "BM.hpp"


namespace GS {


/**
*	Converts the given value to UniString.
*
*	@return	The UniString created from the value, or empty UniString if the
*		conversion was not successful.
*/
template<class Type>
GS::UniString	ValueToUniString (const Type& source)
{
	const size_t bufferSize = 256;
	GS::UniChar::Layout buffer[bufferSize];

	if (valuetostr (source, buffer, bufferSize))
		return GS::UniString (buffer);
	else
		return GS::UniString ();
}


/**
*	Converts the given value to String.
*
*	@return	The String created from the value, or empty String if the
*		conversion was not successful.
*/
template<class Type>
GS::String	ValueToString (const Type& source)
{
	const size_t bufferSize = 256;
	char buffer[bufferSize];

	if (valuetostr (source, buffer, bufferSize))
		return GS::String (buffer);
	else
		return GS::String ();
}
	

/**
*	Fills 'value' with the value of the given UniString.
*
*	@param	source	The UniString to get a value from.
*	@param	value	The result of conversion will be put here.
*	@param	strict	- NotStrict (default): similar to the behaviour of C++
*		standard sscanf function: converts as many characters to the desired
*		type, as many it can, or puts 0 to 'value', if the string's first
*		non-whitespace character is not of the desired type. (e.g. "5.42ag" => 5.42,
*		"ag5.42" => 0 etc. if type of 'value' is double; "5.42" => 5 etc. if int)
*					- Strict: requires the string to contain only characters
*		which can be converted to the desired type, does not let whitespaces
*		in the middle of the string either (e.g. "5.42a" => (-), "5.42 5" => (-)
*		etc. if type of 'value' is double; "5.42" => (-) etc. if int).
*		This parameter does not effect whether whitespaces around the string
*		should be considered non-convertable, change 'trim' parameter to forbid WS-s.
*	@param	trim	- TrimWhiteSpaces (default): similar to the behaviour of C++
*		standard sscanf function: skips every whitespaces in the beginning and
*		at the end of the string.
*					- DontTrimWhiteSpaces: with this option we forbid every
*		leading and trailing whitespaces in the string to convert.
*
*	@return	Whether conversion was successful or not. Make sure to check this
*		return value before processing param 'value', because on conversion 
*		failure its content is undefined.
*/
template<class Type>
bool	UniStringToValue (const GS::UniString&	source,
						  Type&					value,
						  const GS::ToValueMode	strict = GS::NotStrict,
						  const GS::TrimWSMode	trim   = GS::TrimWhiteSpaces)
{
	return strtovalue (source.ToUStr ().Get (), value, strict, trim);
}


/**
*	Fills 'value' with the value of the given String.
*
*	@param	source	The String to get a value from.
*	@param	value	The result of conversion will be put here.
*	@param	strict	- NotStrict (default): similar to the behaviour of C++
*		standard sscanf function: converts as many characters to the desired
*		type, as many it can, or puts 0 to 'value', if the string's first
*		non-whitespace character is not of the desired type. (e.g. "5.42ag" => 5.42,
*		"ag5.42" => 0 etc. if type of 'value' is double; "5.42" => 5 etc. if int)
*					- Strict: requires the string to contain only characters
*		which can be converted to the desired type, does not let whitespaces
*		in the middle of the string either (e.g. "5.42a" => (-), "5.42 5" => (-)
*		etc. if type of 'value' is double; "5.42" => (-) etc. if int).
*		This parameter does not effect whether whitespaces around the string
*		should be considered non-convertable, change 'trim' parameter to forbid WS-s.
*	@param	trim	- TrimWhiteSpaces (default): similar to the behaviour of C++
*		standard sscanf function: skips every whitespaces in the beginning and
*		at the end of the string.
*					- DontTrimWhiteSpaces: with this option we forbid every
*		leading and trailing whitespaces in the string to convert.
*
*	@return	Whether conversion was successful or not. Make sure to check this
*		return value before processing param 'value', because on conversion 
*		failure its content is undefined.
*/
template<class Type>
bool	StringToValue (const GS::String&	 source,
					   Type&				 value,
					   const GS::ToValueMode strict	= GS::NotStrict,
					   const GS::TrimWSMode	 trim	= GS::TrimWhiteSpaces)
{
	return strtovalue (source.ToCStr (), value, strict, trim);
}


/**
 *	Copies 'sourceString' to 'destHandle' in UTF-16 format. The handle will not contain the closing L'\0'.
 *	
 *	@param	sourceString	The string to copy.
 *	@param	destHandle		The target handle. The caller is responsible to free the allocated memory.
 *	
 */
inline void		UniStringToUnicodeHandle (const GS::UniString& sourceString, GSHandle& destHandle)
{
	destHandle = BMhAll (sourceString.GetLength () * sizeof (GS::UniChar::Layout));
	BNCopyMemory (*destHandle, sourceString.ToUStr ().Get (), sourceString.GetLength () * sizeof (GS::UniChar::Layout));
}


		
/**
 *	Convert hexadecimal character to number
 */
inline UChar HexCharToNumber (UChar c)
{
	if (isdigit (c))
		return c - '0';
	else if (isupper (c))
		return c - 'A' + 10;
	else if (islower (c))
		return c - 'a' + 10;
	else {
		DBBREAK ();
		return 0;
	}
}

/**
 *	Convert hexadecimal unistring to binary code, and puts to a GShandle
 */
inline void GetBinaryFromHexadecimal (const GS::UniString& source, GSHandle* gMem)
{
	UInt32 nBytes = 0;
	GSHandle hexa = BMhAll (0);

	USize len = source.GetLength();
	DBASSERT (len % 2 == 0);
	BMhSetSize (hexa, len);
	memcpy (*hexa, source.ToCStr().Get(), len);
	nBytes = len / 2;

	BMhSetSize (*gMem, nBytes);
	for (UInt32 i = 0; i < nBytes; ++i) {
		UChar high	= HexCharToNumber((*hexa)[2 * i]);
		UChar low	= HexCharToNumber((*hexa)[2 * i + 1]);
		(**gMem)[i] = (high << 4) + (low);
	}

	BMhFree (hexa);
}
	
inline void BinaryToHexaDecimalToCharBuffer (GS::UniChar::Layout* hexaDecimalDst, const char* binarySrc, USize nSrcBytes)
{
	for (UIndex i = 0; i < nSrcBytes; ++i) {
		UInt32 byteHigh				= reinterpret_cast<const unsigned char*>(binarySrc)[i] >> 4;
		UInt32 byteLow				= reinterpret_cast<const unsigned char*>(binarySrc)[i] & 0x0F;

		hexaDecimalDst[2 * i]		= static_cast<unsigned char>((byteHigh < 0xA) ? byteHigh + '0' : byteHigh - 10 + 'A');
		hexaDecimalDst[2 * i + 1]	= static_cast<unsigned char>((byteLow < 0xA) ? byteLow + '0' : byteLow - 10 + 'A');
	}
}

inline void BinaryToHexaDecimalToUniString (GS::UniString& hexaDecimalDst, const char* binarySrc, USize nSrcBytes)
{
	hexaDecimalDst.SetLength (0);

	if (binarySrc != nullptr) {
		GS::UniChar::Layout		workStr[200];

		for ( ; nSrcBytes > 100; binarySrc += 100, nSrcBytes -= 100) {
			BinaryToHexaDecimalToCharBuffer (workStr, binarySrc, 100);
			hexaDecimalDst.Append (workStr, 2 * 100);
		}
		if (nSrcBytes > 0) {
			BinaryToHexaDecimalToCharBuffer (workStr, binarySrc, nSrcBytes);
			hexaDecimalDst.Append (workStr, 2 * nSrcBytes);
		}
	}
}


}	// namespace GS

#endif
