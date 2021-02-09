#define VBELEM_LAYER_GETTER_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "GeneralElem.hpp"

namespace VBEO {
class VB_ELEM_OPERATIONS_DLL_EXPORT VBElemLayerGetter {
public:
	static GS::Optional<ADB::AttributeIndex> GetLayer (const EDB::GeneralElemConstRef& generalElem);
private:
	EDB::GeneralElemConstRef generalElem;
};
}
