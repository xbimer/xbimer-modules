
// *****************************************************************************
//
//	                           Class ReflectionService
//
// Module: IO
// Namespace: IO
// Contact person: MB
//
// SG compatible
//
// *****************************************************************************

#if !defined REFLECTIONSERVICE_HPP
#define REFLECTIONSERVICE_HPP

#pragma once

#include "UniString.hpp"
#include "IOBase.hpp"
#include "ThreadSpecificStorage.hpp"

namespace GS {
	class IBinaryChannel;
	class OBinaryChannel;
}

namespace IO {

// ========================== Class ReflectionService ==========================

class IO_DLL_EXPORT ReflectionService {
public:
	static const UShort ServerPort = 65110;
	static const UShort PortsToTry = 10;
	static const UShort PortsToScan = 3;
	static const GS::UniString GetChildrenRequest;
	static const GS::UniString GetAttributesRequest;
	static const GS::UniString GetAttributeValuesRequest;
	static const GS::UniString GetContentRequest;
	static const GS::UniString DisconnectRequest;
	static const GS::UniString IdentRequest;
	static const GS::UniString GetOSRequest;
	static const GS::UniString GetPidRequest;
	static const GS::UniString MacOS;
	static const GS::UniString WinOS;
	static const GS::UniString LinuxOS;
	static const GS::UniString OSVersion;

private:
	ReflectionService ();	// disabled
   ~ReflectionService ();	// disabled
	ReflectionService (const ReflectionService& source);				// disabled
	ReflectionService&	operator= (const ReflectionService& source);	// disabled

public:
	static bool		Initialize (const GS::UniString& prodVers);
	static void		Terminate  ();

// ... helper functions for read / write ........................................
	static void		Read (GS::IBinaryChannel& stream, UInt32* l);
	static void		Read (GS::IBinaryChannel& stream, GS::UniString* string);
	static USize	Read (GS::IBinaryChannel& stream, char* data, UInt32 length);

	static void		Write (GS::OBinaryChannel& stream, UInt32 l);
	static void		Write (GS::OBinaryChannel& stream, const GS::UniString& string);

	static GS::ThreadSpecificStorage<bool> isReflectionThread;
};

}	// namespace IO
// __________________________ Class ReflectionService __________________________

#endif
