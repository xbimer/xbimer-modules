#if !defined (CURVE2DDATA_H)
#define CURVE2DDATA_H

#pragma once
// Geometry internal!!!

#include	"GeometricDefinitions.h"
#include	"RealNumber.h"
#include	"AngleData.h"

#include	"Point2DData.h"
#include	"Vector2D.hpp"

#include	"Direction2D.hpp"
#include	"Sector2DData.h"
#include	"GenArc2DData.h"
#include	"BezierDetails.hpp"

namespace Geometry {

	enum Curve2DDataType {
		CURVE2D_NULL,		// The invalid state
		CURVE2D_Point,		// A single point (Coord)
		CURVE2D_Sector,		// A sector of a straight line
		CURVE2D_Ray,		// A straight line limited only at the first end, but unlimited to the other
		CURVE2D_Line,		// A straight line

		CURVE2D_Circle,		// circle
		CURVE2D_GenArc,		// Circle, Arc, Ellipse, Ellipse Arc

		CURVE2D_Spline,		// sm405 used by CutElems
		CURVE2D_PolyLine,	// VBElems::Polygondata.GeomData-like, considered as a curve (even if closed)
		CURVE2D_2DPolygon	// Geometry-like (Polygon2DData), considered as a shpape
	};

	struct Curve2DData {
		Curve2DDataType	type;
		double			epsilon;
		double			angleEpsilon;

		// only one of them valid - was union
		Coord	point;
		Sector	sector;
		GenArc	genArc;

		BezierDetails spline;
	};

	inline void		InitCurve2DData (Curve2DData* curve, double eps = Eps, double angEps = RadEps);

	GEOMETRY_DLL_EXPORT void GEOM_CALL InitCurve2DData (Curve2DData* curve, const Coord&  point, double eps = Eps, double angEps = RadEps);
	GEOMETRY_DLL_EXPORT void GEOM_CALL InitCurve2DData (Curve2DData* curve, const Sector& sector, Curve2DDataType type, double eps = Eps, double angEps = RadEps);
	GEOMETRY_DLL_EXPORT void GEOM_CALL InitCurve2DData (Curve2DData* curve, const Circle& circle, double eps = Eps, double angEps = RadEps);
	GEOMETRY_DLL_EXPORT void GEOM_CALL InitCurve2DData (Curve2DData* curve, const GenArc& genArc, double eps = Eps, double angEps = RadEps);
	GEOMETRY_DLL_EXPORT void GEOM_CALL InitCurve2DData (Curve2DData* curve, const BezierDetails& spline, double eps = Eps, double angEps = RadEps);

	inline bool		IsCurve2DDataStraight (const Curve2DData& curve);

	inline Coord	GetCurve2DDataBegPosition (const Curve2DData& curve);
	inline Coord	GetCurve2DDataEndPosition (const Curve2DData& curve);

	inline Box2DData	GetCurve2DDataBoundingBox (const Curve2DData& curve);

	GEOMETRY_DLL_EXPORT Direction2D	GEOM_CALL GetCurve2DDataBegDirection (const Curve2DData& curve);
	GEOMETRY_DLL_EXPORT Direction2D	GEOM_CALL GetCurve2DDataEndDirection (const Curve2DData& curve);

	GEOMETRY_DLL_EXPORT bool GEOM_CALL OffsetCurve2DData (
				const Curve2DData&	curveIn,
				double				offset,
				Curve2DData*		curveOut,
				Coord*				refPt = nullptr);	/* [in, out], can be nullptr */

	GEOMETRY_DLL_EXPORT GSErrCode GEOM_CALL GetMaxOffsetValue (
				const Curve2DData&	curve1In,
				const Curve2DData&	curve2In,
				double				triedOffset,
				double*				maxOffset);

	GEOMETRY_DLL_EXPORT void GEOM_CALL		LimitOffsetForArc (
						const Curve2DData&	prevEdgeCurve,
						const Curve2DData&	nextEdgeCurve,
						const Curve2DData&	originalCurve,
						double				offset,
						double*				maxBegOffset,
						double*				maxEndOffset);

	GEOMETRY_DLL_EXPORT double	GEOM_CALL		LimitOffsetToXNeigEdges (
						const Curve2DData&	begXEdgeCurve,
						const Curve2DData&	endXEdgeCurve,
						const Curve2DData&	originalCurve,
						double				offset);

	/* return value: number of valid intersections */
	GEOMETRY_DLL_EXPORT Int32 GEOM_CALL IntersectCurve2DData (const Curve2DData& curve1, const Curve2DData& curve2, Coord result[4]);

	enum GetGivenDistantPointResult {
		DistantPointError,
		DistantPointNoIntersection,
		DistantPointSuccess
	};
	GEOMETRY_DLL_EXPORT GetGivenDistantPointResult GEOM_CALL GetGivenDistantPoint (					/* RJ003 */
																const Curve2DData&	curvePrev,
																double				offset1,
																const Curve2DData&	curveNext,
																double				offset2,
																const Coord&		refPt,
																Coord*				result,
																Curve2DData*		offsetedCurve1 = nullptr,
																Curve2DData*		offsetedCurve2 = nullptr);

	typedef const std::function<void (const Coord&)>& CoordResultFn;

	GEOMETRY_DLL_EXPORT void GEOM_CALL IntersectCurve2DData (const Curve2DData& curve1, const Curve2DData& curve2, CoordResultFn resultCallback);
	GEOMETRY_DLL_EXPORT void GEOM_CALL IntersectCurve2DData (const Curve2DData& curve1, const Curve2DData& curve2, GS::Array<Coord>& result);

	GEOMETRY_DLL_EXPORT void GEOM_CALL ExtendCurveLine		(Curve2DData& curve);

	GEOMETRY_DLL_EXPORT Curve2DData GEOM_CALL ToCurve2DData (const GenArc& arc);
	GEOMETRY_DLL_EXPORT Curve2DData GEOM_CALL ToCurve2DDataLine (const Sector& s);
	GEOMETRY_DLL_EXPORT Curve2DData GEOM_CALL ToCurve2DData (const BezierDetails& bez);

	GEOMETRY_DLL_EXPORT	Curve2DData GEOM_CALL Line (const Coord& a, double alfa);
	GEOMETRY_DLL_EXPORT	Curve2DData GEOM_CALL Line (const Coord& a, const Coord& b);

} // namespace Geometry

inline void Geometry::InitCurve2DData (Curve2DData* curve, double eps, double angEps)
{
	BNZeroMemory (&curve->point, sizeof (Coord));
	BNZeroMemory (&curve->sector, sizeof (Sector));
	BNZeroMemory (&curve->genArc, sizeof (GenArc));
	// don't BNZeroMemory the BezierDetails!

	curve->type			= CURVE2D_NULL;
	curve->epsilon		= eps;
	curve->angleEpsilon = angEps;
}


inline bool Geometry::IsCurve2DDataStraight (const Curve2DData& curve)
{
	return (curve.type != CURVE2D_Circle && curve.type != CURVE2D_GenArc);
}


inline Coord Geometry::GetCurve2DDataBegPosition (const Curve2DData& curve)
{
	Coord	def (0.0, 0.0);

	switch (curve.type) {
		case CURVE2D_NULL:
			DBBREAK_STR ("A null curve has no beg position!");
			return def;

		case CURVE2D_Point:
			return curve.point;

		case CURVE2D_Sector:
			return curve.sector.c1;

		case CURVE2D_Ray:
			return curve.sector.c1;

		case CURVE2D_Line:
			return curve.sector.c1;

		case CURVE2D_Circle:
			DBBREAK_STR ("A circle has no beg position!");
			return def;

		case CURVE2D_GenArc:
			return curve.genArc.GetBegC ();

		default:
			DBBREAK_STR ("Unknown curve type in GetCurve2DDataBegPosition!");
			return def;
	}
}

inline Coord	Geometry::GetCurve2DDataEndPosition (const Curve2DData& curve)
{
	Coord	def (0.0, 0.0);

	switch (curve.type) {
		case CURVE2D_NULL:
			DBBREAK_STR ("A null curve has no end position!");
			return def;

		case CURVE2D_Point:
			return curve.point;

		case CURVE2D_Sector:
			return curve.sector.c2;

		case CURVE2D_Ray:
			return curve.sector.c2;

		case CURVE2D_Line:
			return curve.sector.c2;

		case CURVE2D_Circle:
			DBBREAK_STR ("A circle has no end position!");
			return def;

		case CURVE2D_GenArc:
			return curve.genArc.GetEndC ();

		default:
			DBBREAK_STR ("Unknown curve type in GetCurve2DDataEndPosition!");
			return def;
	}
}

inline Box2DData	Geometry::GetCurve2DDataBoundingBox (const Curve2DData& curve)
{
	Box2DData	box;

	InitBox (&box);

	switch (curve.type) {
		case CURVE2D_NULL:
			return box;

		case CURVE2D_Point:
			CoordToBox (curve.point, &box);
			return box;

		case CURVE2D_Sector:
			return SectorBox (&curve.sector);

		case CURVE2D_Ray:
			DBBREAK_STR ("A ray has no bounding box!");
			return box;

		case CURVE2D_Line:
			DBBREAK_STR ("A ray has no bounding box!");
			return box;

		case CURVE2D_Circle:
			//DBBREAK_STR ("A circle has no end position!");
			box.xMax = curve.genArc.GetOrigo ().x + curve.genArc.GetMainAxisLength ();
			box.xMin = curve.genArc.GetOrigo ().x - curve.genArc.GetMainAxisLength ();
			box.yMax = curve.genArc.GetOrigo ().y + curve.genArc.GetMainAxisLength ();
			box.yMin = curve.genArc.GetOrigo ().y - curve.genArc.GetMainAxisLength ();
			return box;

		case CURVE2D_GenArc:
			return GetGenArcBoundBox (curve.genArc);

		default:
			DBBREAK_STR ("Unknown curve type in GetCurve2DDataBoundingBox!");
			return box;
	}
}



#endif
