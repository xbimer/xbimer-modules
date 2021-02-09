#ifndef OPENING_MODEL_VIEW_OPTION_HPP
#define OPENING_MODEL_VIEW_OPTION_HPP

#include "VBElemOperationsExport.hpp"

namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT OpeningModelViewOption
{
public:
	enum class OpeningDetailLevel
	{
		ShowSymbolic = 0,
		ShowHole = 1,
		DontShow = 2
	};

	enum class OpeningShowAxisLineOption {
		ShowAxisLine = 0,
		NotShowAxisLine = 1
	};

	virtual ~OpeningModelViewOption ();

	virtual OpeningDetailLevel GetDetailLevel () const = 0;
	virtual void SetDetailLevel (OpeningDetailLevel newDetailLevel) = 0;
	virtual OpeningShowAxisLineOption GetShowAxisLineOption () const = 0;
	virtual void SetShowAxisLineOption (OpeningShowAxisLineOption openingShowAxisLineOption) = 0;

	bool operator== (const OpeningModelViewOption& other) const;
	bool operator!= (const OpeningModelViewOption& other) const;
	
	void Copy (const OpeningModelViewOption& source);
};

}

#endif
