#ifndef OPENING_EXTRUSION_LIMITS_HPP
#define OPENING_EXTRUSION_LIMITS_HPP
#pragma once


#include "Plane3D.hpp"
#include "VBElementsBase/ModelElementTypes.hpp"
#include "OpeningTypes.hpp"
#include "OpeningInfiniteExtrusionData.hpp"


namespace Geometry { class Line3D; }


namespace VBEO {
namespace Opening {

class VB_ELEM_OPERATIONS_DLL_EXPORT ExtrusionLimits
{
public:
	static const ExtrusionLimits Empty;
	ExtrusionLimits (const Geometry::Plane3D& beginPlane, const Geometry::Plane3D& endPlane);

	bool IsEmpty () const;
	GS::Optional<Geometry::Plane3D> GetBeginPlane () const;
	GS::Optional<Geometry::Plane3D> GetEndPlane () const;

private:
	ExtrusionLimits ();
	GS::Optional<Geometry::Plane3D> beginPlane;
	GS::Optional<Geometry::Plane3D> endPlane;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT ElemLineIntersectionCalculator
{
public:
	static const ElemLineIntersectionCalculator& Default;

	virtual ~ElemLineIntersectionCalculator ();
	virtual GS::Array<Point3D> Calculate (const VBElem::ModelElementConstRef& elem, const Geometry::Line3D& line) const = 0;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT ExtrusionLimitCalculator
{
public:
	enum BasePlanePosition { BasePlaneMayBeOutsideOfLimits, BasePlaneMustBeBetweenLimits };

	class VB_ELEM_OPERATIONS_DLL_EXPORT InputGeometry
	{
	public:
		InputGeometry (const OpeningGeometry::InfiniteExtrusion& infiniteExtrusion,	const OpeningGeometry::ExtrusionLimitParameters& limitParameters);
		
		static InputGeometry Create (const VBElem::OpeningConstRef& opening);

	private:
		friend class ExtrusionLimitCalculator;

		Geometry::Polygon2D						BasePolygon		() const { return infiniteExtrusion.GetBasePolygon (); }
		UnitVector_3D							Direction		() const { return infiniteExtrusion.GetDirection (); }
		OpeningGeometry::ExtrusionBasePlane		BasePlane		() const { return infiniteExtrusion.GetBasePlane (); }
		OpeningGeometry::ExtrusionFrame			Frame			() const { return infiniteExtrusion.GetFrame (); }
		GS::Optional<double>					StartOffset		() const { return limitParameters.GetStartOffset (); }
		GS::Optional<double>					FiniteLength	() const { return limitParameters.GetFiniteLength (); }

		OpeningGeometry::InfiniteExtrusion infiniteExtrusion;
		OpeningGeometry::ExtrusionLimitParameters limitParameters;
	};	
	
	class VB_ELEM_OPERATIONS_DLL_EXPORT OverhangPolicy
	{
	public:
		static const OverhangPolicy ScreenOnly;
		static const OverhangPolicy OutsideOfElement;
		static const OverhangPolicy Interaction;

		OverhangPolicy (BasePlanePosition basePlanePosition, double overhang);

		bool	NeedIncludeBasePlane	() const { return basePlanePosition == BasePlaneMustBeBetweenLimits; }
		double	Overhang				() const { return overhang; }

	private:
		BasePlanePosition basePlanePosition;
		double overhang;
	};


	static ExtrusionLimits ForSureFullElemCutting (
		const VBElem::ModelElementConstRefList& cuttedElements,
		const InputGeometry& extrusionGeometry
	);

	static ExtrusionLimits ForApproximateMinimalFullElemCutting (
		const VBElem::ModelElementConstRefList& cuttedElements,
		const InputGeometry& extrusionGeometry,
		const OverhangPolicy& overhangPolicy,
		const ElemLineIntersectionCalculator& elemLineIntersections = ElemLineIntersectionCalculator::Default
	);
};

}
}

#endif
