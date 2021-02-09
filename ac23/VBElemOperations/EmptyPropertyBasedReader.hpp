#ifndef GS_EMPTY_PROPERTY_BASED_READER_HPP
#define GS_EMPTY_PROPERTY_BASED_READER_HPP

#pragma once

#include "VBElemOperationsExport.hpp"

#include "IPropertyBasedReader.hpp"

class VB_ELEM_OPERATIONS_DLL_EXPORT EmptyPropertyBasedReader: public IPropertyBasedReader {
	virtual GS::Optional<double>		GetElevation () const override;
	virtual EDB::GeneralElemConstRef	GetElem () const override;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT EmptyPropertyBasedReaderFactory: public IPropertyBasedReaderFactory {
public:
	virtual PropertyBasedReaderPtr CreateReader (EDB::GeneralElemConstRef elem) const override;
};

#endif //GS_EMPTY_PROPERTY_BASED_READER_HPP
