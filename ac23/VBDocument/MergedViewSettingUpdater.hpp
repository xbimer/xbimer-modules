// *********************************************************************************************************************
// Description:		MergedViewSettingUpdater definition
//
// Module:			VBD
// Namespace:		VBD
// Contact person:	SzoL
//
// SG compatible
// *********************************************************************************************************************

#if !defined (__VBD_MERGEDVIEWSETTINGUPDATER_HPP__)
#define __VBD_MERGEDVIEWSETTINGUPDATER_HPP__

#pragma once

// from VBDocument
#include "VBDView.hpp"
#include "VBDocumentExport.hpp"

// from ObjectDatabase
#include "ODBChangeProcessor.hpp"
#include "ODBDependencyNode.hpp"

namespace VBD {

class VBDOCUMENT_DLL_EXPORT_FOR_TEST MergedViewSettingUpdater : public ODB::TypedSequentialChangeProcessor<VBD::View>
{
	DECLARE_DYNAMIC_CLASS_INFO (MergedViewSettingUpdater)

public:
	virtual	GSErrCode	Process (const ODB::Ref<VBD::View>& view) override;
	virtual Triggers	GetTriggers (void) const override;
};

extern	VBDOCUMENT_DLL_EXPORT ODB::DependencyNode mergedViewSettingUpdater;

} // namespace VBD

#endif /* __VBD_MERGEDVIEWSETTINGUPDATER_HPP__ */