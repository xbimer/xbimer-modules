#ifndef	MODELELEM_COMPONENT_ID_HPP
#define	MODELELEM_COMPONENT_ID_HPP

#pragma once

// === Includes ========================================================================================================

// from GSRoot
#include "ClassInfo.hpp"
#include "HashCommon.hpp"

// from VBElemOperations
#include "VBElemOperationsExport.hpp"

// from VectorImage
#include "StructureComponentId.hpp"

// === Predeclarations =================================================================================================

namespace GS {
	class OutputFrame;
	class InputFrame;
}

// =====================================================================================================================

namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT ModelElemComponentId
{
	static GS::ClassInfo classInfo;
public:
	class VB_ELEM_OPERATIONS_DLL_EXPORT SubElementId
	{
		static GS::ClassInfo classInfo;
	public:
		SubElementId ();
		SubElementId (const GS::Guid& guid);

		bool operator== (const SubElementId& other) const;
		bool operator!= (const SubElementId& other) const;

		operator GS::HashValue () const;

		const GS::Guid&	GetGuid () const;

		GSErrCode	Write (GS::OChannel& oc) const;
		GSErrCode	Read (GS::IChannel& ic);
	
	private:
		GSErrCode	WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
		GSErrCode	ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);

		GS::Guid guid;
	};

	ModelElemComponentId ();
	explicit ModelElemComponentId (const StructureComponentId& structureComponentId);
	explicit ModelElemComponentId (const StructureComponentId& structureComponentId, const SubElementId& subElementId);
	
	bool operator== (const ModelElemComponentId& other) const;
	bool operator!= (const ModelElemComponentId& other) const;

	operator GS::HashValue () const;
	
	bool						IsValid () const;
	const StructureComponentId&	GetStructureComponentId () const;
	bool						HasSubElementId () const;
	const SubElementId&			GetSubElementId () const;

	ModelElemComponentId	ExtendWithSubElementId (const SubElementId& subElementId) const;

	GSErrCode	Write (GS::OChannel& oc) const;
	GSErrCode	Read (GS::IChannel& ic);

private:
	GSErrCode	WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode	ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode	WriteVersion2 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode	ReadVersion2 (GS::IChannel& ic, const GS::InputFrame& frame);

	StructureComponentId structureComponentId;
	SubElementId subElementId;
};


VB_ELEM_OPERATIONS_DLL_EXPORT
ModelElemComponentId CreateBasicComponentId (const ModelElemComponentId::SubElementId& subElemId = ModelElemComponentId::SubElementId ());

}

#endif
