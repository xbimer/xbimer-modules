// *****************************************************************************
//
// Declaration of CorePolygon2D class
//
// Module:			Geometry
// Namespace:		Geometry
// Contact person:	RA, RJ
//
// *****************************************************************************

#ifndef GEOMETRY_COREPOLYGON2D_CLASSDEFINITION_HPP
#define GEOMETRY_COREPOLYGON2D_CLASSDEFINITION_HPP

#pragma once

// --- Includes ----------------------------------------------------------------

// from GSRoot
#include "Algorithms.hpp"
#include "DebugHeap.hpp"
#include "Pair.hpp"
#include "DoubleClassification.hpp"

//own
#include "GeometryMain.h"                //GetDebugMode()
#include "Poly2D.hpp"
#include "Curve2DData.h"
#include "Circle2DData.h"
#include "PGBooleanOperations.hpp"	// test miatt kell
#include "CutElems.h"
#include "Polygon2DOperations.h"
#include "Cut2DInternal.h"		// test miatt kell
#include "Poly2DUserDataConverter.hpp"

#include <cstdlib>

namespace Geometry {
	template <class VertexUserData, class EdgeUserData, class PolygonUserData> class CorePolyline2D;
	template <class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData> class CorePolygon2D;

	template <class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	 GSErrCode	Store (GS::ObjectState& os, const CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& polygon);

	 template <class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	 GSErrCode	Restore (const GS::ObjectState& os, CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& polygon);
}

namespace Geometry {

	class GEOMETRY_DLL_EXPORT CorePolygon2DPrivate {
	public:
		static GS::ClassInfo	classInfo;
	};

	class GEOMETRY_DLL_EXPORT CorePolygon2DVertexVisitor
	{
	public:
		virtual ~CorePolygon2DVertexVisitor ();

		virtual void operator () (Point2D &) = 0;
	};
	class GEOMETRY_DLL_EXPORT CorePolygon2DConstVertexVisitor
	{
	public:
		virtual ~CorePolygon2DConstVertexVisitor ();

		virtual void operator () (const Point2D &) = 0;
	};

	class GEOMETRY_DLL_EXPORT EdgeSplitControl 
	{
	public:
		virtual ~EdgeSplitControl ();

		virtual bool AllowSplit (const Sector& /*edge*/, const Point2D& /*splitPoint*/) const { return true; }
		virtual bool AllowSplit (const GenArc& /*edge*/, const Point2D& /*splitPoint*/) const { return true; }
	};

	template <class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	class CorePolygon2D : public GS::SharedObject
	{
		template <class VertexUserData1, class EdgeUserData1, class ContourUserData1, class PolygonUserData1>
		friend class CorePolygon2D;
		template <class VertexUserData1, class EdgeUserData1, class ContourUserData1, class PolygonUserData1>
		friend class CustomPolygon2D;
		template <class VertexUserData1, class EdgeUserData1, class ContourUserData1, class PolygonUserData1>
		friend class CustomIrregularPolygon2D;
		template <class VertexUserData1, class EdgeUserData1, class PolygonUserData1>
		friend class CorePolyline2D;

		friend GSErrCode	Store<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData> (GS::ObjectState& os, const CorePolygon2D& polygon);
		friend GSErrCode	Restore<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData> (const GS::ObjectState& os, CorePolygon2D& polygon);
		friend GSErrCode	Store (GS::ObjectState& os, const Geometry::CustomPolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& polygon);
		friend GSErrCode	Restore (const GS::ObjectState& os, Geometry::CustomPolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& polygon);
		friend GSErrCode	Store (GS::ObjectState& os, const Geometry::CustomIrregularPolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& polygon);
		friend GSErrCode	Restore (const GS::ObjectState& os, Geometry::CustomIrregularPolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& polygon);
	public:
		enum PolygonResult {
			NewPolygons,
			EmptyPolygon,
			ResultSameAsThis,
			ResultSameAsFirstParameter,
			ResultThisAndParameter
		};
		struct Flags {
			Flags ()
			{
				Clear ();
			}
			void Clear ()
			{
				BNZeroMemory (this, sizeof (Flags));
				isSameDirEdgeLegal = true;
				isContourUserData = true;
				isVertexUserData = true;
				isEdgeUserData = true;
				isPolygonUserData = true;
				force64BitPrecision = false;
			}
			GS::UInt16					isSameDirEdgeLegal		: 1; // Set if two succeeding edge with same direction (two edge in one line) is legal
			GS::UInt16					isVertexUserData		: 1;
			GS::UInt16					isEdgeUserData			: 1;
			GS::UInt16					isContourUserData		: 1;
			GS::UInt16					isPolygonUserData		: 1;
			GS::UInt16					force64BitPrecision		: 1; 
		};
		struct Status {
			Status ()
			{
				Clear ();
			}
			void Clear ()
			{
				BNZeroMemory (this, sizeof (Status));
				isRegularized = false;
			}
			unsigned char			isRegularized			: 1;
		};

	private:

		enum class LineCutMode {
			Overlap,
			Difference
		};

		Box2DData						boundBox;			// Boundbox of the whole polygon
		Flags							flags;
		mutable Status					status;
		GS::ArrayFB <PolygonUserData, 6>		inhPolygonInfo;		// Handle containing the attached polygon informations (inherited)
		VertexUserData					defaultVertexData;
		EdgeUserData					defaultEdgeData;
		ContourUserData					defaultContourData;

		USize							nVertices;			// Total number of all vertices in all contours (equals to num of edges)
		GS::ArrayFB <Point2D, 6>				vertices;			// Handle containing the vertices
		GS::ArrayFB <VertexUserData, 6>		vertexInfos;		// Handle containing the attached vertex informations (eg. vertID)

		USize							nArcs;				// Total number of curved edges
		GS::ArrayFB <PolyArcRec, 6>			arcs;				// the view angle of arcs (sign: + means CCW, - means CW) and the connecting vertices
		GS::ArrayFB <EdgeUserData, 6>		inhEdgeInfos;		// Handle containing the attached edge informations inherited by all descendant (eg. height)

		USize							nContours;			// The number of contours: the first is the main (outer), the others are the holes
		GS::ArrayFB <UIndex,6>				contourEnds;		// The index of the vertex in vertexPts which is the last in the given contour
		GS::ArrayFB <ContourUserData, 6>		contourInfos;		// Handle containing the attached contour informations (inherited)


		void InitInternalData ();
		void FreeInternalData ()																										CAN_THROW ((GS::OutOfMemoryException));
		void ReallocateInternalDataSizes (USize verticesNum, USize arcsNum, USize contoursNum, USize verticesCapacity, USize arcsCapacity, USize contoursCapacity)		CAN_THROW ((GS::OutOfMemoryException));
		void AllocateInternalData (USize verticesNum, USize arcsNum, USize contoursNum, USize verticesCapacity = 0, USize arcsCapacity = 0, USize contoursCapacity = 0)		CAN_THROW ((GS::OutOfMemoryException));
		void ReallocateInternalData (USize verticesNum, USize arcsNum, USize contoursNum, USize verticesCapacity = 0, USize arcsCapacity = 0, USize contoursCapacity = 0)		CAN_THROW ((GS::OutOfMemoryException));

		void SetInternalDataVertices (const Point2D* pCoords, GS::UInt32 createFlags);
		void SetInternalDataVertices (const GS::Array<Point2D>& coordLists, USize contourNum, GS::UInt32 createFlags);
		void SetInternalDataArcs (const PolyArcRec* pArcs, USize arcNum, GS::UInt32 createFlags);
		void SetInternalDataArcs (const GS::Array<PolyArcRec>& arcList, USize arcNum, GS::UInt32 createFlags);
		void SetInternalDataContours (const UIndex* pContourEnds, USize contourNum, GS::UInt32 createFlags);
		void SetInternalDataContours (const GS::Array<UIndex>& contourEndList, USize contourNum, GS::UInt32 createFlags);
		void SetInternalData (const Point2D* pCoords, USize coordNum, const PolyArcRec* pArcs, USize arcNum, const UIndex* pContourEnds, USize contourNum, GS::UInt32 createFlags);
		void SetInternalData (const GS::Array<Point2D>& coordList, const GS::Array<PolyArcRec>& arcList, const GS::Array<UIndex>& contourEndList, GS::UInt32 createFlags);

		template <class VertexUserDataSource, class EdgeUserDataSource, class ContourUserDataSource, class PolygonUserDataSource>
		void CopyInternalDataFromWithUserData (const CorePolygon2D<VertexUserDataSource, EdgeUserDataSource, ContourUserDataSource, PolygonUserDataSource>& source, 
											   USize verticesCapacity, 
											   USize arcsCapacity, 
											   USize contoursCapacity,
											   Poly2DUserDataConverter<VertexUserDataSource, EdgeUserDataSource, ContourUserDataSource, PolygonUserDataSource,
											   VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>* userDataConverter)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		
		static void ClearUserDataPolyline (CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		
		void CopyInternalDataFromClearUserData ()			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CopyInternalDataFromUserData (const CorePolygon2D& source, UIndex fromVertex, UIndex begVertexIdx, USize newVerticesNum, UIndex fromContour, UIndex contourIdx, USize newContourNum)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CopyInternalDataFrom (const CorePolygon2D& source, USize verticesCapacity, USize arcsCapacity, USize contoursCapacity)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CopyInternalDataFrom (const CorePolygon2D& source, UIndex contourIdx)															CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CopyInternalContourFrom (const CorePolygon2D& source, UIndex contourIdx)														CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CopyPolygonLevelDataFrom (const CorePolygon2D& source);
		void CopyPolygonLevelDataFrom (const GS::Array<GS::SharedPtr<CorePolygon2D>>& source);

		UIndex VertexIdx2InternalVertexIdx (UIndex vertexIdx) const;
		UIndex InternalVertexIdx2VertexIdx (UIndex vertexIdx) const;

		void AddOneVertex (const Point2D& coord, const VertexUserData* vertexInfo, const EdgeUserData* edgeInfo, const PolyArcRec* arc = nullptr);
		void AddOneContour (const ContourUserData* contourInfo);
		void DeleteVertexFromInternalData (UIndex internalVertexIdx);
		void FixingContours ();  //Erre tipikusan delete vertex utan van szukseg helyre rakja a contourokat, ha van olyan amiben csak ket vagy annal kevesebb pont van, akkor azt torli.

		UIndex GetContourIdxFromInternalVertexIdx (UIndex vertexIdx) const;
		//A contourhoz tartozo elso vertex internal indexet adja
		UIndex GetInternalVertexBeginIdxInContour (UIndex contourIdx) const;
		//A contourhoz mar nem tartozo vertex internal indexet adja
		UIndex GetInternalVertexEndIdxInContour (UIndex contourIdx) const;
		UIndex GetInternalArcIdx (UIndex edgeIdx) const;

		UIndex GetNextVertexIndex (UIndex internalVertexIdx) const;
		UIndex GetPrevVertexIndex (UIndex internalVertexIdx) const;

		bool GetEdgeAsCurve2DData (UIndex internalEdgeIdx, Curve2DData& edge) const;

		void SetEdgeAngleInternal (UIndex internalEdgeIdx, double edgeAngle)																CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ReplaceEdgeInternal (UIndex internalEdgeIdx, const GS::Array<GenArc>& arcs, const ArcDivParams& arcDivParams)					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ReplaceEdgeInternal (UIndex internalEdgeIdx, const GS::Array<double>& angles, const GS::Array<Point2D>& internalVerts)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void InsertVertexInternal (UIndex internalEdgeIdx, const Point2D& newPosition, bool keepArc = false, bool insArc = false)				CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void InsertEdgeInternal (UIndex vertexInternalIndex)																				CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		static GS::Int32 LineLineToDeleteEdge (const Sector &prevEdgeLine, const Sector &nextEdgeLine, Point2D *result);
		void SetEdgeArcToDeleteEdge (GS::Int32& arcInd, bool& isEdgeArc, GenArc& edgeArc, Point2D& edgeArcCentre, double& edgeArcViewAngle, UIndex edgeIdx, const Point2D& vertFromPt, const Point2D& vertFromTo) const;
		bool PolygonDeleteEdge (UIndex edgeIdx)																								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void InsertVertexSplitArc (UIndex internalEdgeIdx, const Point2D& newPosition)														CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void OrientInternalDData ();
		bool CheckOrientInternalData () const;

		Box2DData CalcBoundBox () const;
		void UpdateBoundingBox ();
		void UpdateOrientation ();

		bool     CanIrregular (bool& reverseOrient) const;
		bool     CanVertexNumChange () const;
		void     SetIdsForRegularizationResultsOneNullVertex (const PolyId& edgeId, bool& thisContourRev, const UIndex& vertexIdx, const UIndex& contourIdx, const UIndex& polyIdx, GS::Array<UIndex>& revContourIdxsCurrPoly, const CorePolygon2D<PolyId, PolyId, PolyId, PolyId>& polygonCopy, const GS::Array<GS::SharedPtr<CorePolygon2D<PolyId, PolyId, PolyId, PolyId>>>& result1) const;
		void     SetIdsForRegularizationResultsOneContour (GS::Array<UIndex>& vertexIds, GS::Array<UIndex>& edgeIds, UIndex& vertexIdx, UIndex& contourIdx, UIndex& polyIdx, GS::Array<UIndex>& revContourIdxsCurrPoly, UIndex& regularizationResults /*PolyRegularizationFlags*/, const CorePolygon2D<PolyId, PolyId, PolyId, PolyId>& polygonCopy, const GS::Array<GS::SharedPtr<CorePolygon2D<PolyId, PolyId, PolyId, PolyId>>>& result1) const;
		void     SetIdsForRegularizationResults (GS::Array<UIndex>& vertexIds, GS::Array<UIndex>& edgeIds, GS::Array<UIndex>& contourIds, UIndex& regularizationResults /*PolyRegularizationFlags*/, GS::Array< GS::Array<UIndex>>* reversedContourIdxs, const CorePolygon2D<PolyId, PolyId, PolyId, PolyId>& polygonCopy, const GS::Array<GS::SharedPtr<CorePolygon2D<PolyId, PolyId, PolyId, PolyId>>>& result1) const;
		void     SetRegularizationResults (UIndex regularizationResultsMask /*PolyRegularizationFlags*/, UIndex& regularizationResults /*PolyRegularizationFlags*/, GS::Array< GS::Array<UIndex>>* reversedContourIdxs, const CorePolygon2D<PolyId, PolyId, PolyId, PolyId>& polygonCopy, const GS::Array<GS::SharedPtr<CorePolygon2D<PolyId, PolyId, PolyId, PolyId>>>& result1) const;
		void     CreateRegularizationResult (const CorePolygon2D<PolyId, PolyId, PolyId, PolyId>& polygonCopy, const GS::Array<GS::SharedPtr<CorePolygon2D<PolyId, PolyId, PolyId, PolyId>>>& result1, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const;

		PolygonResult   Regularize (GS::Array<GS::SharedPtr<CorePolygon2D>>&	result,
									PolyRegularizationMode						mode = PolyRegularizationMode_Default) const;

		PolygonResult   Regularize (GS::Array<GS::SharedPtr<CorePolygon2D>>&	result,
									UIndex reqRegularizationFlags				/*PolyRegularizationFlags*/,
									UIndex& regularizationResults				/*PolyRegularizationFlags*/,
									GS::Array< GS::Array<UIndex>>*				reversedContourIdxs = nullptr);

		void SetInternalDataEdgeInfos (const CorePolygon2D&	origPoly, USize verticesNum, GS::Int32 *inds, const GS::Array <const EdgeUserData*>& customInhEdgeInfo) CAN_THROW ((GS::OutOfMemoryException));
		void SetInternalDataVertexInfos (const CorePolygon2D& origPoly, USize verticesNum, GS::Int32 *indc) CAN_THROW ((GS::OutOfMemoryException));
		void SetInternalDataContourInfos (const CorePolygon2D& origPoly, USize contoursNum, GS::UInt32 *boends, GS::Int32 *inds) CAN_THROW ((GS::OutOfMemoryException));
		//Cut With half Planes
		void SetInternalData (	const CorePolygon2D&					origPoly,
								USize									contoursNum,
								GS::UInt32								*boends,
								USize									verticesNum,
								double									*coords,
								USize									arcsNum,
								PolyArcRec								*pArcs,
								const GS::Array <const EdgeUserData*>&	customInhEdgeInfo,
								GS::Int32								*inds,
								GS::Int32								*indc)																						CAN_THROW ((GS::OutOfMemoryException));

		PolygonResult CutWithHalfPlanes (	const GS::Array<HalfPlane2DData>&			cutlines,		/* -> line params. 			*/
											bool										regularizeResults,
											GS::Array<GS::SharedPtr<CorePolygon2D>>&	result, 		/* <- result polygons		*/
											bool*										fullin,		/* <- full inside 			*/
											GS::Int32* 										wrcode,		/* <- warning error code   	*/
											bool*										degen) const	/* <- one line result   	*/					CAN_THROW ((GS::OutOfMemoryException));

		//Internal Cut Elem
		GSErrCode CalcSectionsForCutElem (const GenArc&		genArc,
									GS::Array<CutParam>&	hSect) const;
		void CalcSectionsForCutElem (GS::Int32				nCoords,
									const Point2D*			pCoords,
									const DirType*			pDirs,
									bool					closed,
									GS::Array<CutParam>&	hSect) const;
		GSErrCode CalcSectionsForCutElem (Boundary			cutBoundaries,
									const GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>&	polyline,
									GS::Array<CutParam>&	hSect) const;

		void CutElemInternal (	Boundary				cutBoundaries,	// means that a linepiece overlapping cutpoly Boundary is accepted or not (opened/closed)
								const Sector&			sector,
								PieceFunc*				StorePiece,		// call back for storing pieces
								const void*				userData,		// user data for call back func.
								bool					doDifference) const;	// compute difference or section
		void CutElemInternal (	Boundary				cutBoundaries,	// means that a linepiece overlapping cutpoly Boundary is accepted or not (opened/closed)
								const GenArc&			genArc,
								PieceFunc*				StorePiece,		// call back for storing pieces
								const void*				userData,		// user data for call back func.
								bool					doDifference) const;	// compute difference or section
		void CutElemInternal (	Boundary				cutBoundaries,	// means that a linepiece overlapping cutpoly Boundary is accepted or not (opened/closed)
								const Point2D&			coord,		// spline/polyLine/polygon
								PieceFunc*				StorePiece,		// call back for storing pieces
								const void*				userData,		// user data for call back func.
								bool					doDifference) const;	// compute difference or section
		void CutElemInternal (	Boundary				cutBoundaries,	// means that a linepiece overlapping cutpoly Boundary is accepted or not (opened/closed)
								const BezierDetails&	bezier,
								PieceFunc*				StorePiece,		// call back for storing pieces
								const void*				userData,		// user data for call back func.
								bool					doDifference) const;	// compute difference or section
		void CutElemInternal (	Boundary				cutBoundaries,	// means that a linepiece overlapping cutpoly Boundary is accepted or not (opened/closed)
								const GS::SharedPtr<CorePolygon2D>&	poly2D,			// polygon (alternative form)
								GS::Array<GS::SharedPtr<CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>>>& poly2DResult,		//
								bool&					touching,
								bool					doDifference) const;	// compute difference or section
		void CutElemInternal (	Boundary				cutBoundaries,	// means that a linepiece overlapping cutpoly Boundary is accepted or not (opened/closed)
			const GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>&	polyline,
								PieceFunc*				StorePiece,		// call back for storing pieces
								const void*				userData,		// user data for call back func.
								bool					doDifference) const;	// compute difference or section
		void CutElemInternal (	Boundary				cutBoundaries,	// means that a linepiece overlapping cutpoly Boundary is accepted or not (opened/closed)
			const GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>&	poly2D,			// polygon (alternative form)
			GS::Array<GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>>& poly2DResult,		//
			bool					doDifference) const;	// compute difference or section
		
		void CutElemBorderOnlyInternal (const Sector& sector,
										GS::Array<Sector>& results,
										LineCutMode cutMode) const;
		void CutElemBorderOnlyInternal (const GenArc& genArc,
										GS::Array<GenArc>& results,
										LineCutMode cutMode) const;

		bool IsValidPiece (const GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>& polyline, Boundary cutBoundaries, bool doDifference, const GS::Array<CutParam>& hSect, Int32 actIndex, CutParam& t1, CutParam& t2) const;

	private:
		/////////////////////////////////// PG ///////////////////////////////////////
		void MakePolyExt (Geometry::PGPOLYEXT& polyExt) const;
		static void MakePolyExtArr (const GS::Array<GS::SharedPtr<CorePolygon2D>>& poly2DArray, Geometry::PGPOLYEXT	&polyExt);

		static PGEDGEEXT	GetPolyEdge (const PGPOLYEXT* polygon, GS::Int32 ind);
		static PGEDGEEXT	GetPolyEdgeArr (const PGPOLYEXT* polygon, GS::Int32 ind);

		static GSErr	NewPolygon	(PGRESPOLYEXT* polygons, const PGPOLYEXT* newPoly, PGPOLYEXT** storedPoly);
		static GSErr	NewPolygonArr		(PGRESPOLYEXT* polygons, const PGPOLYEXT* newPoly, PGPOLYEXT** storedPoly);
		static GSErr	NewPolyEdge			(PGPOLYEXT* polygon, const PGEDGEEXT* pgedge);
		static GSErr	NewPolyEdgeArr		(PGPOLYEXT* polygon, const PGEDGEEXT* pgedge);
		static GSErr	NewSector (PGRESSECTOREXT* sectors, const Sector& newSector);

		static GSErr	StoreLine (Geometry::PGRESSECTOREXT* sectors, const Sector& newSector);

		static CmpResult CmpDoubles (const void	*p1, const void	*p2);

	public:
		~CorePolygon2D ();
		CorePolygon2D (const CorePolygon2D& source)																							CAN_THROW ((GS::OutOfMemoryException, GeometryException));

	protected:
		CorePolygon2D (const CorePolygon2D& source, USize verticesCapacity, USize arcsCapacity, USize contoursCapacity)						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (Flags actFlags)																				CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (const CorePolygon2D& source, bool copyPolygonLevelData)																CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (const GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonLevelDataSource)												CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (Flags actFlags, const Box2DData& box)														CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (Flags actFlags, const Circle2D& circ)														CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (Flags actFlags, const Ellipse& ell, ArcDivParams arcDivParams)														CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (const CorePolygon2D& source, UIndex contourIdx, bool keepRegularized);

		CorePolygon2D (Flags actFlags, const GS::Array<Point2D>& coordList, const GS::Array<PolyArcRec>& arcList, const GS::Array<UIndex>& contourEndList, GS::UInt32 createFlags)		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (Flags actFlags, const Point2D* pCoords, USize coordNum, const PolyArcRec* pArcs, USize arcNum, const UIndex* pContourEnds, USize contourNum, GS::UInt32 createFlags)				CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (Flags flags, const GS::Array<CustomPolyContourDescription<VertexUserData, EdgeUserData, ContourUserData>>& contourDesc)					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolygon2D (const CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline)																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		static void Create (Flags actFlags, const GS::Array<Point2D>& coordList, const GS::Array<PolyArcRec>& arcList, const GS::Array<UIndex>& contourEndList, GS::UInt32 createFlags, GS::Array<GS::SharedPtr<CorePolygon2D>>& result)		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static void Create (Flags actFlags, const Point2D* pCoords, USize coordNum, const PolyArcRec* pArcs, USize arcNum, const UIndex* pContourEnds, USize contourNum, GS::UInt32 createFlags, GS::Array<GS::SharedPtr<CorePolygon2D>>& result)				CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static void Create (Flags flags, const GS::Array<CustomPolyContourDescription<VertexUserData, EdgeUserData, ContourUserData>>& contourDesc, GS::Array<GS::SharedPtr<CorePolygon2D>>& result)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static void Create (const CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline, GS::Array<GS::SharedPtr<CorePolygon2D>>& result)									CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		ULong GetRefCount () { return SharedObject::GetRefCount (); }

		double CalcContourPerimeter (UIndex contourIdx) const ;

	private:
		void FillCorePolygon2DFromContourDesc (const GS::Array<CustomPolyContourDescription<VertexUserData, EdgeUserData, ContourUserData>>& contourDesc)					CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		bool CopyPolygonDataVertices (Point2D* destCoords, USize& coordNum,  const GS::Array<UIndex>& reversedContourIndices, GS::UInt32 createFlags) const				CAN_THROW ((GS::OutOfMemoryException));
		void CopyPolygonDataVertices (GS::Array<Point2D>& destCoords,  const GS::Array<UIndex>& reversedContourIndices, GS::UInt32 createFlags) const;
		bool CopyPolygonDataArcs (PolyArcRec* destArcs, USize& arcNum, const GS::Array<UIndex>& reversedContourIndices, GS::UInt32 createFlags) const				CAN_THROW ((GS::OutOfMemoryException));
		void CopyPolygonDataArcs (GS::Array<PolyArcRec>& destArcs, const GS::Array<UIndex>& reversedContourIndices, GS::UInt32 createFlags) const;
		bool CopyPolygonDataContours (UIndex* destContourEnds, USize& contourNum, GS::UInt32 createFlags) const				CAN_THROW ((GS::OutOfMemoryException));
		void CopyPolygonDataContours (GS::Array<UIndex>& destContourEnds, GS::UInt32 createFlags) const;
		bool CompareGeometry (const CorePolygon2D& source) const;
	public:
		CorePolygon2D&	operator= (const CorePolygon2D& source);

		bool Compare (const CorePolygon2D& source) const;

		void Convert (GS::Array<CustomPolyContourDescription<VertexUserData, EdgeUserData, ContourUserData>>& contourDescArray) const																CAN_THROW ((GS::OutOfMemoryException));

		template <class VertexUserDataTarget, class EdgeUserDataTarget, class ContourUserDataTarget, class PolygonUserDataTarget>
		static void ConvertFromCorePolygon2D (CorePolygon2D<VertexUserDataTarget, EdgeUserDataTarget, ContourUserDataTarget, PolygonUserDataTarget>& result, 
											  const CorePolygon2D& source,
											  Poly2DUserDataConverter<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData,
																	  VertexUserDataTarget, EdgeUserDataTarget, ContourUserDataTarget, PolygonUserDataTarget>* userDataConverter);

		void CopyPolygonData (Point2D* destCoords, USize& coordNum, PolyArcRec* destArcs, USize& arcNum, UIndex* destContourEnds, USize& contourNum, const GS::Array<UIndex>& reversedContourIndices, GS::UInt32 createFlags) const				CAN_THROW ((GS::OutOfMemoryException));
		void CopyPolygonData (GS::Array<Point2D>& destCoords, GS::Array<PolyArcRec>& destArcs, GS::Array<UIndex>& destContourEnds, const GS::Array<UIndex>& reversedContourIndices, GS::UInt32 createFlags) const;

		const PolygonUserData* GetUserData () const;
		void SetUserData (const PolygonUserData* userData);


		PolygonResult SetSameDirEdgeLegalAndRegularize (bool actIsSameDirEdgeLegal, GS::Array<GS::SharedPtr<CorePolygon2D>>& result);
		PolygonResult SetForce64BitPrecision (bool force64BitPrecision, GS::Array<GS::SharedPtr<CorePolygon2D>>& result);

		bool GetSameDirEdgeLegal () const { return flags.isSameDirEdgeLegal; }
		bool GetForce64BitPrecision() const { return flags.force64BitPrecision; }
		void SetIsVertexUserData	(bool actIsUserData);
		bool GetIsVertexUserData	() const { return flags.isVertexUserData; }
		void SetIsEdgeUserData		(bool actIsUserData);
		bool GetIsEdgeUserData		() const { return flags.isEdgeUserData; }
		void SetIsContourUserData	(bool actIsUserData);
		bool GetIsContourUserData	() const { return flags.isContourUserData; }
		void SetIsPolygonUserData	(bool actIsUserData);
		bool GetIsPolygonUserData	() const { return flags.isPolygonUserData; }

		const VertexUserData& GetVertexDefaultUserData () const;
		void SetVertexDefaultUserData (const VertexUserData& userData);

		void ResetUserData ();

		USize GetVertexNum () const;
		UIndex GetVertexIdx (const Point2D& coord) const;
		const Point2D& GetCoord (UIndex vertexIdx) const;
		void SetCoord (UIndex vertexIdx, const Point2D& newCoord);
		const VertexUserData* GetVertexUserData (UIndex vertexIdx) const;
		void SetVertexUserData (UIndex vertexIdx, const VertexUserData* userData);

		const EdgeUserData& GetEdgeDefaultUserData () const;
		void SetEdgeDefaultUserData (const EdgeUserData& userData);

		USize GetEdgeNum () const { return nVertices> nContours ? nVertices - nContours : 0;}
		double GetArcAngle (UIndex edgeIdx) const;
		double GetEdgeLength (UIndex edgeIdx) const;
		bool GetSector (UIndex edgeIdx, Sector& sect, GenArc& genArc) const;
		bool GetSector (UIndex edgeIdx, Point2D& begC, Point2D& endC, double& angle) const;
		GS::Optional<Sector>	GetSector (UIndex edgeIdx) const;
		GS::Optional<GenArc>	GetArc (UIndex edgeIdx) const;
		const EdgeUserData* GetEdgeUserData (UIndex edgeIdx) const;
		void SetEdgeUserData (UIndex edgeIdx, const EdgeUserData* userData);

		const ContourUserData& GetContourDefaultUserData () const;
		void SetContourDefaultUserData (const ContourUserData& userData);

		USize GetContourNum () const { return nContours;}
		UIndex GetVertexBeginIdxInContour (UIndex contourIdx) const;
		//A contour-hoz mar nem tartozo vertex indexet adja vissza
		UIndex GetVertexEndIdxInContour (UIndex contourIdx) const;

		UIndex GetEdgeBeginIdxInContour (UIndex contourIdx) const;
		//A contour-hoz mar nem tartozo edge indexet adja vissza
		UIndex GetEdgeEndIdxInContour (UIndex contourIdx) const;

		const ContourUserData* GetContourUserData (UIndex contourIdx) const;
		void SetContourUserData (UIndex contourIdx, const ContourUserData* userData);

		UIndex GetNextVertexIndex (UIndex vertexIdx, const VertexUserData* ud) const;
		UIndex GetPrevVertexIndex (UIndex vertexIdx, const VertexUserData* ud) const;
		UIndex GetNextEdgeIndex (UIndex edgeIdx, const EdgeUserData* ud) const;
		UIndex GetPrevEdgeIndex (UIndex edgeIdx, const EdgeUserData* ud) const;
		UIndex GetNextContourIndex (UIndex contourIdx, const ContourUserData* ud) const;
		UIndex GetPrevContourIndex (UIndex contourIdx, const ContourUserData* ud) const;

		UIndex GetBeginVertexIdxOfEdge (UIndex edgeIdx) const;
		UIndex GetEndVertexIdxOfEdge (UIndex edgeIdx) const;

		UIndex GetPrevEdgeIdxOfVertex (UIndex vertexIdx) const;
		UIndex GetNextEdgeIdxOfVertex (UIndex vertexIdx) const;

		bool IsEmpty () const { return nContours == 0; }
		void Clear ();

		bool IsABox () const;
		bool IsACircle (Circle2D& circ) const;

		void RemoveSmallArcs ();

	private:
		GSErrCode	ReadVersion1Full (GS::IChannel& ic, const GS::InputFrame& frame);
		GSErrCode	ReadVersion1 (GS::IChannel& ic);
		GSErrCode	ReadVersion1UserData (GS::IChannel& ic);
		GSErrCode	ReadVersion2Full (GS::IChannel& ic, const GS::InputFrame& frame);
		GSErrCode	ReadVersion2 (GS::IChannel& ic, const GS::InputFrame& frame);
		GSErrCode	ReadVersion2UserData (GS::IChannel& ic);
		GSErrCode	WriteVersion1Full (GS::OChannel& oc, const GS::OutputFrame& frame) const;
		GSErrCode	WriteVersion1 (GS::OChannel& oc) const;
		GSErrCode	WriteVersion1UserData (GS::OChannel& oc) const;
		GSErrCode	WriteVersion2Full (GS::OChannel& oc, const GS::OutputFrame& frame) const;
		GSErrCode	WriteVersion2 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
		GSErrCode	WriteVersion2UserData (GS::OChannel& oc) const;

		GSErrCode	WriteXMLContours (GS::XMLOChannel& oc) const																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static GSErrCode ReadXMLCreateContourDesc	(GS::XMLIChannel& ic, GS::XMLITag& tagCorePolygon2D, Flags actFlags, USize contoursNum, GS::Array<CustomPolyContourDescription<VertexUserData, EdgeUserData, ContourUserData>>& polyContourDescArray)										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
	public:
		GSErrCode	Read  (GS::IChannel& ic);
		GSErrCode	Write (GS::OChannel& oc) const;

		static GS::SharedPtr<CorePolygon2D>	ReadXMLAndCreate	(GS::XMLIChannel& ic)										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		GSErrCode	WriteXML (GS::XMLOChannel& oc) const																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		//******************************* Calculate *******************************
		const Box2DData& GetBoundBox () const																								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		double CalcArea () const;
		//regularizalt polygon lyukakra negativ eredmenyt ad
		double CalcContourArea (UIndex contourIdx) const;
		bool HasPositiveOrientation (UIndex contourIdx) const;
		double CalcPerimeter () const;
		double CalcPerimeter (UIndex contourIdx) const ;
		void CalcConvexHull (const ArcDivParams& arcDivParams, GS::Array<Point2D>& convexHull) const;		
		double CalcMaxVertexFillet (UIndex vertexIdx) const																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool IsCoordOnEdge (const Point2D& c, UIndex& edgeIndex) const																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		double DistanceFromPoint (const Point2D& co) const																								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
	private:
		RelativePosition GetPositionSmallArea (const Sector& sect, Boundary boundaries) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		RelativePosition GetPositionNormal (const Sector& sect, Boundary boundaries, DetailedRelativePosition* detailed) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		RelativePosition GetPositionSmallArea (const GenArc& arc, Boundary boundaries) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		RelativePosition GetPositionNormal (const GenArc& arc, Boundary boundaries, DetailedRelativePosition* detailed) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		RelativePosition GetPositionZeroArea (const CorePolygon2D& poly, Boundary boundaries, DetailedRelativePosition* detailed) const			CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
	public:
		RelativePosition GetPosition (const Point2D& point, Boundary boundaries, DetailedRelativePosition* detailed = nullptr) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		RelativePosition GetPosition (const Sector& sect, Boundary boundaries, DetailedRelativePosition* detailed = nullptr) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		RelativePosition GetPosition (const GenArc& arc, Boundary boundaries, DetailedRelativePosition* detailed = nullptr) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		RelativePosition GetPosition (const Box2DData& box, Boundary boundaries, DetailedRelativePosition* detailed = nullptr) const				CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		RelativePosition GetPosition (const CorePolygon2D& poly, Boundary boundaries, DetailedRelativePosition* detailed = nullptr) const			CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		RelativePosition GetPosition (const CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& poly, Boundary boundaries, DetailedRelativePosition* detailed = nullptr) const			CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));

		static RelativePosition GetPosition (const GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, const Point2D& point, Boundary boundaries, DetailedRelativePosition* detailed = nullptr)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static RelativePosition GetPosition (const GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, const Sector& sect, Boundary boundaries, DetailedRelativePosition* detailed = nullptr)			CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		static RelativePosition GetPosition (const GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, const GenArc& arc, Boundary boundaries, DetailedRelativePosition* detailed = nullptr)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static RelativePosition GetPosition (const GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, const Box2DData& box, Boundary boundaries, DetailedRelativePosition* detailed = nullptr)		CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		static RelativePosition GetPosition (const GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, const CorePolygon2D& poly, Boundary boundaries, DetailedRelativePosition* detailed = nullptr)	CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		static RelativePosition GetPosition (const GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, const CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& poly, Boundary boundaries, DetailedRelativePosition* detailed = nullptr)	CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));

		PGPositionExt SegmentIn (Sector sector, GS::Array<Sector>* resSectors = nullptr) const;
		PGPositionExt SegmentOut (Sector sector, GS::Array<Sector>* resSectors = nullptr) const;
		bool Intersect (const Point2D& c, const Vector2D& dir, GS::Array<Sector>* result) const														CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		GS::FixArray<Point2D, 4> GetTransformedBoundBox (const TRANMAT& tr, const TRANMAT& invTr = Geometry::IdentityTranMat) const										CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void Visit (CorePolygon2DConstVertexVisitor &visitor) const;

	private:
		void DeleteHoleUpdateUserData (GS::SharedPtr<CorePolygon2D>& result, UIndex begVertexIdx, UIndex endVertexIdx, UIndex contourIdx) const																		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void AddHoleUpdateUserData (GS::SharedPtr<CorePolygon2D>& tmpPoly, const CorePolygon2D& poly, UIndex contourIdx, USize oldNVertices, USize numNewVertices, USize oldNContours, USize numNewContours) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void FillSegmentedArc (const ArcDivParams& arcDivParams, GS::Array< GS::Array<Point2D>>& segmentedArcs, USize& numOfSegmentedVertex) const																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void SetSegmentUp (CorePolygon2D& tmpPoly, GS::Array< GS::Array<Point2D>>& segmentedArcs) const																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void SetSegmentUp (GS::SharedPtr<CorePolygon2D>& tmpPoly, GS::Array< GS::Array<Point2D>>& segmentedArcs) const																		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult OffsetEdgesInContour (const UIndex contourIdx, const GS::HashTable<UIndex, double>& offsets, const bool multiEdgeOffset, GS::SharedPtr<CorePolygon2D>& result) const											CAN_THROW ((GS::OutOfMemoryException, GeometryException));

	public:
		//******************************* Modify polygon *******************************
		GS::SharedPtr<CorePolygon2D> DeleteHole (UIndex contourIdx) const																		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void Move (Vector2D offset)																												CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ClearVertexUserDatas ();
		void ClearEdgeUserDatas ();
		void ClearContourUserDatas ();
		void Stretch (	const Point2D& fixPoint,
						const Point2D& draggedPoint,
						const Vector2D& offsetAxisX,
						const Vector2D& offsetAxisY = Vector2D (0.0, 0.0),
						const ArcDivParams& arcDivParams = ArcDivParams (0.0))																	CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		//Ha az iveket torzitani kellene, akkor assertal
		void TransformPreserveArcs (const TRANMAT& tran)																									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void TransformPreserveArcs (const CorePolygon2D& filterWithPolygon, const TRANMAT& tran, bool& isTransformedAll)									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void TransformPreserveArcs (const GS::Array<GS::SharedPtr<CorePolygon2D>>& filterPolygonArray, const TRANMAT& tran, bool& isTransformedAll)		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		//Megtartjuk az iveket. Ha szukseges, akkor elotte transzformalni kell. Az automatikus szegmentalas gazos, mert ha eloszor szegmentalunk, akkor megvaltozhatnak a lyukak szamai, ezert az indexek rosszak lehetnak. Ha utana szegmentalunk akkor csak az eredmeny rossz
		void TransformHolePreserveArcs (const GS::Array<UIndex>& contourIdxs, const GS::Array<TRANMAT>& trans, bool keepOriginals)				CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void SegmentUp (const ArcDivParams& arcDivParams)																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void EliminateArcs ()																												CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void MoveVertex (UIndex vertexIdx, const Point2D& newPosition)																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void InsertVertex (UIndex edgeIdx, const Point2D& newPosition)																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void InsertEdge (const UIndex vertexIndex)																								CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void InsertVertexOnPolygon (const Point2D& newPosition, UIndex edgeIndex, GS::SharedPtr<CorePolygon2D>& result, UIndex& vertexIdx) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void DeleteVertex (UIndex vertexIdx)																									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void SetEdgeAngle (UIndex edgeIdx, double edgeAngle)																					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void MoveVertices (const GS::HashTable<UIndex, Point2D>& newValues)																		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ReplaceEdge (UIndex edgeIdx, const GS::Array<GenArc>& arcs, const ArcDivParams& arcDivParams)										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ReplaceEdge (UIndex edgeIdx, const GS::Array<double>& angles, const GS::Array<Point2D>& internalVerts)								CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void AddHole (const CorePolygon2D& poly, UIndex contourIdx)																				CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		template<class EdgeUserDataComparator, bool checkUserData>
		void RemoveSameDirEdge (const EdgeUserDataComparator& comparator);
		void RemoveSameDirEdge ();
		template<class EdgeUserDataComparator>
		void RemoveSameDirEdgesOnlyWithSameUserData (const EdgeUserDataComparator& comparator);
		void RemoveSameDirEdgesOnlyWithSameUserData ();

		void RemoveZeroEdgesAndArcs (double minEdgeLength, double minArcAngle);

		class ArrayRangeRotator;	// C++11 legyen a RotateVerticeIndicesInContour fuggvenyben lokalisan definialva
		void RotateVerticeIndicesInContour (UIndex iContourToRotate, UIndex iVertexToFirst);
		void SwapContourOrder (const GS::Array<UIndex>& newContourOrder);
		void SortArcsByIndices ();

		void Visit (CorePolygon2DVertexVisitor &visitor);

		//******************************* Create new polygons *******************************
		PolygonResult RemoveHoles (GS::SharedPtr<CorePolygon2D>& newPoly) const																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		//Ha az iveket torzitani kellene, akkor felszegmentalja oket
		PolygonResult Transform (const ArcDivParams& arcDivParams, const TRANMAT& tran, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult Transform (	const CorePolygon2D& filterWithPolygon,
									const ArcDivParams& arcDivParams,
									const TRANMAT& tran,
									GS::Array<GS::SharedPtr<CorePolygon2D>>& result,
									bool& isTransformedAll) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult Transform (	const GS::Array<GS::SharedPtr<CorePolygon2D>>& filterPolygonArray,
									const ArcDivParams& arcDivParams,
									const TRANMAT& tran,
									GS::Array<GS::SharedPtr<CorePolygon2D>>& result,
									bool& isTransformedAll) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult  SegmentUp (const ArcDivParams& arcDivParams, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult EliminateArcs (GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const													CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		// for regularized polygons only
		void OffsetContour (UIndex contourIdx, double offset, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const							CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void OffsetEdge (UIndex edgeIdx, double offset, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void OffsetEdgeFixArea (UIndex edgeIdx, UIndex vertexIndex, double offset, double area, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void FilletAllVertex (const double filletRadius, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void FilletVertex (UIndex vertexIdx, const double filletRadius, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void FilletVertexInContour (UIndex contourIdx, const double filletRadius, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ChamferAllVertex (const double chamferRadius, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ChamferVertex (UIndex vertexIdx, const double chamferRadius, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const				CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ChamferVertexInContour (UIndex contourIdx, const double chamferRadius, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		// for regularized and irregular polygons too, result in irregular polygon
		PolygonResult OffsetContour (UIndex contourIdx, double offset, GS::SharedPtr<CorePolygon2D>& result) const								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult OffsetEdge (UIndex edgeIdx, double offset, GS::SharedPtr<CorePolygon2D>& result) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult OffsetMultipleEdges (const GS::Array<GS::Pair<UIndex, double>>& offsets, GS::SharedPtr<CorePolygon2D>& result) const				CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult OffsetEdgeFixArea (UIndex edgeIdx, UIndex vertexIndex, double offset, double area, GS::SharedPtr<CorePolygon2D>& result) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		PolygonResult FilletAllVertex (const double filletRadius, GS::SharedPtr<CorePolygon2D>& result) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult FilletVertex (UIndex vertexIdx, const double filletRadius, GS::SharedPtr<CorePolygon2D>& result) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult FilletVertexInContour (UIndex contourIdx, const double filletRadius, GS::SharedPtr<CorePolygon2D>& result) const			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult ChamferAllVertex (const double chamferRadius, GS::SharedPtr<CorePolygon2D>& result) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult ChamferVertex (UIndex vertexIdx, const double chamferRadius, GS::SharedPtr<CorePolygon2D>& result) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult ChamferVertexInContour (UIndex contourIdx, const double chamferRadius, GS::SharedPtr<CorePolygon2D>& result) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));

	private:
		void InsertNewVerticesAndDeleteOld  (UIndex filletChamferVertexIdx, const Point2D& begFilletChamferPosition, const Point2D& endFilletChamferPosition, UIndex begContourIdx, UIndex endContourIdx, UIndex& prevVertIdx, bool isNextEdgeArc, const GenArc& nextEdgeArc, bool united1, bool united2, GS::SharedPtr<CorePolygon2D>& result) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult FilletChamferVertex  (UIndex vertexIdx, const double filletChamferRadius, const bool doFillet, GS::SharedPtr<CorePolygon2D>& result) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult FilletChamferContour (UIndex contourIdx, const double filletChamferRadius, const bool doFillet, GS::SharedPtr<CorePolygon2D>& result) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static void ConvPgstatusToReletivePos (Geometry::PGRelPolyPosExt pgstatus, Boundary boundaries, RelativePosition* relativePosition, DetailedRelativePosition* detailed); 
	public:
		void InsertVertex (UIndex edgeIdx, const Point2D& newPosition, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void MoveVertex (UIndex vertexIdx, const Point2D& newPosition, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void DeleteVertex (UIndex vertexIdx, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const											CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void SetEdgeAngle (UIndex edgeIdx, double edgeAngle, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const							CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void MoveVertices (const GS::HashTable<UIndex, Point2D>& newValues, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const				CAN_THROW ((GS::OutOfMemoryException, GeometryException));		
		void ReplaceEdge (UIndex edgeIdx, const GS::Array<GenArc>& arcs, const ArcDivParams& arcDivParams, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ReplaceEdge (UIndex edgeIdx, const GS::Array<double>& angles, const GS::Array<Point2D>& internalVerts, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool DeleteEdge (UIndex edgeIdx, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const												CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool DeleteEdge (UIndex edgeIdx, GS::SharedPtr<CorePolygon2D>& result) const															CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void AddHole (const CorePolygon2D& poly, UIndex contourIdx, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult Unify (const CorePolygon2D& poly, GS::Array<GS::SharedPtr<CorePolygon2D>>& result) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult Intersect (const CorePolygon2D& poly, GS::Array<GS::SharedPtr<CorePolygon2D>>& result, Boundary boundaries, RelativePosition* relativePosition, DetailedRelativePosition* detailed) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult Substract (const CorePolygon2D& poly, GS::Array<GS::SharedPtr<CorePolygon2D>>& result, Boundary boundaries, RelativePosition* relativePosition, DetailedRelativePosition* detailed) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		PolygonResult Cut (const HalfPlane2DData& cutline, bool regularizeResults, GS::Array<GS::SharedPtr<CorePolygon2D>>& result, bool* fullin = nullptr, GS::Int32* wrcode = nullptr, bool* degen = nullptr) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult Cut (const GS::Array<HalfPlane2DData>& cutlines, bool regularizeResults, GS::Array<GS::SharedPtr<CorePolygon2D>>& result, bool* fullin = nullptr, GS::Int32* wrcode = nullptr, bool* degen = nullptr) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult Strip (const HalfPlane2DData& cutline1, const HalfPlane2DData& cutline2, bool regularizeResults, GS::Array<GS::SharedPtr<CorePolygon2D>>& result, GS::Int32* wrcode = nullptr) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolygonResult Clip (const Box2DData& box, bool regularizeResults, GS::Array<GS::SharedPtr<CorePolygon2D>>& result, GS::Int32* wrcode = nullptr) const						CAN_THROW ((GS::OutOfMemoryException, GeometryException));

	private:
		static void StoreElem (
			short			typ,
			Sector*			pSector,
			GenArc*			pGenArc,
			GS::Int32			nCoords,		// spline/polyLine/polygon
			Point2D*			pCoords,		// spline/polyLine/polygon
			DirType*		pDirs,			// spline
			GS::UInt32*			pVertInds,		// polyLine/polygon
			GS::Int32			nEnds,			// polygon
			GS::Int32*			pEnds,			// polygon
			GS::Int32			arcsNum,		// polyLine/polygon
			PolyArcRec*		pArcs,			// polyLine/polygon
			//Polygon2DData*	pPoly,			// polygon (alternative form)
			const void*		userData)											CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static void IntersectionDetector (
			short			typ,
			Sector*			pSector,
			GenArc*			pGenArc,
			GS::Int32			nCoords,		// spline/polyLine/polygon
			Point2D*			pCoords,		// spline/polyLine/polygon
			DirType*		pDirs,			// spline
			GS::UInt32*			pVertInds,		// polyLine/polygon
			GS::Int32			nEnds,			// polygon
			GS::Int32*			pEnds,			// polygon
			GS::Int32			arcsNum,		// polyLine/polygon
			PolyArcRec*		pArcs,			// polyLine/polygon
			//Polygon2DData*	pPoly,			// polygon (alternative form)
			const void*		userData);

		/* Ezeket mar csak itt hasznalja a CutElem es a ElemPolyDifference creator nelkuli valtozta, ha ez tudna belso creator nelkul mukodni, akkor ki lehetne ezt a kettot torolni */
//		void CutElem (const GS::SharedPtr<CorePolyline2D>& polyline, Boundary cutBoundaries, CorePolyline2DCreator& creator) const				CAN_THROW ((GS::OutOfMemoryException, GeometryException));
//		void ElemPolyDifference (const GS::SharedPtr<CorePolyline2D>& polyline, Boundary cutBoundaries, CorePolyline2DCreator& creator) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));

	private:
		static void UnifyWithOnePoly (GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, PolyHoleHandling holeHandling = ReturnHoles)					CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		static void UnifyWithTwoPoly (GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, PolyHoleHandling holeHandling = ReturnHoles)					CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		bool	CheckSizes			() const;
		bool	CheckVertices		(PolyCheckFlags checkFlags, PolyCheckFlags* errorType, double coordUpperRange) const;
		bool	CheckContourEnds	(PolyCheckFlags checkFlags, PolyCheckFlags* errorType) const;
		bool	CheckArcs			(PolyCheckFlags checkFlags, PolyCheckFlags* errorType) const;

		bool	CanCalculateArcOrigo (UIndex internalArcIdx) const;

	public:

		void CutElem (const Sector& sector, Boundary cutBoundaries, SectorCreator& creator) const												CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		void CutElem (const GenArc& genArc, Boundary cutBoundaries, GenArcCreator& creator) const												CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CutElem (const Point2D& coord, Boundary cutBoundaries, PointCreator& creator) const													CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CutElem (const BezierDetails& bezier, Boundary cutBoundaries, SplineCreator& creator) const										CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void CutElem (const Sector& sector, Boundary cutBoundaries, GS::Array<Sector>& results) const											CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		void CutElem (const GenArc& genArc, Boundary cutBoundaries, GS::Array<GenArc>& results) const											CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CutElem (const Point2D& coord, Boundary cutBoundaries, GS::Array<Point2D>& results) const												CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CutElem (const BezierDetails& bezier, Boundary cutBoundaries, GS::Array<BezierDetails>& results) const								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CutElem (const GS::SharedPtr<CorePolygon2D>& polygon, Boundary cutBoundaries, GS::Array<GS::SharedPtr<CorePolygon2D>>& results) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CutElem (const GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>& polyline, Boundary cutBoundaries, GS::Array<GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>>& results) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void ElemPolyDifference (const Sector& sector, Boundary cutBoundaries, SectorCreator& creator) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		void ElemPolyDifference (const GenArc& genArc, Boundary cutBoundaries, GenArcCreator& creator) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ElemPolyDifference (const Point2D& coord, Boundary cutBoundaries, PointCreator& creator) const										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ElemPolyDifference (const BezierDetails& bezier, Boundary cutBoundaries, SplineCreator& creator) const								CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void ElemPolyDifference (const Sector& sector, Boundary cutBoundaries, GS::Array<Sector>& results) const								CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		void ElemPolyDifference (const GenArc& genArc, Boundary cutBoundaries, GS::Array<GenArc>& results) const								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ElemPolyDifference (const Point2D& coord, Boundary cutBoundaries, GS::Array<Point2D>& results) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ElemPolyDifference (const BezierDetails& bezier, Boundary cutBoundaries, GS::Array<BezierDetails>& results) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ElemPolyDifference (const GS::SharedPtr<CorePolygon2D>& polygon, Boundary cutBoundaries, GS::Array<GS::SharedPtr<CorePolygon2D>>& results) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ElemPolyDifference (const GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>& polyline, Boundary cutBoundaries, GS::Array<GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>>& results) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		bool ElemXPolygon (const Sector& sector, Boundary cutBoundaries) const																	CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		bool ElemXPolygon (const GenArc& genArc, Boundary cutBoundaries) const																	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool ElemXPolygon (const Point2D& coord, Boundary cutBoundaries) const																	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool ElemXPolygon (const BezierDetails& bezier, Boundary cutBoundaries) const															CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool ElemXPolygon (const GS::SharedPtr<CorePolygon2D>& polygon, Boundary cutBoundaries, bool& /*touching*/) const						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool ElemXPolygon (const GS::SharedPtr<CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>>& polyline, Boundary cutBoundaries) const											CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		
		void ElemPolyDiffBorderOnly (const Sector& sector, GS::Array<Sector>& results) const;
		void ElemPolyDiffBorderOnly (const GenArc& genArc, GS::Array<GenArc>& results) const;

		void IntersectElemBorderOnly (const Sector& sector, GS::Array<Sector>& results) const;
		void IntersectElemBorderOnly (const GenArc& genArc, GS::Array<GenArc>& results) const;

		static bool CreateCommonEdgeSections (CorePolygon2D& poly1, CorePolygon2D& poly2, bool toRegular, GS::Array<GS::SharedPtr<CorePolygon2D>>& result)		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void InsertCommonXLineOnPolygon (double x, bool& changed, const EdgeSplitControl& cntrl) 												CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		void InsertCommonYLineOnPolygon (double y, bool& changed, const EdgeSplitControl& cntrl) 												CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		static void Unify (GS::Array<GS::SharedPtr<CorePolygon2D>>& polygonArray, PolyHoleHandling holeHandling = ReturnHoles)					CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));
		static GS::Array<GS::SharedPtr<CorePolygon2D>> Substract (const GS::Array<GS::SharedPtr<CorePolygon2D>>&	minuendPolygons,
																  const GS::Array<GS::SharedPtr<CorePolygon2D>>&	subtrahendPolygons,
																  bool*												changed) CAN_THROW ((GS::OutOfMemoryException, GeometryException, GS::AbortException));

		static bool AUnionB (bool a, bool b);
		static bool AMinusB (bool a, bool b);

		// Debug
		void				Dump3DGDL			(Geometry::Plane * thePlane = nullptr, GS::OChannel& oc = dbChannel) const;
		template <class Output>
		void				Print			(Output& oc)	const;
		void				PrintDebug		()				const { Check (PolyCheckFlags_All_Polygon, nullptr, MaxCoordValue ()); Print (dbChannel); }
		void				PrintDebugXML	()				const { Check (PolyCheckFlags_All_Polygon, nullptr, MaxCoordValue ()); WriteXML (dbXMLChannel); }

		bool				Check			(PolyCheckFlags  checkFlags,
											 PolyCheckFlags* errorType,
											 double			 coordUpperRange) const;
		PolyRepairResult	Repair			(PolyCheckFlags repairType);
	};

	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Store (GS::ObjectState& os, const typename Geometry::CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>::Flags& flags)
	{

		bool tmpIsSameDirEdgeLegal = flags.isSameDirEdgeLegal;
		bool tmpIsVertexUserData = flags.isVertexUserData;
		bool tmpIsEdgeUserData = flags.isEdgeUserData;
		bool tmpIsContourUserData = flags.isContourUserData;
		bool tmpIsPolygonUserData = flags.isPolygonUserData;
		bool tmpForce64BitPrecision = flags.force64BitPrecision;

		os.Add ("isSameDirEdgeLegal", tmpIsSameDirEdgeLegal);
		os.Add ("isVertexUserData", tmpIsVertexUserData);
		os.Add ("isEdgeUserData", tmpIsEdgeUserData);
		os.Add ("isContourUserData", tmpIsContourUserData);
		os.Add ("isPolygonUserData", tmpIsPolygonUserData);
		os.Add ("force64BitPrecision", tmpForce64BitPrecision);
		return NoError;
	}


	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Restore (const GS::ObjectState& os, typename Geometry::CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>::Flags& flags)
	{
		bool tmpIsSameDirEdgeLegal;
		bool tmpIsVertexUserData;
		bool tmpIsEdgeUserData;
		bool tmpIsContourUserData;
		bool tmpIsPolygonUserData;
		bool tmpForce64BitPrecision;

		os.Get ("isSameDirEdgeLegal", tmpIsSameDirEdgeLegal);
		os.Get ("isVertexUserData", tmpIsVertexUserData);
		os.Get ("isEdgeUserData", tmpIsEdgeUserData);
		os.Get ("isContourUserData", tmpIsContourUserData);
		os.Get ("isPolygonUserData", tmpIsPolygonUserData);
		os.Get ("force64BitPrecision", tmpForce64BitPrecision);


		flags.isSameDirEdgeLegal = tmpIsSameDirEdgeLegal;
		flags.isVertexUserData = tmpIsVertexUserData;
		flags.isEdgeUserData = tmpIsEdgeUserData;
		flags.isContourUserData = tmpIsContourUserData;
		flags.isPolygonUserData = tmpIsPolygonUserData;
		flags.force64BitPrecision = tmpForce64BitPrecision;
		return NoError;
	}

	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Read (GS::IChannel& ic, typename Geometry::CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>::Flags& flags)
	{
		GSErrCode err = ic.Read (flags.isSameDirEdgeLegal);
		if (err == NoError) {
			err = ic.Read (flags.isVertexUserData);
		}
		if (err == NoError) {
			err = ic.Read (flags.isEdgeUserData);
		}
		if (err == NoError) {
			err = ic.Read (flags.isContourUserData);
		}
		if (err == NoError) {
			err = ic.Read (flags.isPolygonUserData);
		}
		if (err == NoError) {
			err = ic.Read (flags.force64BitPrecision);
		}
		return err;
	}


	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Write (GS::OChannel& oc, const typename Geometry::CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>::Flags& flags)
	{
		GSErrCode err = oc.Write (flags.isSameDirEdgeLegal);
		if (err == NoError) {
			err = oc.Write (flags.isVertexUserData);
		}
		if (err == NoError) {
			err = oc.Write (flags.isEdgeUserData);
		}
		if (err == NoError) {
			err = oc.Write (flags.isContourUserData);
		}
		if (err == NoError) {
			err = oc.Write (flags.isPolygonUserData);
		}
		if (err == NoError) {
			err = oc.Write (flags.force64BitPrecision);
		}
		return err;
	}


	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Store (GS::ObjectState& os, const typename Geometry::CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>::Status& status)
	{
		os.Add ("isRegularized", status.isRegularized);
		return NoError;
	}


	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Restore (const GS::ObjectState& os, typename Geometry::CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>::Status& status)
	{
		bool tmp;
		os.Get ("isRegularized", tmp);
		status.isRegularized = tmp;
		return NoError;
	}


	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Read (GS::IChannel& ic, typename Geometry::CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>::Status& status)
	{
		GSErrCode err = ic.Read (status.isRegularized);
		return err;
	}


	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Write (GS::OChannel& oc, const typename Geometry::CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>::Status& status)
	{
		GSErrCode err = oc.Write (status.isRegularized);
		return err;
	}

	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Store (GS::ObjectState& os, const CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& polygon)
	{
		os.Add ("boundBox", polygon.boundBox);
		Store<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData> (os, polygon.flags);
		Store<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData> (os, polygon.status);
		os.Add ("inhPolygonInfo", polygon.inhPolygonInfo);
		os.Add ("defaultVertexData", polygon.defaultVertexData);
		os.Add ("defaultEdgeData", polygon.defaultEdgeData);
		os.Add ("nVertices", polygon.nVertices);
		os.Add ("vertexInfos", polygon.vertexInfos);
		os.Add ("nArcs", polygon.nArcs);
		os.Add ("arcs", polygon.arcs);
		os.Add ("inhEdgeInfos", polygon.inhEdgeInfos);
		os.Add ("nContours", polygon.nContours);
		os.Add ("contourEnds", polygon.contourEnds);
		os.Add ("contourInfos", polygon.contourInfos);
		return NoError;
	}


	template<class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData>
	GSErrCode	Restore (const GS::ObjectState& os, CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& polygon)
	{
		os.Get ("boundBox", polygon.boundBox);
		Restore<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData> (os, polygon.flags);
		Restore<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData> (os, polygon.status);
		os.Get ("inhPolygonInfo", polygon.inhPolygonInfo);
		os.Get ("defaultVertexData", polygon.defaultVertexData);
		os.Get ("defaultEdgeData", polygon.defaultEdgeData);
		os.Get ("nVertices", polygon.nVertices);
		os.Get ("vertexInfos", polygon.vertexInfos);
		os.Get ("nArcs", polygon.nArcs);
		os.Get ("arcs", polygon.arcs);
		os.Get ("inhEdgeInfos", polygon.inhEdgeInfos);
		os.Get ("nContours", polygon.nContours);
		os.Get ("contourEnds", polygon.contourEnds);
		os.Get ("contourInfos", polygon.contourInfos);
		return NoError;
	}

}	// namespace Geometry


#endif //GEOMETRY_COREPOLYGON2D_CLASSDEFINITION_HPP
