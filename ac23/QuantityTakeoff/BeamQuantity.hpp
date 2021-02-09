// Contact person : KiP

#ifndef BEAM_QUANTITY_HPP
#define BEAM_QUANTITY_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "IBeamQuantityCalculator.hpp"

// from QuantityTakeoff
#include "VBModelElemQuantity.hpp"
#include "SegmentedAssemblyQuantity.hpp"

#include "ProfileComposite.hpp"
#include "AssemblySegmentTypes.hpp"

// =====================================================================================================================

namespace QT {

class QUANTITYTAKEOFF_DLL_EXPORT_FOR_TEST BeamQuantityCalculator : public Property::IBeamQuantityCalculator, protected SegmentedAssemblyQuantityCalculator
{
public:
	BeamQuantityCalculator (const VBElem::ModelElementConstRef& vbElem, const QuantityModelConstPtr& quantityModel);
	virtual ~BeamQuantityCalculator ();

	virtual	double											GetNetVolume						() const override;
	virtual double											GetTotalSurfaceArea					() const override;
	virtual double											GetHighestPoint						() const override;
	virtual double											GetLowestPoint						() const override;
	virtual double											GetSurfaceArea						(Int32 surfaceType) const override;
	virtual	double											GetBeginSideSurfaceArea				() const override;
	virtual double											GetEndSideSurfaceArea				() const override;
	virtual double											GetLengthLeft						() const override;
	virtual double											GetLengthRight						() const override;
	virtual double											GetProjected3DLength				() const override;
	virtual double											GetProjectedFloorPlanArea			() const override;
	virtual double											GetHolesSurface						() const override; // it makes sense only for basic beams
	virtual double											GetConditionalVolume				() const override;

	virtual GS::HashSet<VBEO::ModelElemComponentId>			GetComponents						() const override;
	virtual	ADB::AttributeIndex								GetComponentBuildingMaterialIdx		(const VBEO::ModelElemComponentId& componentId) const override;
	virtual double											GetComponentVolume					(const VBEO::ModelElemComponentId& componentId) const override;
	virtual	bool											HasComponentProjectedArea			(const VBEO::ModelElemComponentId& componentId) const override;
	virtual	double											GetComponentProjectedArea			(const VBEO::ModelElemComponentId& componentId) const override;

	virtual GS::HashSet<ModelCalculations::SurfaceId>		GetVisibleSurfaces					() const override;
	virtual GS::HashSet<VBEO::ModelElemComponentSurfaceId>	GetComponentSurfaces				() const override;
	virtual	double											GetExposedSurfaceArea				(const VBEO::ModelElemComponentSurfaceId& componentSurfaceId) const override;
	
private:
	virtual void EnumerateModelElemSubElements (const std::function<void (const VBElem::ModelElementConstRef&)>& processor) const override;
	VBElem::BeamConstRef GetBeam () const;
	
	virtual ModelCalculations::ProjectionCurveRef	GetProjectionCurve () const override;

	virtual	ModelCalculations::ProjectionSurfaceRef		GetProjectionSurface	(const VBEO::ModelElemComponentId& componentId) const override;
	virtual	IPolygonSurfaceType*						GetPolygonSurfaceType	() const override;

	enum BeamSide
	{
		Left,
		Right
	};

	double		GetSideLength (BeamSide side) const;

	virtual CONV2LISTREC	GetWithHoleListParams () const override;
	virtual CONV2LISTREC	GetWithoutHoleListParams () const override;
};

}

#endif