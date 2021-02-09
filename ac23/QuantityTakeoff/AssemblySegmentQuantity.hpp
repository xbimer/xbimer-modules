
#ifndef ASSEMBLYSEGMENT_QUANTITY_HPP
#define ASSEMBLYSEGMENT_QUANTITY_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "IAssemblySegmentQuantityCalculator.hpp"

// from QuantityTakeoff
#include "VBModelElemQuantity.hpp"


#include "AssemblySegmentTypes.hpp"
// =====================================================================================================================

namespace QT {

class QUANTITYTAKEOFF_DLL_EXPORT_FOR_TEST AssemblySegmentQuantityCalculator : public Property::IAssemblySegmentQuantityCalculator, protected VBModelElemQuantityCalculator
{
public:
	AssemblySegmentQuantityCalculator (const VBElem::ModelElementConstRef& vbElem, const QuantityModelConstPtr& quantityModel);
	virtual ~AssemblySegmentQuantityCalculator ();

			double											GetGrossVolume () const;
	virtual double											GetNetVolume						() const override;
	virtual double											GetTotalSurfaceArea					() const override;
	virtual double											GetHighestPoint						() const override;
	virtual double											GetLowestPoint						() const override;

	virtual GS::HashSet<VBEO::ModelElemComponentId>			GetComponents						() const override;
	virtual	ADB::AttributeIndex								GetComponentBuildingMaterialIdx		(const VBEO::ModelElemComponentId& componentId) const override;
	virtual double											GetComponentVolume					(const VBEO::ModelElemComponentId& componentId) const override;
	virtual	bool											HasComponentProjectedArea			(const VBEO::ModelElemComponentId& componentId) const override;
	virtual	double											GetComponentProjectedArea			(const VBEO::ModelElemComponentId& componentId) const override;

	virtual double GetCoreSurfaceArea (Int32 surfaceType) const override;

	virtual GS::HashSet<ModelCalculations::SurfaceId>		GetVisibleSurfaces					() const override;
	virtual GS::HashSet<VBEO::ModelElemComponentSurfaceId>	GetComponentSurfaces				() const override;
	virtual	double											GetExposedSurfaceArea				(const VBEO::ModelElemComponentSurfaceId& componentSurfaceId) const override;

protected:
	static CONV2LISTREC		GetNormalModelListParams ();
	static CONV2LISTREC		GetGrossModelListParams ();
	static CONV2LISTREC		GetCoreModelListParams ();
	static CONV2LISTREC		GetGrossCoreModelListParams ();
	static CONV2LISTREC		GetDetailedModelListParams ();

	virtual VBEO::ModelElemComponentId				GetComponentId (const Modeler::MeshBody& body) const override;

private:
	const VBElem::AssemblySegmentConstRef&			GetAssemblySegment () const;
};

}

#endif