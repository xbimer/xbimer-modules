// Contact person : KiP

#ifndef I_CWPANEL_QUANTITY_CALCULATOR_HPP
#define I_CWPANEL_QUANTITY_CALCULATOR_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "IModelElementQuantityCalculator.hpp"

// from GSUtils
#include "GSUtilsDefs.h"

// =====================================================================================================================


namespace Property {

class PROPERTY_OPERATIONS_DLL_EXPORT ICWPanelQuantityCalculator : public IModelElementQuantityCalculator
{
public:
	virtual ~ICWPanelQuantityCalculator ();

	virtual GS::UniString					GetType					() const = 0;
	virtual UIndex							GetClass				() const = 0;
	virtual double							GetVertDirection		() const = 0;
	virtual double							GetHorDirection			() const = 0;
	virtual double							GetSurface				() const = 0;
	virtual double							GetGrossSurface			() const = 0;
	virtual double							GetPerimeter			() const = 0;
	virtual double							GetGrossPerimeter		() const = 0;
	virtual GS::Array<ADB::AttributeIndex>	GetMaterialsOuter		() const = 0;
	virtual GS::Array<ADB::AttributeIndex>	GetMaterialsInner		() const = 0;
	virtual GS::Array<ADB::AttributeIndex>	GetMaterialsCut			() const = 0;
	virtual short							GetFunction				() const = 0;
	virtual GS::UniString					GetOrientation			() const = 0;
};

}

#endif
