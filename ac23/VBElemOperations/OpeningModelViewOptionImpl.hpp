#ifndef OPENING_MODEL_VIEW_OPTION_IMPL_HPP
#define OPENING_MODEL_VIEW_OPTION_IMPL_HPP

#include "OpeningModelViewOption.hpp"

namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT OpeningModelViewOptionImpl : public OpeningModelViewOption
{
public:
	OpeningModelViewOptionImpl ();
	virtual ~OpeningModelViewOptionImpl ();

	virtual OpeningDetailLevel GetDetailLevel () const override final;
	virtual void SetDetailLevel (OpeningDetailLevel newDetailLevel) override final;
	virtual OpeningShowAxisLineOption GetShowAxisLineOption () const override final;
	virtual void SetShowAxisLineOption (OpeningShowAxisLineOption newOpeningShowAxisLineOption) override final;

	void Init ();

	GSErrCode Read (GS::IChannel& ic);
	GSErrCode Write (GS::OChannel& oc) const;

	GSErrCode ReadXML (GS::XMLIChannel& ic);
	GSErrCode WriteXML (GS::XMLOChannel& oc) const;

private:
	GSErrCode ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;

	OpeningDetailLevel detailLevel;
	OpeningShowAxisLineOption showAxisLineOption;

	static GS::ClassInfo classInfo;
};

}

#endif
