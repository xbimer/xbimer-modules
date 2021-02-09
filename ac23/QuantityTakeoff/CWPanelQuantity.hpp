// Contact person : KiP

#ifndef CWPANEL_QUANTITY_HPP
#define CWPANEL_QUANTITY_HPP

#pragma once

// === Includes ========================================================================================================

// from PropertyOperations
#include "ICWPanelQuantityCalculator.hpp"

// from VBElements
#include "CWPanelTypes.hpp"

// from QuantityTakeoff
#include "VBModelElemQuantity.hpp"

// =====================================================================================================================

namespace QT {

class QUANTITYTAKEOFF_DLL_EXPORT_FOR_TEST CWPanelQuantityCalculator : public Property::ICWPanelQuantityCalculator, protected VBModelElemQuantityCalculator
{
public:
	CWPanelQuantityCalculator (VBElem::ModelElementConstRef vbElem, const QuantityModelConstPtr& quantityModel);
	virtual ~CWPanelQuantityCalculator ();

	virtual GS::UniString									GetType								() const override;
	virtual UIndex											GetClass							() const override;
	virtual double											GetVertDirection					() const override;
	virtual double											GetHorDirection						() const override;
	virtual double											GetSurface							() const override;
	virtual double											GetGrossSurface						() const override;
	virtual double											GetPerimeter						() const override;
	virtual double											GetGrossPerimeter					() const override;
	virtual GS::Array<ADB::AttributeIndex>					GetMaterialsOuter					() const override;
	virtual GS::Array<ADB::AttributeIndex>					GetMaterialsInner					() const override;
	virtual GS::Array<ADB::AttributeIndex>					GetMaterialsCut						() const override;
	virtual short											GetFunction							() const override;
	virtual GS::UniString									GetOrientation						() const override;

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
	virtual VBEO::ModelElemComponentId	GetComponentId (const Modeler::MeshBody& body) const override;

	void	CalcCWPanelData (const Int32*		globalsBits,
							 GS::uchar_t*		typeText,
							 UIndex*			panelClass,
							 double*			vertDirection,
							 double*			horDirection,
							 double*			surface,
							 double*			grossSurface,
							 double*			perimeter,
							 double*			grossPerimeter,
							 ADB::AttributeIndex*	material_outer,
							 ADB::AttributeIndex*	material_inner,
							 ADB::AttributeIndex*	material_cut,
							 short*				function,
							 GS::uchar_t*		orientationText) const;

	const VBElem::CWPanelConstRef&	GetCWPanel () const;
};

}

#endif
