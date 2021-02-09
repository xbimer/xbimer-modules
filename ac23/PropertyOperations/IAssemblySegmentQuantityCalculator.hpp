#ifndef I_ASSEMBLYSEGMENT_QUANTITY_CALCULATOR_HPP
#define I_ASSEMBLYSEGMENT_QUANTITY_CALCULATOR_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "PropertyOperationsExport.hpp"
#include "IModelElementQuantityCalculator.hpp"
// =====================================================================================================================

namespace GS {
	template <class Type>
	class HashSet;
}

namespace Property {

class PROPERTY_OPERATIONS_DLL_EXPORT IAssemblySegmentQuantityCalculator : public IModelElementQuantityCalculator
{
public:
	virtual ~IAssemblySegmentQuantityCalculator ();

	virtual double GetCoreSurfaceArea (Int32 surfaceType) const = 0;
};

}

#endif
