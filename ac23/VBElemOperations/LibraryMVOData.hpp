// *********************************************************************************************************************
// Description:		Stores necessary info about library global object
//
// Module:			VBElemOperations
// Namespace:		-
// Contact person:	PK
//
// SG compatible
// *********************************************************************************************************************

#if !defined (LIBRARYMVODATA_HPP)
#define LIBRARYMVODATA_HPP

#pragma once

// --- Includes	--------------------------------------------------------------------------------------------------------

#include "VBElemOperationsExport.hpp"

// from GSRoot
#include "Md5.hpp"
#include "UniString.hpp"
#include "GDLParamList.hpp"
#include "SharedObject.hpp"

namespace GS {
	class XMLOChannel;
	class XMLIChannel;
}

// --- Function declarations -------------------------------------------------------------------------------------------

#define MaxObjectName	256

#define	XMLTag_LibraryMVOData			"LibraryMVOData"
#define	XMLTag_LibraryMVOName			"LibraryMVOName"
#define	XMLTag_ElemGuid					"ElemGuid"
#define	XMLTag_SupersetGuid				"SupersetGuid"
#define	XMLTag_AddParHdl				"AddParHdl"

class VB_ELEM_OPERATIONS_DLL_EXPORT LibraryMVOData : public GS::Object
{
	DECLARE_CLASS_INFO

private:
	GS::UniString				name;
	GS::Guid					elemGuid;
	GS::Guid					supersetGuid;
	GDL::Parameters				addPar;
	MD5::FingerPrint			checkSum;
	
	void					CalcCheckSum	();

	GSErrCode				ReadVersion1	(GS::IChannel& ic);

	GSErrCode				ReadVersion2	(GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode				WriteVersion2	(GS::OChannel& oc, const GS::OutputFrame& frame) const;

	GSErrCode				ReadVersion3 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode				WriteVersion3 (GS::OChannel& oc, const GS::OutputFrame& frame) const;


public:
	LibraryMVOData ();
	LibraryMVOData (const GS::UniString& newName, const GS::Guid& elemGuidIn, const GS::Guid& supersetGuidIn, const GDL::Parameters& addParIn);

	const GS::UniString*	GetName () const { return &name; }
	const GS::Guid*			GetElemGuid () const { return &elemGuid; }
	const GS::Guid*			GetSupersetGuid () const { return &supersetGuid; }
	const GDL::Parameters&	GetAddPar () const { return addPar; }	
	const MD5::FingerPrint&	GetCheckSum () const;

	virtual	GSErrCode		Read		(GS::IChannel& ic) override;
	virtual	GSErrCode		Write		(GS::OChannel& oc) const override;
 	virtual	GSErrCode		ReadXML		(GS::XMLIChannel& ic);
 	virtual	GSErrCode		WriteXML	(GS::XMLOChannel& oc) const;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT LibraryMVODataContainer : public GS::SharedObject
{
private:
	GS::Array<LibraryMVOData> dataArray;

public:
	LibraryMVODataContainer () {}
	LibraryMVODataContainer (const GS::Array<LibraryMVOData>& dataArray) : dataArray (dataArray) {}

	LibraryMVODataContainer& operator= (const LibraryMVODataContainer&) = delete;

	const GS::Array<LibraryMVOData>&	GetDataArray () const { return dataArray; }

	bool								operator == (const LibraryMVODataContainer& other) const;
	bool								operator != (const LibraryMVODataContainer& other) const { return !operator == (other); }

	static void							PushData (GS::Array<LibraryMVOData>& dataArray, const LibraryMVOData& data, bool overwrite = true);

	~LibraryMVODataContainer ();
};


#endif // LIBRARYMVODATA_HPP
