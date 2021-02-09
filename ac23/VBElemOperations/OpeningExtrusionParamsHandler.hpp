#ifndef OPENING_EXTRUSION_PARAMS_HANDLER_HPP
#define OPENING_EXTRUSION_PARAMS_HANDLER_HPP
#pragma once


#include "Optional.hpp"
#include "VBElemOperationsExport.hpp"

namespace OpeningGeometry { class ExtrusionParameters; }
namespace VBElem { class OpeningDefault; }


namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT ExtrusionParamsHandler
{
public:
	typedef GS::Guid ParameterKey;

	static const ParameterKey WidthKey;
	static const ParameterKey HeightKey;
	static const ParameterKey AnchorAltitudeKey;
	static const ParameterKey ExtrusionStartOffsetKey;
	static const ParameterKey FiniteBodyLengthKey;

	static bool IsValid (const OpeningGeometry::ExtrusionParameters& params, const ParameterKey& key);
	static GS::Optional<double> GetValue (const OpeningGeometry::ExtrusionParameters& params, const ParameterKey& key);
	static void SetValue (OpeningGeometry::ExtrusionParameters& params, const ParameterKey& key, double newValue);
	static void SetValue (VBElem::OpeningDefault& openingDefault, const ParameterKey& key, double newValue);
};

}
}

#endif
