#ifndef VBELEM_FILLET_CHAMFER_EXTENSION_HPP
#define	VBELEM_FILLET_CHAMFER_EXTENSION_HPP
#pragma once

#include "Extension.hpp"
#include "VBElemFilletChamfer.hpp"

namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT VBElemFilletChamferExtension : public GS::Extension {
public:
	static GS::ExtensionId id;
	VBElemFilletChamferExtension (GS::ClassInfo* target);
	virtual ~VBElemFilletChamferExtension ();

	virtual bool IsFilletChamferEnabled (const EDB::GeneralElemRef& elem) const = 0;
	virtual void Fillet (const EDB::GeneralElemRef& elem, double radius) const = 0;
	virtual void Chamfer (const EDB::GeneralElemRef& elem, double radius) const = 0;
};

}

#endif