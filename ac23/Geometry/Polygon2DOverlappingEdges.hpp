#ifndef GEOMETRY_POLYGON2DOVERLAPPINGEDGES_HPP
#define GEOMETRY_POLYGON2DOVERLAPPINGEDGES_HPP

#pragma once

#include "Point2DData.h"
#include "Poly2DTypes.hpp"
#include "Line2D.hpp"

#include "QuadTree.hpp"

#include "Polygon2DOverlappingEdgeResult.hpp"
#include "SkipMap.hpp"


namespace Geometry {

template <class MultiPolygonType>
class OverlappingSectorsAndArcsFinder {

private:
	MultiPolygonType polys;


public:

	OverlappingSectorsAndArcsFinder (const MultiPolygonType& p) :
		polys(p)
	{
	}

	void  GetOverlapsSectorsAndArcs (
		GS::Array<GS::Array<UIndex>>& indicesOfRelevantEdges, //az indicesOfRelevantEdges-ben a polygonokat es azon belul az eleket is 0-tol szamozzuk (a poly-k maguknak 1-tol szamozzak az eleiket)
		GS::Array<SectorOfTwoPolys>& commonSectors,
		GS::Array<SectorOfAPoly>& otherSectors,
		GS::Array<ArcOfTwoPolys>& commonArcs,
		GS::Array<ArcOfAPoly>& otherArcs)
	{

		USize numOfPolys = polys.GetSize();
		indicesOfRelevantEdges.SetSize(numOfPolys);

		GS::Array<GS::Array<SectorOfAPoly>> sectorsOfPolys;
		sectorsOfPolys.SetSize(numOfPolys);
		GS::Array<GS::Array<ArcOfAPoly>> arcsOfPolys;
		arcsOfPolys.SetSize(numOfPolys);

		QuadTree::QuadTree<UIndex> quadTreeForPolys;
		for (UIndex polyIdx = 0; polyIdx < numOfPolys; ++polyIdx) {
			quadTreeForPolys.Insert(polys[polyIdx].GetBoundBox(), polyIdx);
		}

 		commonSectors.SetCapacity(polys.GetSize());
 		otherSectors.SetCapacity(polys.GetSize());

		for (UIndex polyIdx1 = 0; polyIdx1 < numOfPolys; ++polyIdx1) {	

			GS::Array<UIndex> relevantPolys = quadTreeForPolys.Enumerate(polys[polyIdx1].GetBoundBox());

			if (relevantPolys.IsEmpty())
				continue;

			CollectSectorsAndArcsOfPolysSorted(polys[polyIdx1], polyIdx1, sectorsOfPolys[polyIdx1], arcsOfPolys[polyIdx1]);

			GS::Array<GS::Array<SectorOfAPoly>> sectors;
			GS::Array<GS::Array<ArcOfAPoly>> arcs;
			sectors.SetSize(polys[polyIdx1].GetEdgeNum());
			arcs.SetSize(polys[polyIdx1].GetEdgeNum());

			for (UIndex i = 0; i < relevantPolys.GetSize(); ++i) {

				UIndex polyIdx2 = relevantPolys[i];

				if (polyIdx2 == polyIdx1)
					continue;

				CollectSectorsAndArcsOfPolysSorted(polys[polyIdx2], polyIdx2, sectorsOfPolys[polyIdx2], arcsOfPolys[polyIdx2]);

				CollectOverlappingEdges(sectors, sectorsOfPolys[polyIdx1], sectorsOfPolys[polyIdx2]);

				CollectOverlappingEdges(arcs, arcsOfPolys[polyIdx1], arcsOfPolys[polyIdx2]);
			}

			for (auto edgeIt = polys[polyIdx1].EnumerateEdge(); edgeIt != nullptr; ++edgeIt) {

				Sector sect;
				GenArc arc;

				UIndex edgeIdx = EdgeIdxInPolyToArrayIdx(edgeIt->GetIndex());

				edgeIt->GetSector(sect, arc);
				if (!sectors[edgeIdx].IsEmpty()) {
					indicesOfRelevantEdges[polyIdx1].Push(edgeIdx);
					GetOverlapOfCollinearSectors(SectorOfAPoly(sect, polyIdx1, edgeIdx), sectors[edgeIdx], commonSectors, otherSectors);
					continue;
				}
				if (!arcs[edgeIdx].IsEmpty()) {
					indicesOfRelevantEdges[polyIdx1].Push(edgeIdx);
					GetOverlapOfConcyclicArcs(ArcOfAPoly(arc, polyIdx1, edgeIdx), arcs[edgeIdx], commonArcs, otherArcs);
					continue;
				}
			}
		}

	}



	GSErrCode  GetResult (GS::Array<GS::SkipMap<UIndex, Polygon2DOverlappingEdgeResults>>& results)
	{
		GSErrCode errorCode = NoError;

		USize numOfPolys = polys.GetSize ();
		results.SetSize (numOfPolys);

		GS::Array<GS::Array<SectorOfAPoly>> sectorsOfPolys;
		sectorsOfPolys.SetSize (numOfPolys);
		GS::Array<GS::Array<ArcOfAPoly>> arcsOfPolys;
		arcsOfPolys.SetSize (numOfPolys);

		QuadTree::QuadTree<UIndex> quadTreeForPolys;
		for (UIndex polyIdx = 0; polyIdx < numOfPolys; ++polyIdx) {
			quadTreeForPolys.Insert (polys[polyIdx].GetBoundBox (), polyIdx);
		}

		for (UIndex polyIdx1 = 0; polyIdx1 < numOfPolys; ++polyIdx1) {

			GS::Array<UIndex> relevantPolys = quadTreeForPolys.Enumerate (polys[polyIdx1].GetBoundBox ());

			if (relevantPolys.IsEmpty ())
				continue;

			CollectSectorsAndArcsOfPolysSorted (polys[polyIdx1], polyIdx1, sectorsOfPolys[polyIdx1], arcsOfPolys[polyIdx1]);

			GS::Array<GS::Array<SectorOfAPoly>> sectors;
			GS::Array<GS::Array<ArcOfAPoly>> arcs;
			sectors.SetSize (polys[polyIdx1].GetEdgeNum ());
			arcs.SetSize (polys[polyIdx1].GetEdgeNum ());

			for (UIndex i = 0; i < relevantPolys.GetSize (); ++i) {

				UIndex polyIdx2 = relevantPolys[i];

				if (polyIdx2 == polyIdx1)
					continue;

				CollectSectorsAndArcsOfPolysSorted (polys[polyIdx2], polyIdx2, sectorsOfPolys[polyIdx2], arcsOfPolys[polyIdx2]);

				CollectOverlappingEdges (sectors, sectorsOfPolys[polyIdx1], sectorsOfPolys[polyIdx2]);

				CollectOverlappingEdges (arcs, arcsOfPolys[polyIdx1], arcsOfPolys[polyIdx2]);
			}

			for (auto edgeIt = polys[polyIdx1].EnumerateEdge (); edgeIt != nullptr; ++edgeIt) {
				UIndex edgeIdx = EdgeIdxInPolyToArrayIdx (edgeIt->GetIndex ());

				if (!sectors[edgeIdx].IsEmpty () || !arcs[edgeIdx].IsEmpty ()) {
					results[polyIdx1].Add (edgeIdx, Polygon2DOverlappingEdgeResults (edgeIdx));
				}
			}


			for (auto edgeIt = polys[polyIdx1].EnumerateEdge (); edgeIt != nullptr; ++edgeIt) {

				Sector sect;
				GenArc arc;

				UIndex edgeIdx = EdgeIdxInPolyToArrayIdx (edgeIt->GetIndex ());

				edgeIt->GetSector (sect, arc);
				if (!sectors[edgeIdx].IsEmpty ()) {
					GetOverlapOfCollinearSectors (SectorOfAPoly (sect, polyIdx1, edgeIdx), sectors[edgeIdx], results);
				} else if (!arcs[edgeIdx].IsEmpty ()) {
					GetOverlapOfConcyclicArcs (ArcOfAPoly (arc, polyIdx1, edgeIdx), arcs[edgeIdx], results, errorCode);
					if (errorCode != NoError)
						return errorCode;
					
					continue;
				}
			}
		}

		for (auto& result : results) {

			result.EnumerateValues ([&] (Geometry::Polygon2DOverlappingEdgeResults& edgeResults) {
				GSErrCode err = edgeResults.OrderEdgeParts (polys);
				if (err != NoError)
					errorCode = Error;
			});
		}	

		return errorCode;
	}

private:

	static UIndex EdgeIdxInPolyToArrayIdx (UIndex idx)
	{
		return idx - 1;
	}

	template <class PolygonType>
	static void CollectSectorsAndArcsOfPolysSorted (
		const PolygonType& poly,
		const UIndex polyIdx,
		GS::Array<SectorOfAPoly>& sectorsOfPoly,
		GS::Array<ArcOfAPoly>& arcsOfPoly)
	{
		if (!(sectorsOfPoly.IsEmpty () && arcsOfPoly.IsEmpty ()))
			return;

		for (auto edgeIt = poly.EnumerateEdge (); edgeIt != nullptr; ++edgeIt) {
			if (IsNearZero (edgeIt->GetLength ()))
				continue;
			Sector sect;
			GenArc arc;
			if (edgeIt->GetSector (sect, arc)) {
				arcsOfPoly.PushNew (ArcOfAPoly (arc, polyIdx, EdgeIdxInPolyToArrayIdx (edgeIt->GetIndex ())));
			} else {
				sectorsOfPoly.PushNew (SectorOfAPoly (sect, polyIdx, EdgeIdxInPolyToArrayIdx (edgeIt->GetIndex ())));
			}
		}

		GS::Sort (sectorsOfPoly.Begin (), sectorsOfPoly.End (), [] (const SectorOfAPoly& s1, const SectorOfAPoly& s2) -> bool {return IsLessByLine2DBegXYEndXY (s1.item, s2.item); });
		GS::Sort (arcsOfPoly.Begin (), arcsOfPoly.End (), [] (const ArcOfAPoly& a1, const ArcOfAPoly& a2) -> bool {return IsLessByCircleAngles (a1.item, a2.item); });
	}


	template <class EdgeType>
	static void CollectOverlappingEdges (
		GS::Array<GS::Array<EdgeOfAPoly<EdgeType>>>& edges,
		const GS::Array<EdgeOfAPoly<EdgeType>>& edgesOfPoly1,
		const GS::Array<EdgeOfAPoly<EdgeType>>& edgesOfPoly2)
	{

		UIndex idx1 = 0, idx2 = 0;
		USize size1 = edgesOfPoly1.GetSize (), size2 = edgesOfPoly2.GetSize ();

		while (idx1 < size1 && idx2 < size2) {

			if (DoTheyOverlap (edgesOfPoly1[idx1], edgesOfPoly2[idx2])) {

				UIndex idx = idx2;
				while (idx < size2 && DoTheyOverlap (edgesOfPoly1[idx1], edgesOfPoly2[idx])) {
					edges[edgesOfPoly1[idx1].edgeIdx].Push (edgesOfPoly2[idx]);
					++idx;
				}
				++idx1;
				continue;
			}
			if (IsLessEdge (edgesOfPoly1[idx1], edgesOfPoly2[idx2])) {		//if we got this far, the two edges surely don't overlap
				++idx1;
				continue;
			}
			if (IsLessEdge (edgesOfPoly2[idx2], edgesOfPoly1[idx1])) {
				++idx2;
				continue;
			}
		}
	}

	static void GetOverlapOfCollinearSectors (
		const SectorOfAPoly& sect0,
		GS::Array<SectorOfAPoly>& sectors,
		GS::Array<SectorOfTwoPolys>& commonParts,
		GS::Array<SectorOfAPoly>& otherParts)
	{

		if (sectors.IsEmpty ()) {
			otherParts.Push (sect0);
			return;
		}

		GS::Sort (sectors.Begin (), sectors.End (), [] (const SectorOfAPoly& s1, const SectorOfAPoly& s2) -> bool {return IsLessByLine2DBegXYEndXY (s1.item, s2.item); });
		bool isOrientationReversed = IsOrientationReversed (sect0.item);

		if (IsLessByXY (GetMinEndC (sect0.item), GetMinEndC (sectors[0].item))) {
			otherParts.PushNew (Sector (GetMinEndC (sect0.item), GetMinEndC (sectors[0].item)), sect0.polyIdx, sect0.edgeIdx, isOrientationReversed);
		}

		Coord cMin = MAXByXY (GetMinEndC (sectors[0].item), GetMinEndC (sect0.item));
		Coord cMax = GetMaxEndC (sectors[0].item), cMax2 = cMin;
		UIndex maxIdx = 0;

		UIndex i = 0;
		while (i < sectors.GetSize ()) {
			while (i < sectors.GetSize () && IsLessOrNearByXY (GetMinEndC (sectors[i].item), cMax)) {
				if (IsGreaterByXY (GetMaxEndC (sectors[i].item), cMax)) {
					cMax2 = cMax;
					cMax = GetMaxEndC (sectors[i].item);
					maxIdx = i;
				}
				++i;
			}

			if (IsGreaterByXY(cMax, GetMaxEndC(sect0.item))) {
				bool isOrientationReversed2 = IsOrientationReversed(sectors[maxIdx].item);
				commonParts.PushNew(Sector(cMax2, GetMaxEndC(sect0.item)), sect0.polyIdx, sect0.edgeIdx, sectors[maxIdx].polyIdx, sectors[maxIdx].edgeIdx, isOrientationReversed, isOrientationReversed2);
				break;
			}

			if (IsNear(cMax, GetMaxEndC(sect0.item))) {
				if (sect0.polyIdx < sectors[maxIdx].polyIdx) {
					bool isOrientationReversed2 = IsOrientationReversed(sectors[maxIdx].item);
					commonParts.PushNew(Sector(cMax2, GetMaxEndC(sect0.item)), sect0.polyIdx, sect0.edgeIdx, sectors[maxIdx].polyIdx, sectors[maxIdx].edgeIdx, isOrientationReversed, isOrientationReversed2);
				}
				break;
			}

			if (i != sectors.GetSize()) {			//if we got this far, cMax < sect0.GetMaxEnd()
				otherParts.PushNew(Sector(cMax, GetMinEndC(sectors[i].item)), sect0.polyIdx, sect0.edgeIdx, isOrientationReversed);
				cMin = GetMinEndC(sectors[i].item);
				cMax = GetMaxEndC(sectors[i].item);
				cMax2 = cMin;
				continue;
			}
			else {
				otherParts.PushNew(Sector(cMax, GetMaxEndC(sect0.item)), sect0.polyIdx, sect0.edgeIdx, isOrientationReversed);
				continue;
			}
		}
	}


void GetOverlapOfCollinearSectors (
	const SectorOfAPoly& sect0,
	GS::Array<SectorOfAPoly>& sectors,
	GS::Array<GS::SkipMap<UIndex, Polygon2DOverlappingEdgeResults>>& edgeResults)
{

	if (DBERROR (sectors.IsEmpty ())) {
		return;
	}

	GS::Sort (sectors.Begin (), sectors.End (), [] (const SectorOfAPoly& s1, const SectorOfAPoly& s2) -> bool {return IsLessByLine2DBegXYEndXY (s1.item, s2.item); });
	bool isOrientationReversed = IsOrientationReversed (sect0.item);

	if (IsLessByXY (GetMinEndC (sect0.item), GetMinEndC (sectors[0].item))) {
		SectorOfAPoly s (Sector (GetMinEndC (sect0.item), GetMinEndC (sectors[0].item)), sect0.polyIdx, sect0.edgeIdx, isOrientationReversed);
			edgeResults[sect0.polyIdx][sect0.edgeIdx].AddResult (s, false);
	}

	Coord cMin = MAXByXY (GetMinEndC (sectors[0].item), GetMinEndC (sect0.item));
	Coord cMax = GetMaxEndC (sectors[0].item), cMax2 = cMin;
	UIndex maxIdx = 0;

	UIndex i = 0;
	while (i < sectors.GetSize ()) {
		while (i < sectors.GetSize () && IsLessOrNearByXY (GetMinEndC (sectors[i].item), cMax)) {
			if (IsGreaterByXY (GetMaxEndC (sectors[i].item), cMax)) {
				cMax2 = cMax;
				cMax = GetMaxEndC (sectors[i].item);
				maxIdx = i;
			} else if (IsNear (GetMaxEndC (sectors[i].item), cMax)) {
				maxIdx = i;
			}
			++i;
		}

		if (IsGreaterByXY (cMax, GetMaxEndC (sect0.item))) {
			
			SectorOfAPoly s (Sector (cMax2, GetMaxEndC (sect0.item)), sect0.polyIdx, sect0.edgeIdx, isOrientationReversed);
					
			bool isOrientationReversed2 = IsOrientationReversed (sectors[maxIdx].item);
			SectorOfAPoly s2 (Sector (cMax2, GetMaxEndC (sect0.item)), sectors[maxIdx].polyIdx, sectors[maxIdx].edgeIdx, isOrientationReversed2);

			bool firstVisible = sect0.polyIdx < sectors[maxIdx].polyIdx;
			
			edgeResults[sect0.polyIdx][sect0.edgeIdx].AddResult (s, true, firstVisible);

			if (!edgeResults[sectors[maxIdx].polyIdx].ContainsKey (sectors[maxIdx].edgeIdx))
				edgeResults[sectors[maxIdx].polyIdx].Add (sectors[maxIdx].edgeIdx, Polygon2DOverlappingEdgeResults (sectors[maxIdx].edgeIdx));
			edgeResults[sectors[maxIdx].polyIdx][sectors[maxIdx].edgeIdx].AddResult (s2, true, !firstVisible);
			break;
		}

		if (IsNear (cMax, GetMaxEndC (sect0.item))) {
			if (sect0.polyIdx < sectors[maxIdx].polyIdx) {
				SectorOfAPoly s (Sector (cMax2, GetMaxEndC (sect0.item)), sect0.polyIdx, sect0.edgeIdx, isOrientationReversed);

				bool isOrientationReversed2 = IsOrientationReversed (sectors[maxIdx].item);
				SectorOfAPoly s2 (Sector (cMax2, GetMaxEndC (sect0.item)), sectors[maxIdx].polyIdx, sectors[maxIdx].edgeIdx, isOrientationReversed2);
				bool firstVisible = sect0.polyIdx < sectors[maxIdx].polyIdx;

				edgeResults[sect0.polyIdx][sect0.edgeIdx].AddResult (s, true, firstVisible);

				if (!edgeResults[sectors[maxIdx].polyIdx].ContainsKey (sectors[maxIdx].edgeIdx))
					edgeResults[sectors[maxIdx].polyIdx].Add (sectors[maxIdx].edgeIdx, Polygon2DOverlappingEdgeResults (sectors[maxIdx].edgeIdx));

				edgeResults[sectors[maxIdx].polyIdx][sectors[maxIdx].edgeIdx].AddResult (s2, true, !firstVisible);
			}
			break;
		}

		if (i != sectors.GetSize ()) {			//if we got this far, cMax < sect0.GetMaxEnd()
			SectorOfAPoly s (Sector (cMax, GetMinEndC (sectors[i].item)), sect0.polyIdx, sect0.edgeIdx, isOrientationReversed);
			edgeResults[sect0.polyIdx][sect0.edgeIdx].AddResult (s, false);
			cMin = GetMinEndC (sectors[i].item);
			cMax = GetMaxEndC (sectors[i].item);
			cMax2 = cMin;
			continue;
		} else {
			SectorOfAPoly s (Sector (cMax, GetMaxEndC (sect0.item)), sect0.polyIdx, sect0.edgeIdx, isOrientationReversed);
			edgeResults[sect0.polyIdx][sect0.edgeIdx].AddResult (s, false);
			continue;
		}
	}
}

	static void GetOverlapOfConcyclicArcs (
		const ArcOfAPoly& arc0,
		GS::Array<ArcOfAPoly>& arcs,
		GS::Array<ArcOfTwoPolys>& commonParts,
		GS::Array<ArcOfAPoly>& otherParts)
	{

		if (arcs.IsEmpty()) {
			otherParts.Push(arc0);
			return;
		}

		bool isOrientationReversed = (arc0.item.GetArcAngle() < 0);
		USize numOfArcs = arcs.GetSize();
		double startAngle = GetMinEndAng(arc0.item), endAngle = GetMaxEndAng(arc0.item);
		Coord startC = GetMinEndC(arc0.item), endC = GetMaxEndC(arc0.item);
		double aMin = startAngle, aMax = startAngle, aMax2 = startAngle, aStart = startAngle;
		Coord cMin = startC, cMax = startC, cMax2 = startC, cStart = startC;
		UIndex maxIdx = 0;

		//kezdo, majd vegszog szerint rendezunk, startAngle-tol szamolva, a startAngle-ben kezdodoek a vegere kerulnek
		GS::Sort(arcs.Begin(), arcs.End(), [&startAngle](const ArcOfAPoly& arc1, const ArcOfAPoly& arc2) -> bool {
			return (IsStrictlyBetweenAng(GetMinEndAng(arc1.item), startAngle, GetMinEndAng(arc2.item)) ||
				(IsNear(GetMinEndAng(arc1.item), GetMinEndAng(arc2.item)) && IsStrictlyBetweenAng(GetMaxEndAng(arc1.item), GetMinEndAng(arc1.item), GetMaxEndAng(arc2.item))));
		});

		CalculateAStart(arcs, startAngle, aStart, aMax2, cMax2, cStart, aMax, cMax, maxIdx);

		if (HandleTrivialCasesAboutAStart(aStart, endAngle, startAngle, arc0, cMax2, endC, arcs[0], arcs[maxIdx], commonParts, isOrientationReversed, cStart, otherParts)) {
			return;
		}

		if (IsStrictlyBetweenAng(GetMinEndAng(arcs[0].item), aStart, endAngle)) {
			SetMinMax(arc0, cStart, arcs, otherParts, isOrientationReversed, aMax, aMin, aMax2, cMax, cMin, cMax2, maxIdx, 0);
		}
		else {
			aMax = GetMaxEndAng(arcs[0].item);
			aMin = GetMinEndAng(arcs[0].item);
			aMax2 = aStart;
			cMax = cStart;
		}

		UIndex i = 0;
		while (i < numOfArcs) {
			while (i < numOfArcs && IsBetweenAng(GetMinEndAng(arcs[i].item), aMin, aMax)) {
				if (maxIdx != i && IsBetweenAng(aMax, aMin, GetMaxEndAng(arcs[i].item))) {
					aMax2 = aMax;
					cMax2 = cMax;
					aMax = GetMaxEndAng(arcs[i].item);
					cMax = GetMaxEndC(arcs[i].item);
					maxIdx = i;
				}
				if (IsStrictlyBetweenAng(endAngle, aMin, aMax)) {
					GenArc thisArc;
					thisArc.SetToCircleArc(arc0.item.GetOrigo(), cMax2, endC);
					bool isOrientationReversed2 = (arcs[maxIdx].item.GetArcAngle() < 0);
					commonParts.PushNew(thisArc, arc0.polyIdx, arc0.edgeIdx, arcs[maxIdx].polyIdx, arcs[maxIdx].edgeIdx, isOrientationReversed, isOrientationReversed2);
					return;
				}
				if (IsNear(endAngle, aMax)) {
					if (arc0.polyIdx < arcs[maxIdx].polyIdx) {
						GenArc thisArc;
						thisArc.SetToCircleArc(arc0.item.GetOrigo(), cMax2, endC);
						bool isOrientationReversed2 = (arcs[maxIdx].item.GetArcAngle() < 0);
						commonParts.PushNew(thisArc, arc0.polyIdx, arc0.edgeIdx, arcs[maxIdx].polyIdx, arcs[maxIdx].edgeIdx, isOrientationReversed, isOrientationReversed2);
					}
					return;
				}
				++i;
			}

			if (i == arcs.GetSize()) {
				GenArc thisArc;
				thisArc.SetToCircleArc(arc0.item.GetOrigo(), cMax, endC);
				otherParts.PushNew(thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
				continue;
			}
			else {
				if (IsStrictlyBetweenAng(GetMinEndAng(arcs[i].item), startAngle, endAngle)) {
					SetMinMax(arc0, cStart, arcs, otherParts, isOrientationReversed, aMax, aMin, aMax2, cMax, cMin, cMax2, maxIdx, i);
					continue;
				}
				else {
					GenArc thisArc;
					thisArc.SetToCircleArc(arc0.item.GetOrigo(), cMax, GetMaxEndC(arc0.item));
					otherParts.PushNew(thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
					return;
				}
			}

		}
	}

	void GetOverlapOfConcyclicArcs (
		const ArcOfAPoly& arc0,
		GS::Array<ArcOfAPoly>& arcs,
		GS::Array<GS::SkipMap<UIndex,Polygon2DOverlappingEdgeResults>>& edgeResults,
		GSErrCode& err)
	{

		if (DBERROR (arcs.IsEmpty ())) {
			return;
		}

		bool isOrientationReversed = (arc0.item.GetArcAngle () < 0);
		USize numOfArcs = arcs.GetSize ();
		double startAngle = GetMinEndAng (arc0.item), endAngle = GetMaxEndAng (arc0.item);
		Coord startC = GetMinEndC (arc0.item), endC = GetMaxEndC (arc0.item);
		double aMin = startAngle, aMax = startAngle, aMax2 = startAngle, aStart = startAngle;
		Coord cMin = startC, cMax = startC, cMax2 = startC, cStart = startC;
		UIndex maxIdx = 0;

		//kezdo, majd vegszog szerint rendezunk, startAngle-tol szamolva, a startAngle-ben kezdodoek a vegere kerulnek
		GS::Sort (arcs.Begin (), arcs.End (), [&startAngle] (const ArcOfAPoly& arc1, const ArcOfAPoly& arc2) -> bool {
			return (IsStrictlyBetweenAng (GetMinEndAng (arc1.item), startAngle, GetMinEndAng (arc2.item)) ||
				(IsNear (GetMinEndAng (arc1.item), GetMinEndAng (arc2.item)) && IsStrictlyBetweenAng (GetMaxEndAng (arc1.item), GetMinEndAng (arc1.item), GetMaxEndAng (arc2.item))));
		});

		CalculateAStart (arcs, startAngle, aStart, aMax2, cMax2, cStart, aMax, cMax, maxIdx);

		if (HandleTrivialCasesAboutAStart (aStart, endAngle, startAngle, arc0, cMax2, endC, arcs[0], arcs[maxIdx], isOrientationReversed, cStart, edgeResults)) {
			return;
		}

		if (IsStrictlyBetweenAng (GetMinEndAng (arcs[0].item), aStart, endAngle)) {
			SetMinMax (arc0, cStart, arcs, edgeResults, isOrientationReversed, aMax, aMin, aMax2, cMax, cMin, cMax2, maxIdx, 0);
		} else {
			aMax = GetMaxEndAng (arcs[0].item);
			aMin = GetMinEndAng (arcs[0].item);
			aMax2 = aStart;
			cMax = cStart;
		}

		UIndex i = 0;
		while (i < numOfArcs) {
			while (i < numOfArcs && IsBetweenAng (GetMinEndAng (arcs[i].item), aMin, aMax)) {
				if (maxIdx != i && IsBetweenAng (aMax, aMin, GetMaxEndAng (arcs[i].item))) {
					aMax2 = aMax;
					cMax2 = cMax;
					aMax = GetMaxEndAng (arcs[i].item);
					cMax = GetMaxEndC (arcs[i].item);
					maxIdx = i;
				}
				if (IsStrictlyBetweenAng (endAngle, aMin, aMax)) {
					GenArc thisArc;
					thisArc.SetToCircleArc (arc0.item.GetOrigo (), cMax2, endC);

					ArcOfAPoly a (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);					

					bool isOrientationReversed2 = (arcs[maxIdx].item.GetArcAngle () < 0);
					ArcOfAPoly a2 (thisArc, arcs[maxIdx].polyIdx, arcs[maxIdx].edgeIdx, isOrientationReversed2);
					bool firstVisible = arc0.polyIdx < arcs[maxIdx].polyIdx;
					edgeResults[arc0.polyIdx][arc0.edgeIdx].AddResult (a, true, firstVisible);

					if (!edgeResults[arcs[maxIdx].polyIdx].ContainsKey (arcs[maxIdx].edgeIdx))
						edgeResults[arcs[maxIdx].polyIdx].Add (arcs[maxIdx].edgeIdx, Polygon2DOverlappingEdgeResults (arcs[maxIdx].edgeIdx));
					edgeResults[arcs[maxIdx].polyIdx][arcs[maxIdx].edgeIdx].AddResult (a2, true, !firstVisible);

					return;
				}
				if (IsNear (endAngle, aMax)) {
					if (arc0.polyIdx < arcs[maxIdx].polyIdx) {
						GenArc thisArc;
						thisArc.SetToCircleArc (arc0.item.GetOrigo (), cMax2, endC);

						ArcOfAPoly a (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
						
						bool isOrientationReversed2 = (arcs[maxIdx].item.GetArcAngle () < 0);
						ArcOfAPoly a2 (thisArc, arcs[maxIdx].polyIdx, arcs[maxIdx].edgeIdx, isOrientationReversed2);
						bool firstVisible = arc0.polyIdx < arcs[maxIdx].polyIdx;
						edgeResults[arc0.polyIdx][arc0.edgeIdx].AddResult (a, true, firstVisible);

						if (!edgeResults[arcs[maxIdx].polyIdx].ContainsKey (arcs[maxIdx].edgeIdx))
							edgeResults[arcs[maxIdx].polyIdx].Add (arcs[maxIdx].edgeIdx, Polygon2DOverlappingEdgeResults (arcs[maxIdx].edgeIdx));
						edgeResults[arcs[maxIdx].polyIdx][arcs[maxIdx].edgeIdx].AddResult (a2, true, !firstVisible);
					}
					return;
				}
				++i;
			}

			if (i == arcs.GetSize ()) {
				GenArc thisArc;
				thisArc.SetToCircleArc (arc0.item.GetOrigo (), cMax, endC);
				ArcOfAPoly a (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
				edgeResults[arc0.polyIdx][arc0.edgeIdx].AddResult (a, false);
				continue;
			} else {
				if (IsStrictlyBetweenAng (GetMinEndAng (arcs[i].item), startAngle, endAngle)) {
					if (NoError != SetMinMax (arc0, cStart, arcs, edgeResults, isOrientationReversed, aMax, aMin, aMax2, cMax, cMin, cMax2, maxIdx, i, true)) {
						err = Error;
						return;
					}
					continue;
				} else {
					GenArc thisArc;
					thisArc.SetToCircleArc (arc0.item.GetOrigo (), cMax, GetMaxEndC (arc0.item));
					ArcOfAPoly a (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
					edgeResults[arc0.polyIdx][arc0.edgeIdx].AddResult (a, false);
					return;
				}
			}

		}
	}

	static void CalculateAStart (GS::Array<ArcOfAPoly>& arcs, double startAngle, double& aStart, double& aMax2, Coord& cMax2, Coord& cStart, double& aMax, Coord& cMax, UIndex& maxIdx)
	{
		for (UIndex i = 0; i < arcs.GetSize(); ++i) {
			if (IsBetweenAng(startAngle, GetMinEndAng(arcs[i].item), GetMaxEndAng(arcs[i].item)) && IsBetweenAng(aStart, startAngle, GetMaxEndAng(arcs[i].item))) {
				aMax2 = aStart;
				cMax2 = cStart;
				aStart = GetMaxEndAng(arcs[i].item);
				cStart = GetMaxEndC(arcs[i].item);
				aMax = aStart;
				cMax = cStart;
				maxIdx = i;
			}
		}
	}

	static bool HandleTrivialCasesAboutAStart (double aStart,
											   double endAngle,
											   double startAngle,
											   const ArcOfAPoly& arc0,
											   Coord cMax2,
											   Coord endC,
											   const  ArcOfAPoly& arcs_0,
											   const  ArcOfAPoly& arcs_maxIdx,
											   GS::Array<ArcOfTwoPolys>& commonParts,
											   bool isOrientationReversed,
											   Coord cStart,
											   GS::Array<ArcOfAPoly>& otherParts)
	{
		if (IsStrictlyBetweenAng(aStart, endAngle, startAngle)) {
			GenArc thisArc;
			thisArc.SetToCircleArc(arc0.item.GetOrigo(), cMax2, endC);
			bool isOrientationReversed2 = (arcs_maxIdx.item.GetArcAngle() < 0);
			commonParts.PushNew(thisArc, arc0.polyIdx, arc0.edgeIdx, arcs_maxIdx.polyIdx, arcs_maxIdx.edgeIdx, isOrientationReversed, isOrientationReversed2);
			return true;
		}

		if (IsNear(aStart, endAngle)) {
			if (arc0.polyIdx < arcs_maxIdx.polyIdx) {
				GenArc thisArc;
				thisArc.SetToCircleArc(arc0.item.GetOrigo(), cMax2, endC);
				bool isOrientationReversed2 = (arcs_maxIdx.item.GetArcAngle() < 0);
				commonParts.PushNew(thisArc, arc0.polyIdx, arc0.edgeIdx, arcs_maxIdx.polyIdx, arcs_maxIdx.edgeIdx, isOrientationReversed, isOrientationReversed2);
			}
			return true;
		}

		//if we got this far, aStart must be inside arc0

		if (IsBetweenAng(GetMinEndAng(arcs_0.item), endAngle, startAngle)) {
			//ez azt jelenti, hogy arcs-ban mindenki tartalmazza arc0 kezdopontjat
			GenArc thisArc;
			thisArc.SetToCircleArc(arc0.item.GetOrigo(), cStart, endC);
			otherParts.PushNew(thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
			return true;
		}

		return false;
	}


	static bool HandleTrivialCasesAboutAStart (const double aStart, 
											   const double endAngle, 
											   const double startAngle, 
											   const ArcOfAPoly& arc0, 
											   const Coord cMax2,
											   const Coord endC, 
											   const  ArcOfAPoly& arcs_0, 
											   const  ArcOfAPoly& arcs_maxIdx, 
											   const bool isOrientationReversed, 
											   const Coord cStart, 
											   GS::Array<GS::SkipMap<UIndex,Polygon2DOverlappingEdgeResults>>& edgeResults)
	{
		if (IsStrictlyBetweenAng (aStart, endAngle, startAngle)) {
			GenArc thisArc;
			thisArc.SetToCircleArc (arc0.item.GetOrigo (), cMax2, endC);
			
			ArcOfAPoly a (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);

			bool isOrientationReversed2 = (arcs_maxIdx.item.GetArcAngle () < 0);
			ArcOfAPoly a2 (thisArc, arcs_maxIdx.polyIdx, arcs_maxIdx.edgeIdx, isOrientationReversed2);
			bool firstVisible = arc0.polyIdx < arcs_maxIdx.polyIdx;
			edgeResults[arc0.polyIdx][arc0.edgeIdx].AddResult (a, true, firstVisible);

			if (!edgeResults[arcs_maxIdx.polyIdx].ContainsKey (arcs_maxIdx.edgeIdx))
				edgeResults[arcs_maxIdx.polyIdx].Add (arcs_maxIdx.edgeIdx, Polygon2DOverlappingEdgeResults (arcs_maxIdx.edgeIdx));
			edgeResults[arcs_maxIdx.polyIdx][arcs_maxIdx.edgeIdx].AddResult (a2, true, !firstVisible);

			return true;
		}

		if (IsNear (aStart, endAngle)) {
			if (arc0.polyIdx < arcs_maxIdx.polyIdx) {
				GenArc thisArc;
				thisArc.SetToCircleArc (arc0.item.GetOrigo (), cMax2, endC);

				ArcOfAPoly a (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);

				bool isOrientationReversed2 = (arcs_maxIdx.item.GetArcAngle () < 0);
				ArcOfAPoly a2 (thisArc, arcs_maxIdx.polyIdx, arcs_maxIdx.edgeIdx, isOrientationReversed2);
				bool firstVisible = arc0.polyIdx < arcs_maxIdx.polyIdx;
				edgeResults[arc0.polyIdx][arc0.edgeIdx].AddResult (a, true, firstVisible);

				if (!edgeResults[arcs_maxIdx.polyIdx].ContainsKey (arcs_maxIdx.edgeIdx))
					edgeResults[arcs_maxIdx.polyIdx].Add (arcs_maxIdx.edgeIdx, Polygon2DOverlappingEdgeResults (arcs_maxIdx.edgeIdx));
				edgeResults[arcs_maxIdx.polyIdx][arcs_maxIdx.edgeIdx].AddResult (a2, true, !firstVisible);
			}
			return true;
		}

		//if we got this far, aStart must be inside arc0

		if (IsBetweenAng (GetMinEndAng (arcs_0.item), endAngle, startAngle)) {
			//ez azt jelenti, hogy arcs-ban mindenki tartalmazza arc0 kezdopontjat
			GenArc thisArc;
			thisArc.SetToCircleArc (arc0.item.GetOrigo (), cStart, endC);
			ArcOfAPoly a (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
			edgeResults[arc0.polyIdx][arc0.edgeIdx].AddResult (a, false);
			return true;
		}

		return false;
	}


	static void SetMinMax (const ArcOfAPoly& arc0, 
						   const Coord cStart, 
						   GS::Array<ArcOfAPoly>& arcs, 
						   GS::Array<ArcOfAPoly>& otherParts, 
						   const bool isOrientationReversed, 
						   double& aMax, double& aMin, 
						   double& aMax2, 
						   Coord& cMax, 
						   Coord& cMin, 
						   Coord& cMax2, 
						   UIndex& maxIdx, 
						   UIndex idx)
	{
		GenArc thisArc;
		thisArc.SetToCircleArc (arc0.item.GetOrigo (), cStart, GetMinEndC (arcs[0].item));
		otherParts.PushNew (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
		aMax = GetMaxEndAng (arcs[0].item);
		aMin = GetMinEndAng (arcs[0].item);
		aMax2 = GetMinEndAng (arcs[0].item);
		cMax = GetMaxEndC (arcs[0].item);
		cMin = GetMinEndC (arcs[0].item);
		cMax2 = GetMinEndC (arcs[0].item);
		maxIdx = idx;
	}


	static GSErrCode SetMinMax(const ArcOfAPoly& arc0,
							 const Coord cStart,
							 const GS::Array<ArcOfAPoly>& arcs,
							 GS::Array<GS::SkipMap<UIndex,Polygon2DOverlappingEdgeResults>>& edgeResults,
							 const bool isOrientationReversed,
							 double& aMax,
							 double& aMin,
							 double& aMax2,
							 Coord& cMax,
							 Coord& cMin,
							 Coord& cMax2,
							 UIndex& maxIdx,
							 const UIndex idx,
							 const bool canReturnError = false)
	{
		const double in_aMax = aMax;
		const double in_aMin = aMin;
		const double in_aMax2 = aMax2;
		const Coord in_cMax = cMax;
		const Coord in_cMin = cMin;
		const Coord in_cMax2 = cMax2;
		const UIndex in_maxIdx = maxIdx;

		GenArc thisArc;
		thisArc.SetToCircleArc(arc0.item.GetOrigo(), cStart, GetMinEndC(arcs[0].item));
		ArcOfAPoly a (thisArc, arc0.polyIdx, arc0.edgeIdx, isOrientationReversed);
		edgeResults[arc0.polyIdx][arc0.edgeIdx].AddResult (a, false);
		aMax = GetMaxEndAng(arcs[0].item);
		aMin = GetMinEndAng(arcs[0].item);
		aMax2 = GetMinEndAng(arcs[0].item);
		cMax = GetMaxEndC(arcs[0].item);
		cMin = GetMinEndC(arcs[0].item);
		cMax2 = GetMinEndC(arcs[0].item);
		maxIdx = idx;

		if (canReturnError &&
			!(IsNotNear (in_aMax, aMax) ||
			 IsNotNear (in_aMin, aMin) ||
			 IsNotNear (in_aMax2, aMax2) ||
			 in_cMax != cMax ||
			 in_cMin != cMin ||
			 in_cMax2 != cMax2 ||
			 in_maxIdx != maxIdx)) {
			DBBREAK ();
			return Error;
		}

		return NoError;
	}

	//#RJ_A180_GeometryRefactor Ez utan jovo method-okat meg kellene nezni, es ami ertelmes altalanos funkcio azt ki kellene emelni. A tobbire meg kellne egy review-t irni Varga Pistanak, hogy tegyek rendbe
	static double GetLineOfSectorDistanceFromOrigo (const Sector& sect)
	{
		double x1 = sect.c1.x, x2 = sect.c2.x;
		double y1 = sect.c1.y, y2 = sect.c2.y;

		DBASSERT (IsNotNearZero (sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2))));

		double distance = (-x1 * y2 + x2 * y1) / sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

		if (IsNearZero (distance)) {
			distance = 0;
		} else if (IsNegative (distance)) {		//if the line does not pass through the origin, we want the normal vector to point in the direction of the line
			distance = -distance;
		}
		return distance;
	}


	static Vector2d GetLineOfSectorDirectionVector (const Sector& sect) {
		double x1 = sect.c1.x, x2 = sect.c2.x;
		double y1 = sect.c1.y, y2 = sect.c2.y;
		double d = (-x1 * y2 + x2 * y1) / sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

		DBASSERT (IsNotNearZero (sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2))));

		Vector2d perpVector;
		perpVector.x = (y1 - y2) / sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
		perpVector.y = (x2 - x1) / sqrt ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));

		if (IsNearZero (d)) {
			//if the line passes through the origin, we want the x coordinate of the normal vector to be nonnegative
			//if the x coordinate is 0, we want y to be nonnegative
			if (IsNearZero (perpVector.x)) {
				perpVector.x = 0;
				if (IsNegative (perpVector.y)) {
					perpVector.y = -perpVector.y;
				}
			}
			if (IsNegative (perpVector.x)) {
				perpVector.x = -perpVector.x;
				perpVector.y = -perpVector.y;
			}
		} else if (IsNegative (d)) {		//if the line does not pass through the origin, we want the normal vector to point in the direction of the line
			perpVector.x = -perpVector.x;
			perpVector.y = -perpVector.y;
		}

		return perpVector;
	}

	static bool IsLessLineOfSectorByXYD (const Sector& sect1, const Sector& sect2) {

		Vector2d line1PerpVector = GetLineOfSectorDirectionVector (sect1);
		double line1D = GetLineOfSectorDistanceFromOrigo (sect1);

		Vector2d line2PerpVector = GetLineOfSectorDirectionVector (sect2);
		double line2D = GetLineOfSectorDistanceFromOrigo (sect2);

		return (IsLess (line1PerpVector.x, line2PerpVector.x) || (IsNear (line1PerpVector.x, line2PerpVector.x) &&
			(IsLess (line1PerpVector.y, line2PerpVector.y) || (IsNear (line1PerpVector.y, line2PerpVector.y) && IsLess (line1D, line2D)))));
	}

	static bool IsNearLineOfSector (const Sector& sect1, const Sector& sect2) {

		Vector2d line1PerpVector = GetLineOfSectorDirectionVector (sect1);
		double line1D = GetLineOfSectorDistanceFromOrigo (sect1);

		Vector2d line2PerpVector = GetLineOfSectorDirectionVector (sect2);
		double line2D = GetLineOfSectorDistanceFromOrigo (sect2);

		return IsNear (line1PerpVector.x, line2PerpVector.x) && (IsNear (line1PerpVector.y, line2PerpVector.y) && IsNear (line1D, line2D));
	}

	static bool IsLessByLine2DBegXYEndXY (const Sector& sect1, const Sector& sect2) {
		return IsLessLineOfSectorByXYD (sect1, sect2) || (IsNearLineOfSector (sect1, sect2) &&
			(IsLessByXY (GetMinEndC (sect1), GetMinEndC (sect2)) || (IsNear (GetMinEndC (sect1), GetMinEndC (sect2)) && IsLessByXY (GetMaxEndC (sect1), GetMaxEndC (sect2)))));
	}

	static bool DoSectorsOverlap (const Sector& sect1, const Sector& sect2) {
		return XSectorsOverLapByDist (sect1, sect2);
	}

	static bool DoTheyOverlap (const SectorOfAPoly& edge1, const SectorOfAPoly& edge2)
	{
		return DoSectorsOverlap (edge1.item, edge2.item);
	}

	static bool DoTheyOverlap (const ArcOfAPoly& edge1, const ArcOfAPoly& edge2)
	{
		return DoArcsOverlap (edge1.item, edge2.item);
	}

	static bool IsLessEdge (const SectorOfAPoly& edge1, const SectorOfAPoly& edge2)
	{
		return IsLessByLine2DBegXYEndXY (edge1.item, edge2.item);
	}

	static bool IsLessEdge (const ArcOfAPoly& edge1, const ArcOfAPoly& edge2)
	{
		return IsLessByCircleAngles (edge1.item, edge2.item);
	}
};
} // namespace Geometry

#endif