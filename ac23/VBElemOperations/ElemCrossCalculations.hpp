// *********************************************************************************************************************
// Description:		Junction related utilities for 2D
//
// Module:			VBElemOperations
// Namespace:		VBCalculations
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************

#if !defined (ELEMCROSSCALCULATIONS_HPP)
#define ELEMCROSSCALCULATIONS_HPP

#pragma once

// --- Includes	--------------------------------------------------------------------------------------------------------

// from GSRoot
#include	"Definitions.hpp"

// from Geometry
#include	"Coord.h"
#include	"Circle2DData.h"

// from VBElements
#include	"OrderIndex.hpp"
#include	"VBElements/WallTypes.hpp"


#include	"VBElemOperationsExport.hpp"
#include	"IWall.hpp"
#include	"WallVRDData.hpp"


namespace VBCalculations
{

/*==========================	TYPE DEFINITIONS	========================*/

struct Wall2Crit {						/* Wall Connection Priority crit.			*/
	Int32					sequence;	/* Sequence of the Connected wall			*/
	EDB::OrderIndex			drwOrdInd;	/* OrderIndex of the Connected wall			*/
	Int32					cind;		/* Conn index of the Connected wall			*/
	double					thick;		/* Thickness of the Connected wall			*/
	ADB::AttributeIndex		cutFill;	/* Pattern of the Connected wall			*/
	ADB::AttributeIndex		composite;	/* Composite attribute of the Connected wall*/
	short					pen;		/* Pen of the Connected wall				*/

	Wall2Crit () : sequence (0), drwOrdInd (), cind (0), thick (0.0), cutFill (), composite (), pen (0) {}
};

struct BeamCrit {						/* Beam Connection Priority crit.			*/
	Int32					sequence;	/* Priority of the Connected beam			*/
	EDB::OrderIndex			drwOrdInd;	/* OrderIndex of the Connected beam			*/
	Int32					cind;		/* Conn index of the Connected beam			*/
	double					thick;		/* Thickness of the Connected beam			*/
	short					pen;		/* Pen of the Connected beam				*/

	BeamCrit () : sequence (0), drwOrdInd (), cind (0), thick (0.0), pen (0) {}
};

/*==========================		CONSTANTS		========================*/

#define Max_Conn	5.0
#define Max_Conn_Compl	25.0

VB_ELEM_OPERATIONS_DLL_EXPORT
bool ElemCrossPoint (const Coord*		p1,
						const Coord*		p2,
						const Coord*		q1,
						const Coord*		q2,
						double*			t1,
						double*			t2,
						const Geometry::Circle2D*	iptr,
						const Geometry::Circle2D*	jptr,
						bool			iflg,
						bool			jflg,
						short			chkFlg,
						bool			firstX = false,
						bool			cwChkIns = false,
						bool			closerInters = true);

VB_ELEM_OPERATIONS_DLL_EXPORT
bool ElemCrossAndCheckT (const Coord*		p,
						const Geometry::Circle2D*		pc,
						const bool*			arcp,
						const bool*			negp,
						const Coord*		q,
						const Geometry::Circle2D*		qc,
						const bool*			arcq,
						const bool*			negq,
						const bool			checkMinMax,
						const bool          complfl,
						const short			chkFlg,
						const bool			firstX,
						double*				tp,
						double*				qt,
						bool*				okConn,
						bool*				badConn);

VB_ELEM_OPERATIONS_DLL_EXPORT
bool ElemCrossAndCheckL (const bool			realL,
						const bool			polyCross,
						const bool			complfl,
						const Coord*		p,
						const Geometry::Circle2D*		pc,
						const bool*			arcp,
						const bool*			negp,
						const Coord*		q,
						const Geometry::Circle2D*		qc,
						const bool*			arcq,
						const bool*			negq,
						const bool			specCr,
						double*				tp,
						double*				qt,
						short*				crossTyp,
						bool*				halfAngle,
						bool*				okConn);

VB_ELEM_OPERATIONS_DLL_EXPORT
bool ElemCrossAndCheckLRegular (const bool			realL,
								const bool			polyCross,
								const bool			complfl,
								const Coord*		p,
								const Geometry::Circle2D*		pc,
								const bool*			arcp,
								const bool*			negp,
								const Coord*		q,
								const Geometry::Circle2D*		qc,
								const bool*			arcq,
								const bool*			negq,
								const bool			specCr,
								double*				tp,
								double*				qt,
								short*				crossTyp,
								bool*				halfAngle,
								bool*				okConn);

VB_ELEM_OPERATIONS_DLL_EXPORT
bool ElemCrossAndCheckLSimple (const Coord*		p,
							  const Geometry::Circle2D*		pc,
							  const bool*		arcp,
							  const bool*		negp,
							  const Coord*		q,
							  const Geometry::Circle2D*		qc,
							  const bool*		arcq,
							  const bool*		negq,
							  bool*				halfAngle);

VB_ELEM_OPERATIONS_DLL_EXPORT
bool ElemCrossAndCheckTSimple (const Coord*		p,
							  const Geometry::Circle2D*		pc,
							  const bool*		arcp,
							  const bool*		negp,
							  const Coord*		q,
							  const Geometry::Circle2D*		qc,
							  const bool*		arcq,
							  const bool*		negq);

VB_ELEM_OPERATIONS_DLL_EXPORT
void	SetWall2Crit (Wall2Crit* wCrit0,
					  Wall2Crit* wCrit1,
					  Wall2Crit* pactCrit);

VB_ELEM_OPERATIONS_DLL_EXPORT
void	SetBeamCrit (BeamCrit* bCrit0,
					 BeamCrit* bCrit1,
					 BeamCrit* pactCrit);

VB_ELEM_OPERATIONS_DLL_EXPORT
double ElemCrPnt (const Sector*	s1,
				  const Coord*	xc);

VB_ELEM_OPERATIONS_DLL_EXPORT
double ElemCalAng (bool		aflg,
				   const Sector*	sc,
				   const Geometry::Circle2D*	cptr,
				   const Coord*		xc,
				   bool				cwChkIns = false);

VB_ELEM_OPERATIONS_DLL_EXPORT
void	SetWall2PointAndInd (const VBD::IWall&					 wall,
							 bool							 	 begFlag,
							 Coord*								 p,
							 Geometry::Circle2D*				 pc,
							 bool*								 arcWall,
							 bool*								 negAng,
							 Int32*								 pind,
							 const VBD::IWall&					 connWall,
							 bool								 connBegFl,
							 const Geometry::IrregularPolygon2D* pWall2ToUse = nullptr);

VB_ELEM_OPERATIONS_DLL_EXPORT
Int32	GetTind (const VBD::IWall& wall, const VBD::IWall& connWall, bool connBegFl);

VB_ELEM_OPERATIONS_DLL_EXPORT
Int32	GetCrossInd (const VBD::IWall& wall, const VBD::IWall& connWall, Int32 inval, bool firstRef);

VB_ELEM_OPERATIONS_DLL_EXPORT
Int32	EqualPointCorr (bool   connBegFl,
						Int32   j,
						Int32   re,
						Int32   maxn,
						Coord* p,
						Coord* bc,
						Coord* ec,
						bool   flipped);
VB_ELEM_OPERATIONS_DLL_EXPORT
void	CalcBestPToInd (Int32*  numP,
						Int32   index,
						Int32*  resInd,
						Coord* refP,
						Coord* newP,
						Coord* resP);

VB_ELEM_OPERATIONS_DLL_EXPORT
Int32 	NextInd (Int32 ind, Int32 maxInd);

VB_ELEM_OPERATIONS_DLL_EXPORT
Int32 	PrevInd (Int32 ind, Int32 maxInd);


} // namespace VBCalculations

#endif
