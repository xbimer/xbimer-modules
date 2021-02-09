
#ifndef BEAMSEGMENTQUANTITY_HPP
#define BEAMSEGMENTQUANTITY_HPP

#pragma once

// === Includes ========================================================================================================

// from QuantityTakeoff
#include "VBModelElemQuantity.hpp"
#include "AssemblySegmentQuantity.hpp"

// from PropertyOperations
#include "IBeamSegmentQuantityCalculator.hpp"

#include "BeamSegmentTypes.hpp"
// =====================================================================================================================

namespace ModelCalculations { enum class ElemSideType; }

namespace QT {

class QUANTITYTAKEOFF_DLL_EXPORT BeamSegmentQuantityCalculator : public Property::IBeamSegmentQuantityCalculator, protected AssemblySegmentQuantityCalculator
{
public:
	BeamSegmentQuantityCalculator (const VBElem::ModelElementConstRef& vbElem, const QuantityModelConstPtr& quantityModel);
	virtual ~BeamSegmentQuantityCalculator ();

	virtual double		GetSurfaceArea			(Int32 surfaceType) const override;
	virtual	double		GetBeginSideSurfaceArea () const override;
	virtual double		GetEndSideSurfaceArea	() const override;
	
	virtual double		GetCoreSurfaceArea		(Int32 surfaceType) const override;
	virtual double		GetTotalSurfaceArea		() const override;
	virtual double		GetExposedSurfaceArea	(const VBEO::ModelElemComponentSurfaceId& componentSurfaceId) const override;

	virtual double		GetLengthLeft	() const override;
	virtual double		GetLengthRight	() const override;

	virtual double		GetHolesSurface	() const override;

	virtual double		GetProjected3DLength	  () const override;
	virtual double		GetProjectedFloorPlanArea () const override;

	virtual double		GetConditionalVolume () const override;
	virtual	double		GetNetVolume		 () const override;
	
	virtual GS::HashSet<VBEO::ModelElemComponentId>			GetComponents () const override;
	virtual ADB::AttributeIndex								GetComponentBuildingMaterialIdx (const VBEO::ModelElemComponentId& componentId) const override;
	virtual double											GetComponentVolume (const VBEO::ModelElemComponentId& componentId) const override;
	virtual bool											HasComponentProjectedArea (const VBEO::ModelElemComponentId& componentId) const override;
	virtual double											GetComponentProjectedArea (const VBEO::ModelElemComponentId& componentId) const override;
	virtual GS::HashSet<ModelCalculations::SurfaceId>		GetVisibleSurfaces () const override;
	virtual GS::HashSet<VBEO::ModelElemComponentSurfaceId>  GetComponentSurfaces () const override;
	virtual double											GetHighestPoint () const override;
	virtual double											GetLowestPoint () const override;

private:
	enum BeamSegmentSide
	{
		Left,
		Right
	};

	double GetSideLength		(BeamSegmentSide side) const;
	double GetSideSurfaceArea	(const ModelCalculations::ElemSideType position) const;

	VBElem::BeamSegmentConstRef GetBeamSegment () const;

	virtual ModelCalculations::ProjectionSurfaceRef GetProjectionSurface (const VBEO::ModelElemComponentId& componentId) const override;
	virtual IPolygonSurfaceType* GetPolygonSurfaceType () const override;
};

}

#endif