// *****************************************************************************
//
// Declaration of CorePolyline2D class
//
// Module:			Geometry
// Namespace:		Geometry
// Contact person:	RA, RJ
//
// *****************************************************************************

#ifndef GEOMETRY_COREPOLYLINE2D_CLASSDEFINITION_HPP
#define GEOMETRY_COREPOLYLINE2D_CLASSDEFINITION_HPP

#pragma once

// --- Includes ----------------------------------------------------------------

// from GSRoot
#include "Algorithms.hpp"
#include "DoubleClassification.hpp"

//own
#include "Poly2D.hpp"
#include "Curve2DData.h"
#include "CutElems.h"
#include "HalfPlane2DData.h"
#include "Cut2DInternal.h"
#include "NurbsCurve2D.hpp"

namespace Geometry {
	template <class VertexUserData, class EdgeUserData, class ContourUserData, class PolygonUserData> class CorePolygon2D;
}

namespace Geometry {
	template <class VertexUserData, class EdgeUserData, class PolygonUserData>
	class CorePolyline2D;

	template <class VertexUserData, class EdgeUserData, class PolygonUserData>
	GSErrCode	Store (GS::ObjectState& os, const CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline);
	
	template <class VertexUserData, class EdgeUserData, class PolygonUserData>
	GSErrCode	Restore (const GS::ObjectState& os, CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline);

	class GEOMETRY_DLL_EXPORT CorePolyline2DPrivate {
	public:
		static GS::ClassInfo	classInfo;
	};

	class GEOMETRY_DLL_EXPORT CorePolyline2DVertexVisitor
	{
	public:
		virtual ~CorePolyline2DVertexVisitor ();

		virtual void operator () (Point2D &) = 0;
	};
	class GEOMETRY_DLL_EXPORT CorePolyline2DConstVertexVisitor
	{
	public:
		virtual ~CorePolyline2DConstVertexVisitor ();

		virtual void operator () (const Point2D &) = 0;
	};

	template <class VertexUserData, class EdgeUserData, class PolygonUserData>
	class CorePolyline2D : public GS::SharedObject
	{
		template <class VertexUserData1, class EdgeUserData1, class PolygonUserData1>
		friend class CorePolyline2D;
		template <class VertexUserData1, class EdgeUserData1, class PolygonUserData1>
		friend class CustomPolyline2D;
		template <class VertexUserData1, class EdgeUserData1, class ContourUserData1, class PolygonUserData1>
		friend class CorePolygon2D;

		friend GSErrCode	Store<VertexUserData, EdgeUserData, PolygonUserData> 	(GS::ObjectState& os, const CorePolyline2D& polyline);
		friend GSErrCode	Restore<VertexUserData, EdgeUserData, PolygonUserData> 	(const GS::ObjectState& os, CorePolyline2D& polyline);
		friend GSErrCode	Store 	(GS::ObjectState& os, const Geometry::CustomPolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline);
		friend GSErrCode	Restore (const GS::ObjectState& os, Geometry::CustomPolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline);

	public:
		enum PolylineResult {
			NewPolylines,
			EmptyPolyline,
			ResultSameAsThis,
			ResultSameAsFirstParameter
		};

		enum PolylinePointPosition {PointOnRightSide, PointOnPolyline, PointOnLeftSide};

		struct Flags {
			Flags ()
			{
				Clear ();
			}
			void Clear (void)
			{
				BNZeroMemory (this, sizeof (Flags));
				isSameDirEdgeLegal = true;
				isClosed = false;
				isSameCoord = false;
				isVertexUserData = true;
				isEdgeUserData = true;
				isPolygonUserData = true;
			}
			GS::UInt16					isSameDirEdgeLegal		: 1; // Set if two succeeding edge with same direction (two edge in one line) is legal
			GS::UInt16					isClosed				: 1;
			GS::UInt16					isSameCoord				: 1;
			GS::UInt16					isVertexUserData		: 1;
			GS::UInt16					isEdgeUserData			: 1;
			GS::UInt16					isPolygonUserData		: 1;
		};

	private:
		struct NurbsRec {
			UIndex		begIndex;
			NurbsCurve2D		nurbs;

			NurbsRec () : begIndex (0) {};
		};

		Box2DData					boundBox;			// Boundbox of the whole polygon
		Flags						flags;

		GS::Array <PolygonUserData>	inhPolygonInfo;		// Array containing the attached polygon informations (inherited)
		VertexUserData				defaultVertexData;
		EdgeUserData				defaultEdgeData;

		USize						nVertices;			// Total number of all vertices in all contours (equals to num of edges)
		GS::Array <Point2D>			vertices;			// Array containing the vertices
		GS::Array <VertexUserData>	vertexInfos;		// Array containing the attached vertex informations (eg. vertID)

		USize						nArcs;				// Total number of curved edges
		GS::Array <PolyArcRec>		arcs;				// the view angle of arcs (sign: + means CCW, - means CW) and the connecting vertices
		GS::Array <EdgeUserData>	inhEdgeInfos;		// Array containing the attached edge informations inherited by all descendant (eg. height)

		USize						nNurbs;				// Total number of Nurbs
		GS::Array <NurbsRec>		nurbs;

		void InitInternalData (void);
		void FreeInternalData (void)																																	CAN_THROW ((GS::OutOfMemoryException));
		void ReallocateInternalDataSizes (USize verticesNum, USize arcsNum, USize nurbsNum, USize verticesCapacity, USize arcsCapacity, USize nurbsCapacity)			CAN_THROW ((GS::OutOfMemoryException));
		void AllocateInternalData (USize verticesNum, USize arcsNum, USize nurbsNum, USize verticesCapacity = 0, USize arcsCapacity = 0, USize nurbsCapacity = 0)		CAN_THROW ((GS::OutOfMemoryException));
		void ReallocateInternalData (USize verticesNum, USize arcsNum, USize nurbsNum, USize verticesCapacity = 0, USize arcsCapacity = 0, USize nurbsCapacity = 0)		CAN_THROW ((GS::OutOfMemoryException));

		void SetInternalDataVertices (const Point2D* pCoords, USize coordNum, GS::UInt32 createFlags);
		void SetInternalDataVertices (const GS::Array<Point2D>& coordList, USize coordNum, GS::UInt32 createFlags);
		void SetInternalDataArcs (const PolyArcRec* pArcs, USize arcNum, GS::UInt32 createFlags);
		void SetInternalDataArcs (const GS::Array<PolyArcRec>& arcList, USize arcNum, GS::UInt32 createFlags);
		void SetInternalDataNurbs (const NurbsRec* pNnurbs, USize nurbsNum);
		void SetInternalDataNurbs (const GS::Array<NurbsRec>& inNurbs, USize nurbsNum);
		void SetInternalData (const Point2D* pCoords, USize coordNum, const PolyArcRec* pArcs, USize arcNum, const NurbsRec* pNurbs, USize nurbsNum, GS::UInt32 createFlags);
		void SetInternalData (const GS::Array<Point2D>& coordList, const GS::Array<PolyArcRec>& arcList, const GS::Array<NurbsRec>& inNurbs, GS::UInt32 createFlags);

		template <class VertexUserData1, class EdgeUserData1, class PolygonUserData1>
		void CopyInternalDataFromWithoutUserData (const CorePolyline2D<VertexUserData1, EdgeUserData1, PolygonUserData1>& source, USize verticesCapacity, USize arcsCapacity, USize nurbsCapacity)		CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void CopyInternalDataFromClearUserData (void)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		template <class ContourUserData>
		void CopyInternalDataFromUserData (const CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& source, UIndex fromVertex, UIndex begVertexIdx, USize newVerticesNum)			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CopyInternalDataFrom (const CorePolyline2D& source)																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		template <class ContourUserData>
		void CopyInternalDataFrom (const CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& source, UIndex contourIdx)															CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void CopyPolygonLevelDataFrom (const CorePolyline2D& source);

		void AddOneVertex (const Point2D& coord, const VertexUserData* vertexInfo, const EdgeUserData* edgeInfo, const PolyArcRec* arc = nullptr, const NurbsRec* nurbs = nullptr);

		UIndex GetInternalArcIdx (UIndex edgeIdx) const;
		UIndex GetInternalSector (UIndex edgeIdx, Point2D& begC, Point2D& endC, Point2D& origo, double& angle) const;
		UIndex GetInternalNurbsIdx (UIndex edgeIdx) const;
		void GetCoord (const CutParam& cutParam, Point2D *c) const;

		void DeleteVertexFromInternalData (UIndex remIndex);
		void ReplaceEdgeFromInternalData (UIndex insIndex, const GenArc& genArc, const ArcDivParams& arcDivParams);
		void InsertVertexFromInternalData (UIndex insIndex, const Point2D& newPosition, bool keepArc = false, bool insArc = false);
		static GS::Int32 LineLineToDeleteEdge (const Sector &prevEdgeLine, const Sector &nextEdgeLine, Point2D *result);
		void SetEdgeArcToDeleteEdge (GS::Int32& arcInd, bool& isEdgeArc, GenArc& edgeArc, Point2D& edgeArcCentre, double& edgeArcViewAngle, UIndex edgeIdx, const Point2D& vertFromPt, const Point2D& vertFromTo) const;
		bool PolylineDeleteEdge (UIndex edgeIdx)																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void PolylineFilletChamferContour (double filletChamferRadius, bool doFillet)														CAN_THROW ((GS::LogicErrorException, GS::OutOfMemoryException, GeometryException));
		static double GetMaxMid (bool twocirc, const Circle2D* ci1, const GenArc*	arc1, const Circle2D* ci2, const GenArc* arc2, const Sector*	ls);
		void GetMaxVertexFillet (UIndex filletedVertexIdx, double* maxFilletRadius) const;
		void PolylineFilletChamferVertex (UIndex filletChamferVertexIdx, double filletChamferRadius, bool doFillet)							CAN_THROW ((GS::LogicErrorException, GS::OutOfMemoryException, GeometryException));
		void GetOffsetLimit (double *offset) const;
		void OffsetAllEdge (double offset);
		void GetOffsetLimitEdge (double *offset, UIndex edgeId) const;
		void OffsetOneEdge (double offset, UIndex edgeId, bool *degen1, Point2D *degenC1, bool *degen2, Point2D *degenC2);
		void DropNurbs (void);

		//Cut With half Planes
		void SetInternalData (	const CorePolyline2D&					origPoly,
								const USize								verticesNum,
								const double*							coords,
								const USize								arcsNum,
								const PolyArcRec*						pArcs,
								const USize								nurbsNum,
								const NurbsRec*							inNurbs,
								const GS::Array <const EdgeUserData*>&	customInhEdgeInfo,
								const GS::Int32*						inds,
								const GS::Int32*						indc)																CAN_THROW ((GS::OutOfMemoryException));
		PolylineResult	CutWithHalfPlanes (	const GS::Array<HalfPlane2DData>&			cutlines,		/* -> line params. 			*/
											GS::Array<GS::SharedPtr<CorePolyline2D> >&	respoly, 		/* <- result polygons		*/
											bool										*fullin,		/* <- full inside 			*/
											GS::Int32 									*wrcode) const	/* <- warning error code   	*/		CAN_THROW ((GS::OutOfMemoryException));

		Box2DData CalcBoundBox (void) const;
		void UpdateBoundingBox (void);

		template<class EdgeUserDataComparator, bool checkUserData>
		void RemoveSameDirEdge (const EdgeUserDataComparator& comparator);
		void RemoveSameDirEdge ();

		void Regularize (void);
		void DeleteNearPoints (void);

	public:
		~CorePolyline2D ();

		CorePolyline2D (const CorePolyline2D& source);
		template <class ContourUserData>
		CorePolyline2D (const CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& source, UIndex contourIdx);

	protected:
		CorePolyline2D (Flags actFlags);
		CorePolyline2D (const CorePolyline2D& source, bool copyPolygonLevelData);

		CorePolyline2D (Flags actFlags, const Box2DData& box);
		CorePolyline2D (Flags actFlags, const Point2D* pCoords, USize coordNum, const PolyArcRec* pArcs, USize arcNum, GS::UInt32 createFlags);
		CorePolyline2D (Flags actFlags, const Point2D* pCoords, USize coordNum, const PolyArcRec* pArcs, USize arcNum, UIndex begCoordIdx, UIndex endCoordIdx, GS::UInt32 createFlags);
		CorePolyline2D (Flags actFlags, const GS::Array<Point2D>& coordList, const GS::Array<PolyArcRec>& arcList, GS::UInt32 createFlags)		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CorePolyline2D (Flags actFlags, const GS::Array<Point2D>& coordList, const GS::Array<PolyArcRec>& arcList, UIndex begCoordIdx, UIndex endCoordIdx, GS::UInt32 createFlags);
		CorePolyline2D (Flags actFlags, const GenArc genArc, const ArcDivParams& arcDivParams);

		CorePolyline2D (Flags actFlags, const CustomPolyContourDescription<VertexUserData, EdgeUserData, PolygonUserData> & contourDesc);

		static void Create (Flags actFlags, const Point2D* pCoords, USize coordNum, const PolyArcRec* pArcs, USize arcNum, const UIndex* pContourEnds, USize contourNum, GS::UInt32 createFlags, GS::Array<GS::SharedPtr<CorePolyline2D> >& result)	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static void Create (Flags actFlags, const GS::Array<Point2D>& coordList, const GS::Array<PolyArcRec>& arcList, const GS::Array<UIndex> contourEndList, GS::UInt32 createFlags, GS::Array<GS::SharedPtr<CorePolyline2D> >& result)	CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		ULong GetRefCount (void) { return SharedObject::GetRefCount (); }

		bool IsFirstLastCoordEqual (void) const;

	private:
		void FillCorePolyline2DFromContourDesc (const CustomPolyContourDescription<VertexUserData, EdgeUserData, PolygonUserData>& contourDesc)					CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		bool CopyPolylineDataVertices (Point2D* destCoords, USize& coordNum,  GS::UInt32 createFlags) const			CAN_THROW ((GS::OutOfMemoryException));
		bool CopyPolylineDataArcs (PolyArcRec* destArcs, USize& arcNum, GS::UInt32 createFlags) const				CAN_THROW ((GS::OutOfMemoryException));
		bool CopyPolylineDataNurbs (GS::Array<NurbsCurve2D>& destNurbs) const												CAN_THROW ((GS::OutOfMemoryException));
		bool CompareGeometry (const CorePolyline2D& source) const;
	public:
		CorePolyline2D&	operator= (const CorePolyline2D& source);

		bool Compare (const CorePolyline2D& source) const;

		void Convert (CustomPolyContourDescription<VertexUserData, EdgeUserData, PolygonUserData >& contourDesc) const																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		template <class VertexUserData1, class EdgeUserData1, class PolygonUserData1>
		static void ConvertFromCorePolyline2D (CorePolyline2D<VertexUserData1, EdgeUserData1, PolygonUserData1>& result, const CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& source)											CAN_THROW ((GS::OutOfMemoryException, GeometryException));	

		void CopyPolylineData (Point2D* destCoords, USize& coordNum, PolyArcRec* destArcs, USize& arcNum, GS::Array<NurbsCurve2D>& destNurbs, GS::UInt32 createFlags) const				CAN_THROW ((GS::OutOfMemoryException));
		void CopyPolylineData (GS::Array<Point2D>& destCoords, GS::Array<PolyArcRec>& destArcs, GS::Array<NurbsCurve2D>& destNurbsList, GS::UInt32 createFlags) const;

		void ResetUserData (void);

		void SetUserData (const PolygonUserData* userData);
		const PolygonUserData* GetUserData (void) const;

		void SetSameDirEdgeLegal (bool isSameDirEdgeLegal);
		bool GetSameDirEdgeLegal (void) const { return flags.isSameDirEdgeLegal; }

		void SetIsVertexUserData	(bool actIsUserData);
		bool GetIsVertexUserData	() const { return flags.isVertexUserData; }
		void SetIsEdgeUserData		(bool actIsUserData);
		bool GetIsEdgeUserData		() const { return flags.isEdgeUserData; }
		void SetIsPolygonUserData	(bool actIsUserData);
		bool GetIsPolygonUserData	() const { return flags.isPolygonUserData; }

		void SetClosed			(bool closed)																										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool GetClosed			(void) const { return flags.isClosed; }
		void DetectAndSetClosed (void);

		void SetIsSameCoord		(bool isSameCoord);
		bool GetIsSameCoord		() const { return flags.isSameCoord; }

		const VertexUserData& GetVertexDefaultUserData (void) const;
		void SetVertexDefaultUserData (const VertexUserData& userData);

		USize GetVertexNum (void) const;
		UIndex GetVertexIdx (const Point2D& coord) const;
		const Point2D& GetCoord (UIndex vertexIdx) const;
		void SetCoord (UIndex vertexIdx, const Point2D& newCoord);
		const VertexUserData* GetVertexUserData (UIndex vertexIdx) const;
		void SetVertexUserData (UIndex vertexIdx, const VertexUserData* userData);

		const EdgeUserData& GetEdgeDefaultUserData (void) const;
		void SetEdgeDefaultUserData (const EdgeUserData& userData);

		USize GetEdgeNum (void) const { return nVertices > 0 ? nVertices - 1 : 0; }
		double GetArcAngle (UIndex edgeIdx) const;
		double GetEdgeLength (UIndex edgeIdx) const;
		NurbsCurve2D GetNurbs (UIndex edgeIdx) const;
		bool GetSector (UIndex edgeIdx, Sector& sect, GenArc& genArc) const;
		bool GetSector (UIndex edgeIdx, Point2D& begC, Point2D& endC, double& angle) const;
		GS::Optional<Sector>	GetSector	(UIndex edgeIdx) const;
		GS::Optional<GenArc>	GetArc		(UIndex edgeIdx) const;
		const EdgeUserData* GetEdgeUserData (UIndex edgeIdx) const;
		void SetEdgeUserData (UIndex edgeIdx, const EdgeUserData* userData);

		UIndex GetNextVertexIndex (UIndex vertexIdx, const VertexUserData* ud) const;
		UIndex GetPrevVertexIndex (UIndex vertexIdx, const VertexUserData* ud) const;
		UIndex GetNextEdgeIndex (UIndex edgeIdx, const EdgeUserData* ud) const;
		UIndex GetPrevEdgeIndex (UIndex edgeIdx, const EdgeUserData* ud) const;

		UIndex GetBeginVertexIdxOfEdge (UIndex edgeIdx) const;
		UIndex GetEndVertexIdxOfEdge (UIndex edgeIdx) const;

		UIndex GetPrevEdgeIdxOfVertex (UIndex vertexIdx) const;
		UIndex GetNextEdgeIdxOfVertex (UIndex vertexIdx) const;

		bool IsEmpty (void) const { return nVertices == 0; }
		void Clear (void);

		// returns true, if the polyline is a rectangle and its edges are parallel with the X and Y axises
		bool IsABox (void) const;

		void RemoveSmallArcs (void);

	private:
		GSErrCode	ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
		GSErrCode	ReadVersion1UserData (GS::IChannel& ic);
		GSErrCode	ReadVersion2 (GS::IChannel& ic, const GS::InputFrame& frame);
		GSErrCode	ReadVersion2UserData (GS::IChannel& ic);
		GSErrCode	ReadVersion3 (GS::IChannel& ic, const GS::InputFrame& frame);

		GSErrCode	WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
		GSErrCode	WriteVersion1UserData (GS::OChannel& oc) const;
		GSErrCode	WriteVersion2 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
		GSErrCode	WriteVersion2UserData (GS::OChannel& oc) const;
		GSErrCode	WriteVersion3 (GS::OChannel& oc, const GS::OutputFrame& frame) const;

		GSErrCode	WriteXMLContour (GS::XMLOChannel& oc) const																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		static GSErrCode ReadXMLCreateContourDesc	(GS::XMLIChannel& ic, GS::XMLITag& tagSegments, Flags actFlags, CustomPolyContourDescription<VertexUserData, EdgeUserData, PolygonUserData>& polyContourDesc, bool& isEdgeTag)										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
	public:
		GSErrCode	Read (GS::IChannel& ic);
		GSErrCode	Write (GS::OChannel& oc) const;

		static GS::SharedPtr<CorePolyline2D>	ReadXMLAndCreate	(GS::XMLIChannel& ic)								CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		GSErrCode	WriteXML (GS::XMLOChannel& oc) const																					CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		//******************************* Calculate *******************************
		const Box2DData& GetBoundBox (void) const																							CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		double CalcArea (bool signedArea = false) const;
		bool IsClockwise (void) const;
		double CalcLength (void) const;
		double CalcMaxVertexFillet (UIndex vertexIdx) const																					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool IsCoordOnEdge (const Point2D& c, GS::Array<UIndex>& edgeIndex) const															CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		GS::Array<GS::Pair<UIndex, Vector2D>> GetTangentAtPoint (const Point2D& c) const													CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		Point2D GetMiddlePoint (void) const																									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		GS::FixArray<Point2D, 4>	GetTransformedBoundBox (const TRANMAT& tr, const TRANMAT& invTr = Geometry::IdentityTranMat) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		double DistanceFromPoint (const Point2D& co, UIndex& edgeOfMin, GS::Array<bool>* filter) const										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void Visit (CorePolyline2DConstVertexVisitor &visitor) const;
		double GetTurningAngleAtVertex (UIndex vertexIdx) const;

	private:
		void FillSegmentedArc (const ArcDivParams& arcDivParams, GS::Array< GS::Array<Point2D> >& segmentedArcs, USize& numOfSegmentedVertex) const																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void FillSegmentedNurbs (const ArcDivParams& arcDivParams, GS::Array< GS::Array<Point2D> >& segmentedNurbs, USize& numOfSegmentedVertex) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void SetSegmentUp (GS::SharedPtr<CorePolyline2D>& result, GS::Array< GS::Array<Point2D> >& segmentedArcs, GS::Array< GS::Array<Point2D> >& segmentedNurbs) const																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));

	public:
		//******************************* Modify polyline *******************************
		void Move (Vector2D offset)																											CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		//Ha az iveket torzitani kellene, akkor assertal
		void TransformPreserveArcs (const TRANMAT& tran)																					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ClearVertexUserDatas (void);
		void ClearEdgeUserDatas (void);
		void Reverse (void);  //Change vertex order

		void Stretch (	const Point2D& fixPoint,
						const Point2D& draggedPoint,
						const Vector2D& offsetAxisX,
						const Vector2D& offsetAxisY = Vector2D (0.0, 0.0),
						const ArcDivParams& arcDivParams = ArcDivParams (0.0))																CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		class ArrayRangeRotator;	// C++11 legyen a RotateVerticeIndicesInContour fuggvenyben lokalisan definialva
		void RotateVerticeIndices (UIndex iVertexToFirst);
		void SortArcsByIndices (void);

		void Append (const Point2D& c, const bool atEnd)																						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void Append (const CorePolyline2D& polylineToAppend)																				CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void Visit (CorePolyline2DVertexVisitor &visitor);

		//******************************* Create new polyline(s) *******************************

			//Ha az iveket torzitani kellene, akkor nurbsse konvertalja oket
		void ArcToNurbs ();
		PolylineResult Transform (const ArcDivParams& arcDivParams, const TRANMAT& tran, GS::SharedPtr<CorePolyline2D>& result) const					CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		template <class ContourUserData>
		PolylineResult Transform (const CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData>& filterWithPolygon, const ArcDivParams& arcDivParams, const TRANMAT& tran, GS::SharedPtr<CorePolyline2D>& result, bool& isTransformedAll) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		template <class ContourUserData>
		PolylineResult Transform (const GS::Array<GS::SharedPtr<CorePolygon2D<VertexUserData, EdgeUserData, ContourUserData, PolygonUserData> > >& filterPolygonArray, const ArcDivParams& arcDivParams, const TRANMAT& tran, GS::SharedPtr<CorePolyline2D>& result, bool& isTransformedAll) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolylineResult SegmentUp (const ArcDivParams& arcDivParams, GS::SharedPtr<CorePolyline2D>& result) const										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolylineResult EliminateArcs (GS::SharedPtr<CorePolyline2D>& result) const											CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void Reverse (GS::SharedPtr<CorePolyline2D>& result) const																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void OffsetAllEdge (double offset, GS::SharedPtr<CorePolyline2D>& result, double* newOffset) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolylineResult OffsetEdge (UIndex edgeIdx, double offset, GS::SharedPtr<CorePolyline2D>& result) const								CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		void MoveVertex (UIndex vertexIdx, const Point2D& newPosition, GS::SharedPtr<CorePolyline2D>& result) const							CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void FilletAllVertex (const double filletRadius, GS::SharedPtr<CorePolyline2D>& result) const										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void FilletVertex (UIndex vertexIdx, const double filletRadius, GS::SharedPtr<CorePolyline2D>& result) const						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ChamferAllVertex (const double chamferRadius, GS::SharedPtr<CorePolyline2D>& result) const										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ChamferVertex (UIndex vertexIdx, const double chamferRadius, GS::SharedPtr<CorePolyline2D>& result) const						CAN_THROW ((GS::OutOfMemoryException, GeometryException));
	private:
		void InsertNewVerticesAndDeleteOld  (UIndex filletChamferVertexIdx, const Point2D& begFilletChamferPosition, const Point2D& endFilletChamferPosition, UIndex begContourIdx, UIndex endContourIdx, UIndex& prevVertIdx, bool isPrevEdgeArc, GS::Int32 arcIndBeg, double newPrevEdgeArcViewAngle, bool isNextEdgeArc, GS::Int32 arcIndEnd, double newNextEdgeArcViewAngle, bool united1, bool united2)	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
	public:
		void InsertVertex (UIndex edgeIdx, const Point2D& newPosition, GS::SharedPtr<CorePolyline2D>& result) const							CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void InsertVertexOnPolyline (const Point2D& newPosition, UIndex edgeIndex, GS::SharedPtr<CorePolyline2D>& result, UIndex& vertexIdx) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void DeleteVertex (UIndex vertexIdx, GS::SharedPtr<CorePolyline2D>& result) const													CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void SetEdgeAngle (UIndex edgeIdx, double edgeAngle, GS::SharedPtr<CorePolyline2D>& result) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ReplaceEdge (UIndex edgeIdx,const GenArc& genArc, const ArcDivParams& arcDivParams, GS::SharedPtr<CorePolyline2D>& result) const									CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		bool DeleteEdge (UIndex edgeIdx, GS::SharedPtr<CorePolyline2D>& result) const														CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		void ShiftPolyline (double length, bool firstEdge, GS::SharedPtr<CorePolyline2D>& result) const										CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolylineResult CutAtVertices (GS::Array<UIndex>& vertexIdxArray, GS::Array<GS::SharedPtr<CorePolyline2D> >& result) const			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolylineResult Cut (const HalfPlane2DData& cutline, GS::Array<GS::SharedPtr<CorePolyline2D> >& result, bool* fullin = nullptr, GS::Int32* wrcode = nullptr) const		CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolylineResult Strip (const HalfPlane2DData& cutline1, const HalfPlane2DData& cutline2, GS::Array<GS::SharedPtr<CorePolyline2D> >& result, GS::Int32* wrcode = nullptr) const	CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolylineResult Clip (const Box2DData& box, GS::Array<GS::SharedPtr<CorePolyline2D> >& result, GS::Int32* wrcode = nullptr) const				CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		PolylineResult CutPiece (const CutParam& t1, const CutParam& t2, GS::SharedPtr<CorePolyline2D>& result) const;
		GS::Array<CutParam> CutPointToCutParam (const Point2D& cutPoint) const																CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		Point2D	CutParamToCutPoint (const CutParam& cutParam) const																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		double	CutParamToDistance (const CutParam& cutParam) const																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));
		CutParam	DistanceToCutParam (double distance)	const																			CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		template<class EdgeUserDataComparator>
		void RemoveSameDirEdgesOnlyWithSameUserData (const EdgeUserDataComparator& comparator);
		void RemoveSameDirEdgesOnlyWithSameUserData ();

	private:
		bool	CheckSizes			(void) const;
		bool	CheckVertices		(PolyCheckFlags checkFlags, PolyCheckFlags* errorType, double coordUpperRange) const;
		bool	CheckArcs			(PolyCheckFlags checkFlags, PolyCheckFlags* errorType) const;
		bool	CheckNurbs			() const;

	public:
		PolylinePointPosition	PointPosition (const Point2D& sidePoint) const																CAN_THROW ((GS::OutOfMemoryException, GeometryException));

		// Debug
		void				Print			(GS::OChannel& oc)	const;
		void				PrintDebug		(void)				const { Check (PolyCheckFlags_All_Polyline, nullptr, MaxCoordValue ()); Print (dbChannel); }
		void				PrintDebugXML	(void)				const { Check (PolyCheckFlags_All_Polyline, nullptr, MaxCoordValue ()); WriteXML (dbXMLChannel); }

		bool				Check			(PolyCheckFlags  checkFlags,
											 PolyCheckFlags* errorType,
											 double			 coordUpperRange) const;
		PolyRepairResult	Repair			(PolyCheckFlags repairType);

		
		GSErrCode	Store (GS::ObjectState& os, const NurbsRec& nurbs) const
		{
			os.Add ("begIndex", nurbs.begIndex);
			os.Add ("nurbs", nurbs.nurbs);
			return NoError;
		}
		
		GSErrCode	Restore (const GS::ObjectState& os, NurbsRec& nurbs)
		{
			os.Get ("begIndex", nurbs.begIndex);
			os.Get ("nurbs", nurbs.nurbs);
			return NoError;
		}
		
		GSErrCode	Read (GS::IChannel& ic, NurbsRec& nurbs)
		{
			GSErrCode err = ic.Read (nurbs.begIndex);
			if (err == NoError)
				err = nurbs.nurbs.Read (ic);
			return err;
		}

		GSErrCode	Write (GS::OChannel& oc, const NurbsRec& nurbs) const
		{
			GSErrCode err = oc.Write (nurbs.begIndex);
			if (err == NoError)
				err = nurbs.nurbs.Write (oc);
			return err;
		}
	};

	template<class VertexUserData, class EdgeUserData, class PolygonUserData>
	GSErrCode	Store (GS::ObjectState& os, const typename Geometry::CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>::Flags& flags)
	{

		bool tmpIsSameDirEdgeLegal = flags.isSameDirEdgeLegal;
		bool tmpIsVertexUserData = flags.isVertexUserData;
		bool tmpIsEdgeUserData = flags.isEdgeUserData;
		bool tmpIsPolygonUserData = flags.isPolygonUserData;

		os.Add ("isSameDirEdgeLegal", tmpIsSameDirEdgeLegal);
		os.Add ("isVertexUserData", tmpIsVertexUserData);
		os.Add ("isEdgeUserData", tmpIsEdgeUserData);
		os.Add ("isPolygonUserData", tmpIsPolygonUserData);
		return NoError;
	}


	template<class VertexUserData, class EdgeUserData, class PolygonUserData>
	GSErrCode	Restore (const GS::ObjectState& os, typename Geometry::CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>::Flags& flags)
	{
		bool tmpIsSameDirEdgeLegal;
		bool tmpIsVertexUserData;
		bool tmpIsEdgeUserData;
		bool tmpIsPolygonUserData;

		os.Get ("isSameDirEdgeLegal", tmpIsSameDirEdgeLegal);
		os.Get ("isVertexUserData", tmpIsVertexUserData);
		os.Get ("isEdgeUserData", tmpIsEdgeUserData);
		os.Get ("isPolygonUserData", tmpIsPolygonUserData);


		flags.isSameDirEdgeLegal = tmpIsSameDirEdgeLegal;
		flags.isVertexUserData = tmpIsVertexUserData;
		flags.isEdgeUserData = tmpIsEdgeUserData;
		flags.isPolygonUserData = tmpIsPolygonUserData;
		return NoError;
	}


	template<class VertexUserData, class EdgeUserData, class PolygonUserData>
	GSErrCode	Read (GS::IChannel& ic, typename Geometry::CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>::Flags& flags)
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


	template<class VertexUserData, class EdgeUserData, class PolygonUserData>
	GSErrCode	Write (GS::OChannel& oc, const typename Geometry::CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>::Flags& flags)
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


	template<class VertexUserData, class EdgeUserData, class PolygonUserData>
	GSErrCode	Store (GS::ObjectState& os, const CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline)
	{
		os.Add ("boundBox", polyline.boundBox);
		Store<VertexUserData, EdgeUserData, PolygonUserData> (os, polyline.flags);
		os.Add ("inhPolygonInfo", polyline.inhPolygonInfo);
		os.Add ("defaultVertexData", polyline.defaultVertexData);
		os.Add ("defaultEdgeData", polyline.defaultEdgeData);
		os.Add ("nVertices", polyline.nVertices);
		os.Add ("nVertices", polyline.vertices);
		os.Add ("vertexInfos", polyline.vertexInfos);
		os.Add ("nArcs", polyline.nArcs);
		os.Add ("arcs", polyline.arcs);
		os.Add ("inhEdgeInfos", polyline.inhEdgeInfos);
		os.Add ("nNurbs", polyline.nNurbs);
		for (const auto& nurb : polyline.nurbs)
			polyline.Store (os, nurb);
		return NoError;
	}


	template<class VertexUserData, class EdgeUserData, class PolygonUserData>
	GSErrCode	Restore (const GS::ObjectState& os, CorePolyline2D<VertexUserData, EdgeUserData, PolygonUserData>& polyline)
	{
		os.Get ("boundBox", polyline.boundBox);
		Restore<VertexUserData, EdgeUserData, PolygonUserData> (os, polyline.flags);
		os.Get ("inhPolygonInfo", polyline.inhPolygonInfo);
		os.Get ("defaultVertexData", polyline.defaultVertexData);
		os.Get ("defaultEdgeData", polyline.defaultEdgeData);
		os.Get ("nVertices", polyline.nVertices);
		os.Get ("nVertices", polyline.vertices);
		os.Get ("vertexInfos", polyline.vertexInfos);
		os.Get ("nArcs", polyline.nArcs);
		os.Get ("arcs", polyline.arcs);
		os.Get ("inhEdgeInfos", polyline.inhEdgeInfos);
		os.Get ("nNurbs", polyline.nNurbs);
		for (auto& nurb : polyline.nurbs)
			polyline.Restore (os, nurb);
		return NoError;
	}
}	// namespace Geometry

#endif //GEOMETRY_COREPOLYLINE2D_CLASSDEFINITION_HPP
