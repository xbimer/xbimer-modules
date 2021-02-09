#ifndef OPENING_OPERATIONS_HPP
#define OPENING_OPERATIONS_HPP
#pragma once

#include "OpeningTypes.hpp"
#include "VBElemOperationsExport.hpp"
#include "Poly2DTypes.hpp"
#include "OpeningConstructionParameters.hpp"
#include "OpeningDefault.hpp"
#include "CreateOpeningsFromSelectionParameters.hpp"
#include "OpeningExtrusionLimits.hpp"

namespace OpeningGeometry {
	class ExtrusionFrame;
	class ExtrusionParameters;
}


namespace VBEO {

class VB_ELEM_OPERATIONS_DLL_EXPORT OpeningOperations
{
public:
	static Box3D GetBoundsOfOperatorModel (const VBElem::OpeningConstRef& opening, const Opening::ExtrusionLimitCalculator::OverhangPolicy& overhangPolicy);
	static Box3D  GetBoundsAbovePolygon (const VBElem::OpeningConstRef& opening, const Geometry::Polygon2D& polygon);
	static Box3D  GetBoundsAboveMultiPolygon (const VBElem::OpeningConstRef& opening, const Geometry::MultiPolygon2D& multiPoly);


	static bool					HasSillAndHeaderHeight				(const VBElem::OpeningConstRef& opening);
	static bool					HasSillAndHeaderHeight				(const VBElem::OpeningDefault* defOpening);
	static GS::Optional<double> CalcSillHeightFromOpening			(const VBElem::OpeningConstRef& opening);
	static GS::Optional<double> CalcHeaderHeightFromOpening			(const VBElem::OpeningConstRef& opening);
	static GS::Optional<double> CalcSillHeightFromOpeningDefault	(const VBElem::OpeningDefault* defOpening);
	static GS::Optional<double> CalcHeaderHeightFromOpeningDefault	(const VBElem::OpeningDefault* defOpening);
	static GS::Optional<GS::Pair<GS::Pair<double, double>, Vector2D>>   // <<width, height>, xAxis>
		                        CalcRectangleParametersFromOpening  (const VBElem::OpeningConstRef& opening);
	static bool                 NormalizeRectPoligonalOpening       (const VBElem::OpeningRef& opening);
	static void					FlipOpenings (const GS::HashSet<VBElem::OpeningRef>& openingsToFlip);
	static bool					CanExistOpeningFillWithOpeningAttributes (const VBElem::OpeningConstRef& opening);
	static void					FlipOpeningIfHasNoIntersection		(const VBElem::OpeningRef& opening, const VBElem::OpeningDefaultFieldMask& mask);

	static OpeningGeometry::ExtrusionFrame MoveExtrusionFrameToNewPositionByRotations (
		const OpeningGeometry::ExtrusionFrame& originalFrame, 
		const Point3D& newOrigo,
		const UnitVector_3D& newExtrusionDirection);


	class VB_ELEM_OPERATIONS_DLL_EXPORT ClosestPoint	//#KiP_A180_NiceToHave ennek hol lenne a helye?
	{
	public:
		Point2D			Get () const;
		UnitVector_2D	GetNormal () const;

		static ClosestPoint GetOnArc (const GenArc& arc, const Point2D& externalPoint);
		static GS::Optional<ClosestPoint> GetOnSector (const Sector& sector, const Point2D& externalPoint);
		static ClosestPoint GetOnPolygon (const Geometry::IrregularPolygon2D& polygon, const Point2D& externalPoint);

	private:
		ClosestPoint () : point (), normal (UnitVector_2D::XAxis) {}
		ClosestPoint (const Point2D point, const UnitVector_2D& normal) : point (point), normal (normal) {}

		Point2D point;
		UnitVector_2D normal;
	};
};


class VB_ELEM_OPERATIONS_DLL_EXPORT VertexArrayToOpeningParametersConverter
{
public:
	struct Result
	{
		VBElem::OpeningConstructionParameters	constructionParameters;
		OpeningGeometry::ExtrusionParameters	extrusionParameters;

		Result (const OpeningGeometry::ExtrusionParameters&		inExtrusionParameters,
				const VBElem::OpeningConstructionParameters&	inConstructionParameters);
	};

private:
	Coord3D GetWeightCenterOfVertices ();
	GS::Array<Coord> GetProjectedVerticesOnPlane (const Geometry::Plane& plane);
	Coord3D CalculateExtrusionCenter ();

public:
	VertexArrayToOpeningParametersConverter (
		const GS::Array<Coord3D>&						inVertices,
		const OpeningGeometry::ExtrusionParameters&		inTemplateExtrusionParameters,
		const VBElem::ModelElementRef&					inTargetElem,
		const double&									inViewAngle);
	Result Execute (const Opening::CreateOpeningsFromSelectionParameters& params);

private:
	const GS::Array<Coord3D>& vertices;
	const OpeningGeometry::ExtrusionParameters& templateExtrusionParameters;
	VBElem::ModelElementRef targetElem;
	const double viewAngle;
};

}

#endif
