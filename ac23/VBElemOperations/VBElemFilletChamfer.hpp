#ifndef VBELEM_FILLET_CHAMFER_HPP
#define VBELEM_FILLET_CHAMFER_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "ODBReference.hpp"

namespace EDB {
	class GeneralElem;
	typedef ODB::Ref<GeneralElem> GeneralElemRef;
}

namespace VBEO {


class VB_ELEM_OPERATIONS_DLL_EXPORT VBElemFilletChamfer {
public:

	VBElemFilletChamfer (const EDB::GeneralElemRef& elem);

	bool IsFilletChamferEnabled ();
	void Fillet (double radius);
	void Chamfer (double radius);
private:
	EDB::GeneralElemRef elem;
};

}


#endif