// *********************************************************************************************************************
// Description:		Simple data class representing a Bezier spline
//
// Module:			Geometry
// *********************************************************************************************************************

#ifndef BEZIERDETAILS_HPP
#define BEZIERDETAILS_HPP

// --- Includes	--------------------------------------------------------------------------------------------------------

#include "Sector2DData.h"

// from GSRoot
#include "Array.hpp"

namespace Geometry {

struct DirType {
	double		lenPrev;			/* Bezier direction length prev/next */
	double		lenNext;
	double		dirAng;				/* Bezier direction Angle */
};

GEOMETRY_DLL_EXPORT GSErrCode	Store	(GS::ObjectState& os, const DirType& dirType);
GEOMETRY_DLL_EXPORT GSErrCode	Restore	(const GS::ObjectState& os, DirType& dirType);

// --- BezierDetails ---------------------------------------------------------------------------------------------------

class GEOMETRY_DLL_EXPORT BezierDetails {

private:
	GS::Array<Point2D>		coords;
	GS::Array<DirType>		dirs;
	bool					closed;

public:
	BezierDetails ();
	BezierDetails (const BezierDetails&);

	BezierDetails&	operator= (const BezierDetails&);

	BezierDetails (Int32 nCoords, const Point2D** hCoords, const DirType** hDirs);
	BezierDetails (Int32 nCoords, const Point2D* pCoords, const DirType* pDirs);

	Int32 GetCoordsNum () const { return coords.GetSize (); }
//	const Point2D** const	GetCoords () const { return const_cast<const Point2D** const> (coords); }
//	const DirType** const GetDirType () const { return const_cast<const DirType** const> (dirs); }

	const Point2D* const	GetCoordsPtr () const { return coords.GetContent (); }
	const DirType* const GetDirsPtr () const { return dirs.GetContent (); }

	bool SetData (Int32 nCoords, const Point2D* pCoords, const DirType* pDirs);
	bool SetData (const GS::Array<Point2D>& aCoords, const GS::Array<DirType>& aDirs);
	bool GetData (GS::Array<Point2D>& aCoords, GS::Array<DirType>& aDirs) const;

	bool IsClosed () const { return closed; }
	void SetClosed (bool clo) { closed = clo; }

	void GetBezierDirPoint (Int32 cIndex, bool forward, Point2D* cc) const;

	GSErrCode		Read  (GS::IChannel& ic);
	GSErrCode		Write (GS::OChannel& oc) const;

	GSErrCode		Store	(GS::ObjectState& os) const;
	GSErrCode		Restore	(const GS::ObjectState& os);
};


GEOMETRY_DLL_EXPORT void TangentLines (const Point2D& p, const BezierDetails& bez, SectorResultFn fn);
GEOMETRY_DLL_EXPORT void TangentLines (const Sector& s, const BezierDetails& bez, SectorResultFn fn);
GEOMETRY_DLL_EXPORT void NormalLines (const Point2D& p, const BezierDetails& bez, SectorResultFn fn);
GEOMETRY_DLL_EXPORT void NormalLines (const Sector& s, const BezierDetails& bez, SectorResultFn fn);


inline GSErrCode	Store	(GS::ObjectState& os, const BezierDetails& bezier)		{ return bezier.Store (os); }
inline GSErrCode	Restore	(const GS::ObjectState& os, BezierDetails& bezier)		{ return bezier.Restore (os); }


} // namespace Geometry

#endif // BEZIERDETAILS_HPP
