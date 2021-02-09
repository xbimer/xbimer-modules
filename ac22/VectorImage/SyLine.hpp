#ifndef SYLINE_HPP
#define SYLINE_HPP

#pragma once

#include "ISymbol.hpp"
#include "DrawIndex.hpp"
#include "Point2D.hpp"
#include "ExtendedPen.hpp"
#include "Sy2DRenovationStatusTypes.hpp"

struct Box2DData;
struct TRANMAT;

class VI_DLL_CLASS_EXPORT Sy_LinType {			// 56 Bytes; Symbol Line record
	
	GSErrCode	Write_Version36 (GS::OChannel& oc) const;
	GSErrCode	Read_Version36 (GS::IChannel& ic);

	static const short actVersion;
	static const short previousVersion;

public:
	Int32			item_Len;					// length of the data record = sizeof(Sy_LinType)
	unsigned char	item_Typ;					// = SyLine (3)
	DrawIndex		drwIndex;					// index in drawing order or 0
private:
	VBAttr::ExtendedPen	sy_pen;					// color information 0 if deleted
public:
	short			sy_fragmentIdx;				// Fragment (layer) number: 1 - 16
	short			pattern;					// Pattern/lineType info.
private:
	short			filler_0;
public:
	Int32			sy_inIndex;					// use in VirtSy line index
private:
	Int32			filler_1;
public:
	Coord			begC;						// Coordinate of the first Point
	Coord			endC;						// Coordinate of the second Point
	short			determine;					// Determines the type of line (contour - 0 / inner - 1)
	short			specForProfile;				// 0 standard line, 1 special lines for profile (status line or window placement)
	char			sy2dRenovationStatus;		//Sy2dRenovationStatusTypes
	char			specFor3D;					// 0: Sy3DInformation
private:
	short			filler_2;
public:

	Sy_LinType ();
	Sy_LinType (const VBAttr::ExtendedPen& pen, short pattern, short fragmentIdx, DrawIndex drwIndex,
				double x1, double y1, double x2, double y2, short determine = 0 ,
				char specFor3D = SySymbolic,
				char sy2drenovationStatus = VI::SyUndefinedRenovationStatus);

	bool operator== (const Sy_LinType& other) const;
	Sy_LinType& operator= (const Sy_LinType& source);

	void				SetExtendedPen (const VBAttr::ExtendedPen& inPen);
	VBAttr::ExtendedPen	GetExtendedPen () const;

	void GetBounds (Box2DData& box) const;
	void Transform (const TRANMAT& tran);
	
	SyEnum		GetType () const;
	GSErrCode	ReadBin (GS::IChannel& oc, short version);

	GSErrCode	Write (GS::OChannel& oc, short version) const;
	GSErrCode	Read (GS::IChannel& oc, short version);
	
	static short GetActVersion () { return actVersion; }
	static short GetPreviousVersion () { return previousVersion; }
	static Int32 GetConvertedSize (Int32 ac19ItemLen);
};



#endif // SYLINE_HPP