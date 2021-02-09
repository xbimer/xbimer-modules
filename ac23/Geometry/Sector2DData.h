/****************************************************************************/
/*																			*/
/*					2D Line, Sector and their operations					*/
/*																			*/
/****************************************************************************/
#if !defined (SECTOR2DDATA_H)
#define SECTOR2DDATA_H

#pragma once

// from Geometry
#include "GeometricDefinitions.h"
#include "Point2DData.h"

namespace GS { template<typename T, UInt32> class PagedArray; }


struct F_Sector {
	Geometry::Point2f	c1;
	Geometry::Point2f	c2;
};

struct GEOMETRY_DLL_EXPORT Sector {
	Point2D	c1;
	Point2D	c2;

	Sector () : c1 (Point2D (0.0, 0.0)), c2 (Point2D (0.0, 0.0)) {}
	Sector (const Point2D& inC1, const Point2D& inC2) : c1 (inC1), c2 (inC2) {}

public:
	inline double	GetLength () const;
};

GEOMETRY_DLL_EXPORT GSErrCode			Read (GS::IChannel& ic, Sector& sector);
GEOMETRY_DLL_EXPORT GSErrCode			Write (GS::OChannel& oc, const Sector& sector);
GEOMETRY_DLL_EXPORT GSErrCode			Store (GS::ObjectState& os, const Sector& sector);
GEOMETRY_DLL_EXPORT GSErrCode			Restore (const GS::ObjectState& os, Sector& sector);


namespace Geometry {

	typedef const std::function<void (const Sector&)>& SectorResultFn;

	enum DistType {
		POS_DIST,		// Positive distant
		SIGN_DIST		// Signed distant
	};

	GEOMETRY_DLL_EXPORT Vector2D	SectorVector (const Sector& sct);

	GEOMETRY_DLL_EXPORT void   InvertSectorOrientation (Sector& sct);

	GEOMETRY_DLL_EXPORT bool   IsLineCoord (const Point2D& pt, const Sector& sct);

	GEOMETRY_DLL_EXPORT Sector OffsetSector (const Sector& sector, const Vector2D& offset);

	GEOMETRY_DLL_EXPORT bool   InternalCoord (
							const Sector&			sector,
							const Point2D&			point,
							Boundary		sectorBegEnd,
							double					eps = Eps);

	inline				double DistLineCoord (const Sector& ln, const Point2D& pt);

	GEOMETRY_DLL_EXPORT double DistCoordSector (const Point2D& p, const Sector& s, DistType distType = POS_DIST);

	GEOMETRY_DLL_EXPORT double DistOrigoSector (const Sector& s, DistType distType = POS_DIST);

	GEOMETRY_DLL_EXPORT Point2D  CoordProjSector (const Point2D& c, const Sector& s);

	GEOMETRY_DLL_EXPORT double CoordProjSectorParam (const Point2D& c, const Sector& s);

	GEOMETRY_DLL_EXPORT Point2D  MirrorCoord (const Sector&	s, const Point2D&	wc);

	GEOMETRY_DLL_EXPORT void   CalcWithSector (Sector& s, double width);

	GEOMETRY_DLL_EXPORT bool   SectorContainsSector (const Sector& sector, const Sector& otherSector, double eps = Eps);

	GEOMETRY_DLL_EXPORT bool	IsSectorPointIntersect (
							const Sector&	s,
							const Point2D&	pt,
							Boundary		sectorBegBoundary,
							Boundary		sectorEndBoundary,
							double			eps);

	GEOMETRY_DLL_EXPORT bool	XLines (
											const Sector&	s1,
											const Sector&	s2,
											Point2D*			xc,
											double*			xt1 = nullptr,
											double*			xt2 = nullptr);

	GEOMETRY_DLL_EXPORT bool	XLinesEps  (
							const Sector&	s1,
							const Sector&	s2,
							Point2D			*xc,
							double			eps,
							double			radEps,
							double*			xt1 = nullptr,
							double*			xt2 = nullptr);		/* mi46 */

	GEOMETRY_DLL_EXPORT bool	XRayLineEps (
							const Sector&	s1,
							const Sector&	s2,
							Boundary		rayBoundaries,
							Point2D			*xc,
							double			eps,
							double			radEps,
							double*			xt1 = nullptr,
							double*			xt2 = nullptr);

	GEOMETRY_DLL_EXPORT bool	XSectorLineEps (
							const Sector&	s1,
							const Sector&	s2,
							Boundary		sectorBoundaries,
							Point2D			*xc,
							double			eps,
							double			radEps,
							double*			xt1 = nullptr,
							double*			xt2 = nullptr);

	GEOMETRY_DLL_EXPORT bool	XSectorRayEps (
							const Sector&	s1,
							const Sector&	s2,
							Boundary		boundaries,
							Point2D			*xc,
							double			eps,
							double			radEps,
							double*			xt1 = nullptr,
							double*			xt2 = nullptr);

	GEOMETRY_DLL_EXPORT bool	XRaysEps (
							const Sector&	s1,
							const Sector&	s2,
							Boundary		rayBoundaries,
							Point2D*			xc,
							double			eps,
							double			radEps,
							double*			xt1 = nullptr,
							double*			xt2 = nullptr);

	GEOMETRY_DLL_EXPORT bool	XSectors (
							const Sector&	s1,
							const Sector&	s2,
							Boundary		sectorBoundaries,
							Point2D*			xc,
							double*			xt1 = nullptr,
							double*			xt2 = nullptr);

	GEOMETRY_DLL_EXPORT bool	XSectorsEps (
							const Sector&	s1,
							const Sector&	s2,
							Boundary		sectorBoundaries,
							Point2D*			xc,
							double			eps,
							double			radEps,
							double*			xt1 = nullptr,
							double*			xt2 = nullptr);

	GEOMETRY_DLL_EXPORT bool	XSectorsOverLapByDist  (
							const Sector&	s1,
							const Sector&	s2,
							Sector*			xs = nullptr,
							double			epsIn = Eps);

	GEOMETRY_DLL_EXPORT Int32	XSectorsOverLap (
							const Sector*	s1,
							const Sector*	s2,
							Point2D*			xc);

	GEOMETRY_DLL_EXPORT void	CutSectorWithOtherSector (
							const Sector&		target,
							const Sector&		cutter,
							Sector&				intersection,
							GS::Array<Sector>&	remainders,
							double				epsIn = Eps);

	GEOMETRY_DLL_EXPORT short	PGSegmInt (
							const Point2D*	pb,
							const Point2D*	pe,
							const Point2D*	qb,
							const Point2D*	qe,
							Point2D*			qint,
							double			invlenp,
							double			invlenq,
							double			eps);

	 GEOMETRY_DLL_EXPORT void	SplitSectorsByLinesEps (
							const GS::PagedArray<Sector>&	sectors,
							const GS::PagedArray<Sector>&	lines,
							Boundary						sectorBoundaries,
							GS::PagedArray<Sector>*			resSectors,
							double							eps,
							double							radEps);

	 GEOMETRY_DLL_EXPORT	bool	IsHorizontalSector (const Sector& sector, double eps = EPS);
	 GEOMETRY_DLL_EXPORT	bool	IsVerticalSector (const Sector& sector, double eps = EPS);

	GEOMETRY_DLL_EXPORT bool	IsPointInPositiveSide (const Point2D& p, const Sector& s);
	GEOMETRY_DLL_EXPORT bool	IsPointInNegativeSide (const Point2D& p, const Sector& s);
	GEOMETRY_DLL_EXPORT bool	ArePointsInSameSide (const Sector& s, const Point2D& p1, const Point2D& p2);
	GEOMETRY_DLL_EXPORT GSErrCode	ReadXMLSector	(GS::XMLIChannel& ic, Sector& sec);
	GEOMETRY_DLL_EXPORT GSErrCode	WriteXMLSector	(GS::XMLOChannel& oc, const Sector& sec);
	inline GSErrCode	ReadXML (GS::XMLIChannel& ic, Sector& sec)	{ return ReadXMLSector (ic, sec); }
	inline GSErrCode	WriteXML (GS::XMLOChannel& oc, const Sector& sec)	{ return WriteXMLSector (oc, sec); }

	GEOMETRY_DLL_EXPORT double SpecDistPointToSector (const Point2D& c, const Sector& s, double* overHang = nullptr);

	GEOMETRY_DLL_EXPORT void GetExtremePointsOfSectors (const GS::Array<Sector> sectors,
														const Vector2D& direction,
														GS::Array<Point2D>& minima,
														GS::Array<Point2D>& maxima);



	inline bool IsOrientationReversed(const Sector& sect)
	{
		return IsLessByXY(sect.c2, sect.c1);
	}
	inline Coord GetMinEndC(const Sector& sect)
	{
		return MINByXY(sect.c1, sect.c2);
	}
	inline Coord GetMaxEndC(const Sector& sect)
	{
		return MAXByXY(sect.c1, sect.c2);
	}


} // namespace Geometry


/****************************************************************************/
/*					INLINE function implementations							*/
/****************************************************************************/

inline double	Sector::GetLength () const
{
	return (c2 - c1).GetLength ();
}

inline double Geometry::DistLineCoord (const Sector& ln, const Point2D& pt)

{
	return Geometry::Dist (CoordProjSector (pt, ln), pt);
}

#endif
