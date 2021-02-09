// Contact person : KiP

#ifndef I_COLUMN_SEGMENT_QUANTITY_CALCULATOR_HPP
#define I_COLUMN_SEGMENT_QUANTITY_CALCULATOR_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "IAssemblySegmentQuantityCalculator.hpp"

// =====================================================================================================================

namespace Property {

class PROPERTY_OPERATIONS_DLL_EXPORT IColumnSegmentQuantityCalculator : public IAssemblySegmentQuantityCalculator
{
public:
	virtual ~IColumnSegmentQuantityCalculator ();

	virtual double		GetSurfaceArea (Int32 surfaceType) const = 0;
	virtual double		GetProjected3DLength () const = 0;
	virtual double		GetProjectedFloorPlanArea () const = 0;
	virtual double		GetCrossSectionArea () const = 0;
	virtual double		GetBottomSurfaceProjectedArea () const = 0;
	virtual double		GetPerimeterOfBase () const = 0;
	virtual double		GetMinimumHeight () const = 0;
	virtual double		GetMaximumHeight () const = 0;
	
	virtual double		GetGrossCoreSurfaceArea (Int32 surfaceType) const = 0;
	virtual double		GetGrossVeneerSurfaceArea (Int32 surfaceType) const = 0;
	virtual double		GetGrossVeneerVolume () const = 0;
	virtual double		GetGrossCoreVolume () const = 0;
	virtual double		GetCoreVolume () const = 0;
	virtual double		GetVeneerVolume () const = 0;

	virtual double		GetVeneerSurfaceArea (Int32 surfaceType) const = 0;
	virtual double		GetGrossVolume () const = 0;
};

}

#endif
