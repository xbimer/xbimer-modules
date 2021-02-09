#ifndef GS_I_PROPERTY_BASED_READER_HPP
#define GS_I_PROPERTY_BASED_READER_HPP

#pragma once

#include "VBElemOperationsExport.hpp"

#include "Optional.hpp"
#include "Owner.hpp"
#include "ODBReference.hpp"

namespace EDB {
class GeneralElem;
typedef ODB::ConstRef<GeneralElem> GeneralElemConstRef;
}

//#KoMa_A180_M4 namespace?

class VB_ELEM_OPERATIONS_DLL_EXPORT IPropertyBasedReader {
public:
	virtual GS::Optional<double>		GetElevation () const = 0;

	virtual EDB::GeneralElemConstRef	GetElem () const = 0;

	virtual ~IPropertyBasedReader();
};

class VB_ELEM_OPERATIONS_DLL_EXPORT IPropertyBasedReaderFactory {
public:
	typedef GS::Owner<IPropertyBasedReader> PropertyBasedReaderPtr;
	virtual PropertyBasedReaderPtr CreateReader (EDB::GeneralElemConstRef elem) const = 0;

	virtual ~IPropertyBasedReaderFactory();
};

#endif //GS_I_PROPERTY_BASED_READER_HPP
