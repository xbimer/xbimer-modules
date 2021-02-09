
#ifndef ColumnSEGMENTQUANTITY_HPP
#define ColumnSEGMENTQUANTITY_HPP

#pragma once

// === Includes ========================================================================================================

// from QuantityTakeoff
#include "VBModelElemQuantity.hpp"
#include "AssemblySegmentQuantity.hpp"

// from PropertyOperations
#include "IColumnSegmentQuantityCalculator.hpp"

#include "ColumnSegmentTypes.hpp"
// =====================================================================================================================

namespace QT {

class QUANTITYTAKEOFF_DLL_EXPORT ColumnSegmentQuantityCalculator : public Property::IColumnSegmentQuantityCalculator, protected AssemblySegmentQuantityCalculator
{
public:
	ColumnSegmentQuantityCalculator (const VBElem::ModelElementConstRef& vbElem, const QuantityModelConstPtr& quantityModel);
	virtual ~ColumnSegmentQuantityCalculator ();

	virtual double		GetSurfaceArea			(Int32 surfaceType) const override;
	virtual double		GetVeneerSurfaceArea	(Int32 surfaceType) const override;
	virtual double		GetCoreSurfaceArea		(Int32 surfaceType) const override;
	virtual double		GetTotalSurfaceArea		() const override;
	virtual double		GetExposedSurfaceArea   (const VBEO::ModelElemComponentSurfaceId& componentSurfaceId) const override;
	virtual double		GetGrossCoreSurfaceArea (Int32 surfaceType) const override;
	virtual double		GetGrossVeneerSurfaceArea (Int32 surfaceType) const override;

	virtual double		GetProjected3DLength			() const override;
	virtual double		GetProjectedFloorPlanArea		() const override;
	virtual double		GetCrossSectionArea				() const override;
	virtual double		GetBottomSurfaceProjectedArea	() const override;

	virtual	double		GetNetVolume () const override;
	virtual	double		GetGrossVolume () const override;
	virtual double		GetPerimeterOfBase () const override;
	virtual double		GetGrossVeneerVolume () const override;
	virtual double		GetGrossCoreVolume () const override;
	virtual double		GetCoreVolume () const override;
	virtual double		GetVeneerVolume () const override;

	virtual double		GetMinimumHeight () const override;
	virtual double		GetMaximumHeight () const override;

	virtual GS::HashSet<VBEO::ModelElemComponentId>			GetComponents					() const override;
	virtual ADB::AttributeIndex								GetComponentBuildingMaterialIdx (const VBEO::ModelElemComponentId& componentId) const override;
	virtual double											GetComponentVolume				(const VBEO::ModelElemComponentId& componentId) const override;
	virtual bool											HasComponentProjectedArea		(const VBEO::ModelElemComponentId& componentId) const override;
	virtual double											GetComponentProjectedArea		(const VBEO::ModelElemComponentId& componentId) const override;
	virtual GS::HashSet<ModelCalculations::SurfaceId>		GetVisibleSurfaces				() const override;
	virtual GS::HashSet<VBEO::ModelElemComponentSurfaceId>  GetComponentSurfaces			() const override;
	virtual double											GetHighestPoint					() const override;
	virtual double											GetLowestPoint					() const override;

private:
	VBElem::ColumnSegmentConstRef GetColumnSegment () const;

	virtual ModelCalculations::ProjectionSurfaceRef GetProjectionSurface (const VBEO::ModelElemComponentId& componentId) const override;
	virtual IPolygonSurfaceType*					GetPolygonSurfaceType () const override;

	void											CalculateElevations	(double& bottom, double& highestTopPoint, double& lowestTopPoint) const;

	double											GetSquareSegmentProjectedArea (const VBEO::ModelElemComponentId& componentId, const CONV2LISTREC& listParams = CreateListParams (DetailedFinalModel)) const;
};

}

#endif