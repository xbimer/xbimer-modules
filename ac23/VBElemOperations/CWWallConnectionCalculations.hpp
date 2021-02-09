// *********************************************************************************************************************
// Description:		Connection Calculations between Wall and CurtainWall
//
// Module:			VBElemOperations
// Namespace:		VBCalculations
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************




#ifndef	CWWALLCONNECTIONCALCULATIONS_HPP
#define	CWWALLCONNECTIONCALCULATIONS_HPP

#pragma once

// from Geometry
#include "GridMesh.hpp"
#include "PeriodicInterval.hpp"

// from VBElements
#include "VBElements/CurtainWall.hpp"


#include "VBElemOperationsExport.hpp"
#include "IWall.hpp"


namespace VBCalculations {


bool VB_ELEM_OPERATIONS_DLL_EXPORT	IsCWAndWallConnectable (VBElem::WallConstRef wall, VBElem::CurtainWallConstRef cw, bool atBeg, bool* closestInside);


bool VB_ELEM_OPERATIONS_DLL_EXPORT	IsCWAndStraightWallConnectable (const VBElem::CurtainWallConstRef& cw, const  VBD::IWall& wall, Coord* crds,
																	const VBElem::CWSegmentConstRef& cwSeg, bool atBeg, bool firstRun, bool* closestInside, const double* offsets,
																	double* refDirWidth, double& minT, double& maxT, double& len, Int32* whichSideInf);


bool VB_ELEM_OPERATIONS_DLL_EXPORT	IsCWAndCurvedWallConnectable (const VBElem::CurtainWallConstRef& cw, const  VBD::IWall& wall, Coord* crds,
																  const VBElem::CWSegmentConstRef& cwSeg, bool atBeg, bool firstRun, bool* closestInside, const double* offsets,
																  double* refDirWidth, double& minFi, double& maxFi, bool useFullCW, Int32* whichSideInf);


double VB_ELEM_OPERATIONS_DLL_EXPORT	GetCWCutHeight (const VBD::IWall& wall, const VBElem::CurtainWallConstRef& cw, const VBElem::CWSegmentConstRef& cwSeg);


void VB_ELEM_OPERATIONS_DLL_EXPORT	CreateOffsetedCWMesh (const VBElem::CurtainWallConstRef& cw, const VBElem::CWSegmentConstRef& cwSegment,
							  double inOffset, Geometry::GridMesh* outMesh, bool storePolyIndexesAsIDs = false);


void VB_ELEM_OPERATIONS_DLL_EXPORT	IntersectOffsetedCWMeshWithPlane (const VBElem::CurtainWallConstRef& cw, const VBElem::CWSegmentConstRef& cwSegment,
										  double inOffset, double inZ, GS::PagedArray<Coord_2D_Array>& outArray);


void VB_ELEM_OPERATIONS_DLL_EXPORT	GetOffsetedCWBounds (const VBElem::CWSegmentConstRef& cwSegment, const double* offsets, Box3D* bound);


bool VB_ELEM_OPERATIONS_DLL_EXPORT	GetClosestCWPointInDir (const VBElem::CurtainWallConstRef& cw, double inOffset, bool isArc, const Sector& inDir,
															const Geometry::Circle2D* inCircle, bool isNegative, const Geometry::PeriodicInterval& zInterval, Coord3D* outPoint);

}		//namespace VBCalculations


#endif