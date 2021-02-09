#ifndef GEOMETRY_POLYGON2D_OVERLAPPINGEDGE_RESULT_HPP
#define GEOMETRY_POLYGON2D_OVERLAPPINGEDGE_RESULT_HPP
#pragma once

#include "GeometricDefinitions.h"
#include "Coord.h"
#include "GenArc2DData.h"
#include "Sector2DData.h"

#include "Algorithms.hpp"

namespace Geometry {


template <class EdgeType>
class EdgeOfTwoPolys {

public:
	EdgeType item;
	UIndex polyIdx1, edgeIdx1, polyIdx2, edgeIdx2;
	bool isOrientationReversed1, isOrientationReversed2;

	EdgeOfTwoPolys (EdgeType item, UIndex polyIdx1, UIndex edgeIdx1, UIndex polyIdx2, UIndex edgeIdx2, bool isOrientationReversed1 = false, bool isOrientationReversed2 = false) {
		this->item = item;
		this->polyIdx1 = polyIdx1;
		this->edgeIdx1 = edgeIdx1;
		this->isOrientationReversed1 = isOrientationReversed1;
		this->polyIdx2 = polyIdx2;
		this->edgeIdx2 = edgeIdx2;
		this->isOrientationReversed2 = isOrientationReversed2;
	}

};

template <class EdgeType>
class EdgeOfAPoly {

public:
	EdgeType item;
	UIndex polyIdx, edgeIdx;
	bool isOrientationReversed;

	EdgeOfAPoly (EdgeType item, UIndex polyIdx, UIndex edgeIdx, bool isOrientationReversed = false) {
		this->item = item;
		this->polyIdx = polyIdx;
		this->edgeIdx = edgeIdx;
		this->isOrientationReversed = isOrientationReversed;
	}

	const EdgeType GetItem (void) const { return item; }
};

typedef EdgeOfAPoly<Sector> SectorOfAPoly;
typedef EdgeOfAPoly<GenArc> ArcOfAPoly;
typedef EdgeOfTwoPolys<Sector> SectorOfTwoPolys;
typedef EdgeOfTwoPolys<GenArc> ArcOfTwoPolys;



class GEOMETRY_DLL_EXPORT IOverlappingEdgeResult {
private:
	bool isVisible = true;
public:
	virtual bool IsItOverlapping (void) const = 0;
	virtual const Point2D& GetStartPoint (void) const = 0;
	virtual const Point2D& GetEndPoint (void) const = 0;	
	virtual bool IsOrientationReversed (void) const = 0;
	virtual ~IOverlappingEdgeResult ();
	virtual void SetVisible (bool visible) {
		isVisible = visible;
	}
	
	virtual bool IsVisible (void) const {
		return isVisible;
	}

	virtual	IOverlappingEdgeResult*	 Clone (void) const = 0;
};


class GEOMETRY_DLL_EXPORT OverlappingSectorResult : public IOverlappingEdgeResult {
private:
	EdgeOfAPoly<Sector> item;
	bool isItOverlapping;
private:
	virtual const Point2D& GetC1 (void) const {
		return GetItem ().item.c1;
	}

	virtual const Point2D& GetC2 (void) const {
		return GetItem ().item.c2;
	}

	OverlappingSectorResult* Clone (void) const override
	{
		return new OverlappingSectorResult (*this);
	}

public:
	OverlappingSectorResult (EdgeOfAPoly<Sector> val, bool overlapping) :
		item (val),
		isItOverlapping (overlapping) {

	}


	const EdgeOfAPoly<Sector>& GetItem (void) const {
		return item;
	}


	virtual bool IsItOverlapping (void) const override {
		return isItOverlapping;
	}


	virtual const Point2D& GetStartPoint (void) const override {
		return !GetItem ().isOrientationReversed ? GetC1 () : GetC2 ();
	}


	virtual const Point2D& GetEndPoint (void) const override {
		return !GetItem ().isOrientationReversed ? GetC2 () : GetC1 ();
	}


	virtual bool IsOrientationReversed (void) const override;
};


class GEOMETRY_DLL_EXPORT OverlappingArcResult : public IOverlappingEdgeResult {
private:
	EdgeOfAPoly<GenArc> item;
	bool isItOverlapping;
public:
	OverlappingArcResult (EdgeOfAPoly<GenArc> val, bool overlapping) :
		item (val),
		isItOverlapping (overlapping) {

	}

	OverlappingArcResult* Clone (void) const override
	{
		return new OverlappingArcResult (*this);
	}

	const EdgeOfAPoly<GenArc>& GetItem (void) const {
		return item;
	}


	virtual bool IsItOverlapping (void) const override {
		return isItOverlapping;
	}


	virtual const Point2D& GetStartPoint (void) const override {
		return !GetItem ().isOrientationReversed ? GetItem ().item.GetBegC () : GetItem ().item.GetEndC ();
	}


	virtual const Point2D& GetEndPoint (void) const override {
		return !GetItem ().isOrientationReversed ? GetItem ().item.GetEndC () : GetItem ().item.GetBegC ();
	}


	virtual bool IsOrientationReversed (void) const override;
};


class Polygon2DOverlappingEdgeResults {

private:
	UIndex edgeIdx;	

	GS::Array<IOverlappingEdgeResult*>	edgeParts;
public:
	Polygon2DOverlappingEdgeResults () = delete;
	Polygon2DOverlappingEdgeResults& operator= (const Polygon2DOverlappingEdgeResults& source) = delete;
	Polygon2DOverlappingEdgeResults (const Polygon2DOverlappingEdgeResults& source) = delete;
	
	Polygon2DOverlappingEdgeResults (Polygon2DOverlappingEdgeResults&& source) {
		edgeParts = std::move (source.edgeParts);
		source.edgeParts.Clear ();
	}

	
	Polygon2DOverlappingEdgeResults& operator= (Polygon2DOverlappingEdgeResults&& source) {
		Clear ();
		edgeParts = std::move (source.edgeParts);
		source.edgeParts.Clear ();

		return *this;
	}


	Polygon2DOverlappingEdgeResults (UIndex edgeIdx) :
		edgeIdx (edgeIdx)
	{
	}


	~Polygon2DOverlappingEdgeResults() {
		Clear ();
	}

	void Clear () 
	{
		for (IOverlappingEdgeResult* edgePart : edgeParts) {
			delete edgePart;
			edgePart = nullptr;
		}
		edgeParts.Clear ();
	}

	void AddResult (EdgeOfAPoly<Sector> val, bool overlapping, bool isVisible = true)
	{	
		if (val.GetItem ().GetLength () < Eps)
			return;

		IOverlappingEdgeResult* edgePart = new OverlappingSectorResult(val, overlapping);
		edgePart->SetVisible (isVisible);
		edgeParts.Push (edgePart);
	}

	void AddResult (EdgeOfAPoly<GenArc> val, bool overlapping, bool isVisible = true)
	{
		if (Geometry::IsNear (val.GetItem ().GetBegC (), val.GetItem ().GetEndC ()))
			return;

		IOverlappingEdgeResult* edgePart = new OverlappingArcResult (val, overlapping);
		edgePart->SetVisible (isVisible);
		edgeParts.Push (edgePart);
	}


	int GetPartNum () const {
		return edgeParts.GetSize();
	}


	IOverlappingEdgeResult* GetEdgpePartFromEndPoint (Point2D endPoint) const {
		int results = 0;
		IOverlappingEdgeResult* result = edgeParts[0];
		for (const auto& edgePart : edgeParts) {
			if (Geometry::IsNear (edgePart->GetEndPoint (), endPoint)){ 
				++results;
				result = edgePart;
			}
		}

		if (results != 1) {
			DBBREAK ();
			throw GS::GeneralException ();
		}

		return result;
	}

	
	IOverlappingEdgeResult* GetEdgpePartFromStartPoint (Point2D startPoint, GSErrCode& err) const {
		int results = 0;
		IOverlappingEdgeResult* result = edgeParts[0];
		for (const auto& edgePart : edgeParts) {
			if (Geometry::IsNear (edgePart->GetStartPoint (), startPoint)) {
				++results;
				result = edgePart;
			}
		}

		if (results != 1) {
			DBBREAK ();
			err = Error;
			return edgeParts[0];
		}
		return result;
	}


	template <class MultiPolygonType>
	GSErrCode OrderEdgeParts (MultiPolygonType& polys) {
		GSErrCode err = NoError;
		if (edgeParts.GetSize () > 1) {
			const OverlappingSectorResult* sectorResult = dynamic_cast<const OverlappingSectorResult*> (edgeParts.GetFirst ());
			if (sectorResult != nullptr) {
				GS::Sort (edgeParts.Begin (), edgeParts.End (), [] (const IOverlappingEdgeResult* const s1, const IOverlappingEdgeResult* const s2) -> bool {
					if (s1->IsOrientationReversed ())
						return IsGreaterByXY (s1->GetStartPoint (), s2->GetStartPoint ());
					else
						return IsLessByXY (s1->GetStartPoint (), s2->GetStartPoint ());
				});
			} else {
				GS::Array<IOverlappingEdgeResult*>	orderedEdgeParts;
				const OverlappingArcResult* arcResult = dynamic_cast<const OverlappingArcResult*> (edgeParts.GetFirst ());
				if (DBVERIFY (arcResult != nullptr)) {
					auto edgeIdx = arcResult->GetItem ().edgeIdx + 1;
					auto polyIdx = arcResult->GetItem ().polyIdx;
					GS::Optional<GenArc> genArc = polys[polyIdx].GetEdgeIterator (edgeIdx)->GetArc ();
					if (DBVERIFY (genArc.HasValue ())){
						orderedEdgeParts.PushNew (GetEdgpePartFromStartPoint (genArc->GetBegC (), err));
					} else {
						err = Error;
					}
					for (int i = 0; i < GetPartNum () - 1; i++) {
						orderedEdgeParts.PushNew (GetEdgpePartFromStartPoint (orderedEdgeParts[i]->GetEndPoint (), err));
					}
					if (err == NoError)
						edgeParts = orderedEdgeParts;
				} else
					err = Error;
			}
		} else if (edgeParts.GetSize () == 0) {
			err = Error;
		}

		return err;
	}


	const IOverlappingEdgeResult* operator[](UIndex idx) const {
		return edgeParts[idx];
	}


	const GS::Array<IOverlappingEdgeResult*>& GetParts (void) const {
		return edgeParts;
	}


	bool IsEdgeStartingAsHidden () const {
		if (DBVERIFY (edgeParts[0] != nullptr)) {
			return edgeParts[0]->IsItOverlapping ();
		}
		return false;
	}


	GS::Array<GS::Pair<Point2D, bool>> GetCutPoints (bool currentCutHidden, bool nextCutHidden) const {
		GS::Array<GS::Pair<Point2D, bool>> cutPoints;

		for (IOverlappingEdgeResult* edgePart : edgeParts) {
			if (edgePart->IsItOverlapping () != currentCutHidden) {
				cutPoints.PushNew(edgePart->GetStartPoint (), true);
				currentCutHidden = !currentCutHidden;
			} else if (edgePart != edgeParts[0]){
				cutPoints.PushNew(edgePart->GetStartPoint (), false);
			}
		}

		if (currentCutHidden && !nextCutHidden) {
			cutPoints.PushNew (edgeParts.GetLast()->GetEndPoint(), true);
		} else {
			cutPoints.PushNew (edgeParts.GetLast()->GetEndPoint(), false);
		}

		return cutPoints;
	}


};

}
#endif