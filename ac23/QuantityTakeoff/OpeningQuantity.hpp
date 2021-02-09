#ifndef OPENING_QUANTITY_HPP
#define OPENING_QUANTITY_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "IOpeningQuantityCalculator.hpp"

// from QuantityTakeoff
#include "VBModelElemQuantity.hpp"

// =====================================================================================================================

namespace QT {

class QUANTITYTAKEOFF_DLL_EXPORT_FOR_TEST OpeningQuantityCalculator : public Property::IOpeningQuantityCalculator, protected VBModelElemQuantityCalculator
{
public:
	OpeningQuantityCalculator (const VBElem::ModelElementConstRef& vbElem, const QuantityModelConstPtr& quantityModel);
	virtual ~OpeningQuantityCalculator ();

	virtual ModelCalculations::MinMaxPair					GetBodyFirstLastLimits () const override;
	virtual	double											GetThickness () const override;
	virtual bool											GetIfVisibile () const override;

	virtual	double											GetNetVolume () const override;
	virtual	double											GetTotalSurfaceArea () const override;
	virtual double											GetHighestPoint () const override;
	virtual double											GetLowestPoint () const override;

	virtual GS::HashSet<VBEO::ModelElemComponentId>			GetComponents () const override;
	virtual	ADB::AttributeIndex								GetComponentBuildingMaterialIdx (const VBEO::ModelElemComponentId& componentId) const override;
	virtual double											GetComponentVolume (const VBEO::ModelElemComponentId& componentId) const override;
	virtual	bool											HasComponentProjectedArea (const VBEO::ModelElemComponentId& componentId) const override;
	virtual	double											GetComponentProjectedArea (const VBEO::ModelElemComponentId& componentId) const override;

	virtual GS::HashSet<ModelCalculations::SurfaceId>		GetVisibleSurfaces () const override;
	virtual GS::HashSet<VBEO::ModelElemComponentSurfaceId>	GetComponentSurfaces () const override;
	virtual	double											GetExposedSurfaceArea (const VBEO::ModelElemComponentSurfaceId& componentSurfaceId) const override;

private:
	virtual VBEO::ModelElemComponentId						GetComponentId (const Modeler::MeshBody& body) const override;

	void													IterateInfillBodyAttributes (::std::function<void(const ModelCalculations::MeshBodyCalculator&)> functor, bool solidBody) const;
	const VBElem::OpeningConstRef&							GetOpening () const;

	ModelCalculations::MinMaxPair							GetVerticalExtremities () const;

};

}
#endif