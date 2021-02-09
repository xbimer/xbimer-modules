// *********************************************************************************************************************
// Description:		BeamModelViewOption
//
// Module:			VBElemOperations
// Namespace:		VBElemOperations
// Contact person:	KKr
//
// SG compatible
// *********************************************************************************************************************

#ifndef _BEAMMODELVIEWOPTION_HPP_
#define _BEAMMODELVIEWOPTION_HPP_

#include "VBElemOperationsExport.hpp"
#include "Object.hpp"

namespace BeamMVO {

enum class SegmentDetailLevel {
	Detailed,
	Simplified
};

}

class VB_ELEM_OPERATIONS_DLL_EXPORT BeamModelViewOption : public GS::Object {
	DECLARE_CLASS_INFO

private:
	BeamMVO::SegmentDetailLevel	curveDetailLevel;

public:
	BeamModelViewOption ();
	virtual ~BeamModelViewOption ();

public:
	bool operator== (const BeamModelViewOption& rhs) const;
	bool operator!= (const BeamModelViewOption& rhs) const;

public:
	BeamMVO::SegmentDetailLevel	GetSegmentDetailLevel (void) const;
	void						SetSegmentDetailLevel (BeamMVO::SegmentDetailLevel detailLevel);

public:
	virtual BeamModelViewOption*	Clone (void) const override;
	virtual GSErrCode				Read (GS::IChannel& ic) override;
	virtual GSErrCode				Write (GS::OChannel& oc) const override;

	GSErrCode			ReadXML (GS::XMLIChannel& ic);
	GSErrCode			WriteXML (GS::XMLOChannel& oc) const;

private:
	GSErrCode ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
};


#endif // _BEAMMODELVIEWOPTION_HPP_
