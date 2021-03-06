// Contact person : NN

#ifndef RAILINGSEGMENTQUANTITY_HPP
#define RAILINGSEGMENTQUANTITY_HPP

#pragma once

// === Includes ========================================================================================================

// from VBElements
#include "VBElements/RailingSegmentTypes.hpp"
#include "VBElements/RailingSegment.hpp"

// from PropertyOperations
#include "IRailingSegmentQuantityCalculator.hpp"

// from QuantityTakeoff
#include "VBModelElemQuantity.hpp"

// =====================================================================================================================

namespace QT {

class QUANTITYTAKEOFF_DLL_EXPORT_FOR_TEST RailingSegmentQuantityCalculator : public Property::IRailingSegmentQuantityCalculator, protected VBModelElemQuantityCalculator
{
public:
	RailingSegmentQuantityCalculator (const VBElem::ModelElementConstRef& vbElem, const QuantityModelConstPtr& quantityModel);
	virtual ~RailingSegmentQuantityCalculator ();

	virtual double											GetNetVolume						() const override;
	virtual double											GetTotalSurfaceArea					() const override;
	virtual double											GetHighestPoint						() const override;
	virtual double											GetLowestPoint						() const override;

	virtual GS::HashSet<VBEO::ModelElemComponentId>			GetComponents						() const override;
	virtual	ADB::AttributeIndex								GetComponentBuildingMaterialIdx		(const VBEO::ModelElemComponentId& componentId) const override;
	virtual double											GetComponentVolume					(const VBEO::ModelElemComponentId& componentId) const override;
	virtual	bool											HasComponentProjectedArea			(const VBEO::ModelElemComponentId& componentId) const override;
	virtual	double											GetComponentProjectedArea			(const VBEO::ModelElemComponentId& componentId) const override;

	virtual GS::HashSet<ModelCalculations::SurfaceId>		GetVisibleSurfaces					() const override;
	virtual GS::HashSet<VBEO::ModelElemComponentSurfaceId>	GetComponentSurfaces				() const override;
	virtual	double											GetExposedSurfaceArea				(const VBEO::ModelElemComponentSurfaceId& componentSurfaceId) const override;

private:
	virtual VBEO::ModelElemComponentId GetComponentId (const Modeler::MeshBody& body) const override;
	virtual void EnumerateModelElemSubElements (const std::function<void (const VBElem::ModelElementConstRef&)>& processor) const override;
	ModelCalculations::MinMaxPair GetVerticalExtremities () const;

	const VBElem::RailingSegmentConstRef& GetRailingSegment () const;
};

} //namespace QT

#endif