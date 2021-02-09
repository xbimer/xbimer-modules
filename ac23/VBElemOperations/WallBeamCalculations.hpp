// *********************************************************************************************************************
// Description:		Calculating wPoly for View Related Derived Data of Wall 
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************

#ifndef	WALLBEAMCALCULATIONS_HPP
#define	WALLBEAMCALCULATIONS_HPP

#pragma once

#include "WallWPolyCalculations.hpp"
#include "WallConnectionList.hpp"
#include "CWConnectionList.hpp"
#include "Wall.hpp"
#include "WallFloorPlanCalculations.hpp"
#include "WallVRDData.hpp"
#include "ODBReference.hpp"
#include "ODBIRef.hpp"

#include "DefaultDetailHandler.hpp"
#include "DetailHandler.hpp"
#include "AttributeManager.hpp"
#include "fill.hpp"
#include "ODBReference.hpp"
#include "Md5.hpp"
#include "VBElemSet/ElemSet.hpp"

#include "GDLGlobalCalculations.hpp"
#include "CrossSectType.hpp"

// from Phasing
#include "PhasingTypes.hpp"

#include "LineBase.hpp"


enum EndPointType
{
	Main_Wall = 0,
	Other_Wall = 1,
	Invalid_Wall = 2
};

struct EndPoint
{
	double					position;
	CutSurface				cutSurface;
	CompInfo::SkinType		skinType;
	EndPointType			endPointType;
	bool					last;

	EndPoint () :
		position (0.0),
		//cutSurface (),
		skinType (CompInfo::Skin_Normal),
		endPointType (Invalid_Wall),
		last (false)
	{
	}
};

namespace VBD {


void VB_ELEM_OPERATIONS_DLL_EXPORT HalfAngle_Sect (UIndex							ind,
												   GS::Array <EndPoint>&			endPoints,
												   VBCalculations::WallBeamConnectionEnvir* const  wbce);

bool VB_ELEM_OPERATIONS_DLL_EXPORT GoodDirAngle (const bool     bothLineOK,
												 const Coord&   wbegC,
												 const Coord&   wendC,
												 const double   wAngle,
												 const bool     wBegFlag,
												 const Coord&   cbegC,
												 const Coord&   cendC,
												 const double   cAngle,
												 const bool     cBegFlag,
												 const double  limitValue);

void VB_ELEM_OPERATIONS_DLL_EXPORT	Min_Sect (UIndex											ind,
											  double											min,
											  VBCalculations::WallBeamConnectionEnvir* const	wbce);

void VB_ELEM_OPERATIONS_DLL_EXPORT	Max_Sect (UIndex											ind,
											  double											max,
											  VBCalculations::WallBeamConnectionEnvir* const	wbce);

void VB_ELEM_OPERATIONS_DLL_EXPORT	Store_Sect (UIndex											ind,
												double											bm,
												double											em,
												VBCalculations::WallBeamConnectionEnvir* const	wbce);

void VB_ELEM_OPERATIONS_DLL_EXPORT	Init_Sect (UIndex num, VBCalculations::WallBeamConnectionEnvir* const wbce);

void VB_ELEM_OPERATIONS_DLL_EXPORT	Skal_Sect (UIndex num, VBCalculations::WallBeamConnectionEnvir* const wbce);

void VB_ELEM_OPERATIONS_DLL_EXPORT	Full_Sect (UIndex ind, VBCalculations::WallBeamConnectionEnvir* const wbce, bool onEmptyToo);

}

#endif