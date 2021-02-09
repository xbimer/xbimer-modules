#define VBELEM_LAYER_GETTER_EXTENSION_HPP
#pragma once

#include "Extension.hpp"
#include "VBElemOperationsExport.hpp"
#include "EDBGeneralDefinitions.hpp"
#include "Optional.hpp"
#include "ADBAttributeIndex.hpp"

namespace VBEO{
	
class VB_ELEM_OPERATIONS_DLL_EXPORT VBElemLayerGetterExtension : public GS::Extension {
public:
	static GS::ExtensionId id;
	VBElemLayerGetterExtension (GS::ClassInfo* target);
	virtual ~VBElemLayerGetterExtension ();

	virtual GS::Optional<ADB::AttributeIndex> GetLayer (const EDB::GeneralElemConstRef& elemRef) const;
};

}