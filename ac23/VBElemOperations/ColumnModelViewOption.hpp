// *********************************************************************************************************************
// Description:		ColumnModelViewOption
//
// Module:			VBElemOperations
// Namespace:		VBElemOperations
// Contact person:	KKr
//
// SG compatible
// *********************************************************************************************************************

#ifndef _COLUMNMODELVIEWOPTION_HPP_
#define _COLUMNMODELVIEWOPTION_HPP_

#include "VBElemOperationsExport.hpp"
#include "Object.hpp"

namespace ColumnMVO {

enum class SegmentDetailLevel {
	Detailed,
	Simplified
};

}

class VB_ELEM_OPERATIONS_DLL_EXPORT ColumnModelViewOption : public GS::Object {
	DECLARE_CLASS_INFO

private:
	ColumnMVO::SegmentDetailLevel	curveDetailLevel;

public:
	ColumnModelViewOption ();
	virtual ~ColumnModelViewOption ();

public:
	bool operator== (const ColumnModelViewOption& rhs) const;
	bool operator!= (const ColumnModelViewOption& rhs) const;

public:
	ColumnMVO::SegmentDetailLevel	GetSegmentDetailLevel (void) const;
	void						SetSegmentDetailLevel (ColumnMVO::SegmentDetailLevel detailLevel);

public:
	virtual ColumnModelViewOption*	Clone (void) const override;
	virtual GSErrCode				Read (GS::IChannel& ic) override;
	virtual GSErrCode				Write (GS::OChannel& oc) const override;

	GSErrCode			ReadXML (GS::XMLIChannel& ic);
	GSErrCode			WriteXML (GS::XMLOChannel& oc) const;

private:
	GSErrCode ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
};


#endif // _COLUMNMODELVIEWOPTION_HPP_
