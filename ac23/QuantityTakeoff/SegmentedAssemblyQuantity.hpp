// Contact person : KiP

#ifndef SEGMENTEDASSEMBLY_QUANTITY_HPP
#define SEGMENTEDASSEMBLY_QUANTITY_HPP

#pragma once

// === Includes ========================================================================================================

// from QuantityTakeoff
#include "VBModelElemQuantity.hpp"

#include "ProfileComposite.hpp"
#include "AssemblySegmentTypes.hpp"

// =====================================================================================================================

namespace QT {

class QUANTITYTAKEOFF_DLL_EXPORT_FOR_TEST SegmentedAssemblyQuantityCalculator : protected VBModelElemQuantityCalculator
{
public:
	SegmentedAssemblyQuantityCalculator (const VBElem::ModelElementConstRef& vbElem, const QuantityModelConstPtr& quantityModel);
	virtual ~SegmentedAssemblyQuantityCalculator ();

	double											GetNetVolume					 () const;
	double											GetTotalSurfaceArea				 () const;

	double											GetHighestPoint					 () const;
	double											GetLowestPoint					 () const;

	double											GetSurfaceArea (Int32 surfaceType) const;
	double											GetProjected3DLength			 () const;
	double											GetProjectedFloorPlanArea		 () const;
																					 
	GS::HashSet<VBEO::ModelElemComponentId>			GetComponents					 () const;
	ADB::AttributeIndex								GetComponentBuildingMaterialIdx	 (const VBEO::ModelElemComponentId& componentId) const;
	double											GetComponentVolume				 (const VBEO::ModelElemComponentId& componentId) const;
	bool											HasComponentProjectedArea		 (const VBEO::ModelElemComponentId& componentId) const;
	double											GetComponentProjectedArea		 (const VBEO::ModelElemComponentId& componentId) const;
																					 
	GS::HashSet<ModelCalculations::SurfaceId>		GetVisibleSurfaces				 () const;
	GS::HashSet<VBEO::ModelElemComponentSurfaceId>	GetComponentSurfaces			 () const;
	double											GetExposedSurfaceArea			 (const VBEO::ModelElemComponentSurfaceId& componentSurfaceId) const;

protected:
	virtual ModelCalculations::ProjectionCurveRef			GetProjectionCurve () const = 0;

private:
	virtual void EnumerateModelElemSubElements (const std::function<void (const VBElem::ModelElementConstRef&)>& processor) const override;
	VBElem::SegmentedAssemblyConstRef		GetSegmentedAssembly () const;
	
	static GS::Array<Int32> GetAllSurfaceTypes ();
	static CONV2LISTREC		GetNormalModelListParams ();

	GS::Ref<Property::IModelElementQuantityCalculator>	GetQuantityCalculator (const VBElem::AssemblySegmentConstRef& assemblySegment) const;

	virtual VBEO::ModelElemComponentId					GetComponentId			(const Modeler::MeshBody& body) const override;
};

}

#endif