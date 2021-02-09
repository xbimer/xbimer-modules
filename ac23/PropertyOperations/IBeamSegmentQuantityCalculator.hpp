// Contact person : KiP

#ifndef I_BEAM_SEGMENT_QUANTITY_CALCULATOR_HPP
#define I_BEAM_SEGMENT_QUANTITY_CALCULATOR_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "IAssemblySegmentQuantityCalculator.hpp"

// =====================================================================================================================

namespace Property {

class PROPERTY_OPERATIONS_DLL_EXPORT IBeamSegmentQuantityCalculator : public IAssemblySegmentQuantityCalculator
{
public:
	virtual ~IBeamSegmentQuantityCalculator (); 

	virtual double		GetSurfaceArea			  (Int32 surfaceType) const = 0;
	virtual	double		GetBeginSideSurfaceArea	  () const = 0;
	virtual double		GetEndSideSurfaceArea	  () const = 0;
	virtual double		GetLengthLeft			  () const = 0;
	virtual double		GetLengthRight			  () const = 0;
	virtual double		GetProjected3DLength	  () const = 0;
	virtual double		GetProjectedFloorPlanArea () const = 0;
	virtual double		GetConditionalVolume	  () const = 0;
	virtual double		GetHolesSurface			  () const = 0;
};

}

#endif
