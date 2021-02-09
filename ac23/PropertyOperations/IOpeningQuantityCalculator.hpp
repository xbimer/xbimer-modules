#ifndef I_OPENING_QUANTITY_CALCULATOR_HPP
#define I_OPENING_QUANTITY_CALCULATOR_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "IModelElementQuantityCalculator.hpp"

// from GSModeller
#include "ModelCalculationsDefinitions.hpp"

// =====================================================================================================================

namespace Property {

class PROPERTY_OPERATIONS_DLL_EXPORT IOpeningQuantityCalculator : public IModelElementQuantityCalculator
{
public:
	virtual ~IOpeningQuantityCalculator ();

	virtual ModelCalculations::MinMaxPair GetBodyFirstLastLimits () const = 0;
	virtual double GetThickness () const = 0;
	virtual bool GetIfVisibile () const = 0;
};

}

#endif