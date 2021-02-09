#ifndef STRINGPARAMETERSUPDATER_HPP
#define STRINGPARAMETERSUPDATER_HPP
#pragma once

#include "GDLExport.h"
#include "GDLParamInterface.hpp"
#include "Optional.hpp"
#include "UniString.hpp"

namespace GDL {


class GDL_DLL_EXPORT StringParametersUpdater {
public:
	virtual ~StringParametersUpdater ();

	void Do (GDL::Parameters& parameters);

private:

	virtual GS::Optional<GS::UniString> GetNewValue (const GS::UniString& stringValue) = 0;
};

}
#endif
