#ifndef SYSPLINE_HPP
#define SYSPLINE_HPP

#pragma once

#include "ISymbol.hpp"
#include "DrawIndex.hpp"
#include "Point2D.hpp"
#include "BezierDetails.hpp"
#include "ExtendedPen.hpp"

struct Box2DData;
struct TRANMAT;


class VI_DLL_CLASS_EXPORT Sy_SplineType {	// 48 Bytes + content data; Spline record
	
	GSErrCode	Write_Version36 (GS::OChannel& oc) const;
	GSErrCode	Read_Version36 (GS::IChannel& ic);

	static const short actVersion;
	static const short previousVersion;

public:
	Int32			item_Len;				// length of the data record = endsOff+Round_UP_8(dirsLen)
	unsigned char	item_Typ;				// = SySpline (7)
	DrawIndex		drwIndex;				// index in drawing order or 0
private:
	short			filler_0;
	VBAttr::ExtendedPen	sy_pen;		// color information 0 if deleted
	Int32			filler_1;
public:
	short			sy_fragmentIdx;			// Fragment (layer) number: 1 - 16
	short			pattern;				// Pattern/lineType info.
	bool			autoSm;					// automatically calculated spline coeff from basic points
	bool			closed;					// closed between last-first pt
private:
	short			filler_2;
public:
	Int32			nCoords;				// Number of Coords
	Int32			nDirs;					// Number of Dir data records
	Int32			coorLen;				// Spline coordinate data length
	Int32			coorOff;				// Offset of the Spline coordinate from record start
											// = sizeof(Sy_SplineType)
	Int32			dirsLen;				// Dir data length
	Int32			dirsOff;				// Offset of the Dir data from record start
											// = sizeof(Sy_SplineType)+Round_UP_8(coorLen)
	short			determine;				// Determines the type of line (contour - 0 / inner - 1)
	char			sy2dRenovationStatus;	//Sy2dRenovationStatusTypes
private:
	char			filler_3;
	Int32			filler_4;
public:

	Sy_SplineType ();
	Sy_SplineType (const VBAttr::ExtendedPen&	pen,
				   short						fragmentIdx,
				   DrawIndex					drwIndex,
				   short						pattern,
				   short						status,
				   Int32						nCoords,
				   short						determine = 0);

	void				SetExtendedPen (const VBAttr::ExtendedPen& inPen);
	VBAttr::ExtendedPen	GetExtendedPen () const;

	Coord*						GetCoords (void);
	const Coord*				GetCoords (void) const;
	Geometry::DirType*			GetDirs (void);
	const Geometry::DirType*	GetDirs (void) const;

	void GetBounds (Box2DData& box) const;
	void Transform (const TRANMAT& tran, Coord* pCoords, Geometry::DirType* pDirs);
	void TransformInVectorImage (const TRANMAT& tran);
	bool Check (void) const;
	
	SyEnum		GetType () const;
	GSErrCode	ReadBin (GS::IChannel& oc, short version);

	GSErrCode	Write (GS::OChannel& oc, short version) const;
	GSErrCode	Read (GS::IChannel& oc, short version);

	static short GetActVersion () { return actVersion; }
	static short GetPreviousVersion () { return previousVersion; }
	static Int32 GetConvertedSize (Int32 ac19ItemLen);
};


#endif // SYSPLINE_HPP