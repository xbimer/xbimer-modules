// *********************************************************************************************************************
// Description:		Calculating wPoly for View Related Derived Data of Wall 
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************

#ifndef	CALCULATEWALLWPOLY_HPP
#define	CALCULATEWALLWPOLY_HPP

#pragma once

#include "VBElemOperationsExport.hpp"

#include "WallFloorPlanCalculations.hpp"
#include "WallVRDData.hpp"

namespace VBElem {
	class CWConnectionList;
	class WallConnectionList;
	struct WallConnectionLists;
}

namespace VBCalculations {
	class WallNotViewRelatedPolygons;
}

namespace VBD {

void FinalizeAndStoreWPoly (Coord*										p,
							bool*										arcWall,
							Int32*										pind,
							double*										tp,
							VBD::WallVRDDataRef							vrdData,
							VBElem::WallConstRef						wall,
							VBD::ViewConstRef							view,
							const VBCalculations::WallCutPlaneData*		wCutPlane,
							VBCalculations::WallNotViewRelatedPolygons*	wallNotVRDPolys);

VBD::WallVRDDataRef GetNotFullyCalculatedViewRelatedDerivedData (VBElem::ModelElementConstRef	elem,
																 VBD::ViewConstRef				view);

void VB_ELEM_OPERATIONS_DLL_EXPORT	GetBestWall2Ind (const VBElem::WallConnectionList&	connections,
													 VBElem::WallConstRef				whichWall,
													 bool								begFlag,
													 Int32*								bestI);

VBCalculations::ComplexResult VB_ELEM_OPERATIONS_DLL_EXPORT	ComplexCalculateThicknessAndOffset (ODB::ConstRef<VBD::View>			view,
																								VBElem::WallConstRef				pWall,
																								double*								thickness,
																								double*								offset,
																								short								showElemFloor,
																								EffectiveComposite::Structure		structureDisplay,
																								VBCalculations::WallCutPlaneData*	wCutPlane = nullptr,
																								CompInfo::CompositeInfo*			compInfo = nullptr,
																								double*								thickCorr = nullptr,
																								double*								offsCorr = nullptr,
																								EDB::FloorStructureConstRef			floorStructure = nullptr);

void VB_ELEM_OPERATIONS_DLL_EXPORT ChooseCWConns (ODB::ConstRef<VBD::View>		view,
												  short							showElemFloor,
												  VBElem::CWConnectionList&		connections,
												  bool     						autoVisib = false);

void VB_ELEM_OPERATIONS_DLL_EXPORT ChooseWall2Conns (ODB::ConstRef<VBD::View>		view,
													 short							showElemFloor,
													 VBElem::WallConnectionList&	connections,
													 VBElem::WallConstRef			otherWall);

bool  VB_ELEM_OPERATIONS_DLL_EXPORT IsMirroredSkins (VBCalculations::CW2StructRec*	a_plCW2);

bool VB_ELEM_OPERATIONS_DLL_EXPORT	XElems (Sector*	s1,
											double	angle1,
											bool	isArc1,
											Sector*	s2,
											double	angle2,
											bool	isArc2,
											Coord*	refp,
											bool	inCheck1,
											bool	inCheck2,
											bool	sameCirc,
											Coord*	xc,
											bool*	notInters = nullptr);

void	VB_ELEM_OPERATIONS_DLL_EXPORT CalcWall2SkinsPointNeig (VBElem::WallConstRef							pWall2,
															   const VBCalculations::CW2StructRec* const	pCW2,
															   Int32*										bNum,
															   Coord*										bPoints,
															   Int32*										bNInd,
															   Int32*										eNum,
															   Coord*										ePoints,
															   Int32*										eNInd,
															   bool											fromWPoly,
															   VBCalculations::WallComputingEnvir* const	wce,
															   VBD::WallVRDDataConstRef						vrdData,
															   bool											useIfWasGerbevagasStatus = false,
															   VBD::ViewConstRef							view = nullptr,
															   const VBCalculations::WallCutPlaneData*		wCutPlane = nullptr,
															   VBCalculations::WallNotViewRelatedPolygons*	wallNotVRDPolys  = nullptr);

void VB_ELEM_OPERATIONS_DLL_EXPORT ComputeAnglePars (VBElem::WallConstRef							wall,
													 VBCalculations::CW2StructRec*					cw2,
													 bool											noTurn,
													 bool											begFlag,
													 VBCalculations::DirAnglePars*					dirAnglePars,
													 VBD::WallVRDDataConstRef 						vrdData,
													 VBD::ViewConstRef								view = nullptr,
													 const VBCalculations::WallCutPlaneData*		wCutPlane = nullptr,
													 VBCalculations::WallNotViewRelatedPolygons*	wallNotVRDPolys = nullptr);

void CalcWall2Corn (short											showElemFloor, 
					const VBCalculations::WallCutPlaneData*			wCutPlane,
					VBCalculations::WallNotViewRelatedPolygons*		wallNonVRDPolys,
					VBElem::WallConstRef							whichWall,
					bool											nOnlyCalc,
					VBCalculations::WallComputingEnvir* const		wce,
					VBCalculations::WallBeamConnectionEnvir* const	wbce,
					ODB::ConstRef<VBD::View>						view,
					VBD::WallVRDDataRef 							vrdData,
					const VBElem::WallConnectionLists& 				lists);

void  Elimin_WallEnds (VBCalculations::WallComputingEnvir* const		wce, 
					   VBCalculations::WallBeamConnectionEnvir* const	wbce,
					   VBD::WallVRDDataConstRef 						vrdData,
					   VBElem::WallConstRef								wall,
					   VBD::ViewConstRef								view,
					   const VBCalculations::WallCutPlaneData*			wCutPlane,
					   VBCalculations::WallNotViewRelatedPolygons*		wallNotVRDPolys = nullptr);

void SetBegParams (VBElem::WallConstRef				pWall2,
				   Int32							bInd,
				   Int32							nCo,
				   Sector*							sb,
				   Sector*							sbp,
				   bool*							abIsArc,
				   bool*							abpIsArc,
				   double*							abAngle,
				   double*							abpAngle,
				   Geometry::IrregularPolygon2D		poly);

void SetEndParams (VBElem::WallConstRef				pWall2,
				   Int32							eInd,
				   Int32							nCo,
				   Sector*							se,
				   Sector*							sep,
				   bool*							aeIsArc,
				   bool*							aepIsArc,
				   double*							aeAngle,
				   double*							aepAngle,
				   Geometry::IrregularPolygon2D		poly);

void SetDegenBegParam (VBElem::WallConstRef				pWall2,
					   Int32							bInd,
					   Int32							nCo,
					   Sector*							sb,
					   bool*							abIsArc,
					   double*							abAngle,
					   Geometry::IrregularPolygon2D		poly);

void SetDegenEndParam (VBElem::WallConstRef				pWall2,
					   Int32							eInd,
					   Int32							nCo,
					   Sector*							se,
					   bool*							aeIsArc,
					   double*							aeAngle,
					   Geometry::IrregularPolygon2D		poly);

void GetWall2SkinsPointNeig (VBElem::WallConstRef						wall2,
							 Int32										maxskin,
							 Int32*										bNum,
							 Coord*										bPoints,
							 Int32*										bNInd,
							 Int32*										eNum,
							 Coord*										ePoints,
							 Int32*										eNInd,
							 VBCalculations::WallComputingEnvir* const	wce,
							 Geometry::IrregularPolygon2D				wPoly,
							bool										useIfWasGerbevagasStatus);

void SetWall2PointAndIndComplStand (short											showElemFloor,
									const VBCalculations::WallCutPlaneData*			wCutPlane,
									VBElem::WallConstRef							pWall,
									bool											begFlag,
									Coord*											p,
									Geometry::Circle2D*								pc,
									bool*											arcWall,
									bool*											negAng,
									Int32*											pind,
									VBElem::WallConstRef							connWall,
									bool											connBegFl,
									ODB::ConstRef<VBD::View>						view,
									VBD::WallVRDDataConstRef						vrdData,
									VBCalculations::WallNotViewRelatedPolygons*		wallNotVRDPolys = nullptr);

}		//namespace VBD

#endif