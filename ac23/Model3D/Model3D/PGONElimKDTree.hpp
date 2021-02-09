// *********************************************************************************************************************
// Description:		KDTree structures for PGONElim
//
// Module:			Model3D
// Contact person:	PP, HR
//
// SG compatible
// *********************************************************************************************************************

#if !defined (PGONELIMKDTREE_HPP)
#define PGONELIMKDTREE_HPP

#pragma once
// from Geometry
#include	"GeometricDefinitions.h"
#include	"KDTree.hpp"
#include	"KDTreeKeys.hpp"
#include	"Polygon2DOperations.h"

// from Model3D
#include	"Model3D/Model3D.hpp"

// from GSUtils
#include	"ExecutorForTT.hpp"

namespace PGONElimination {

struct	PolyPosData
{
	UIndex					elemIdx;
	UIndex					bodyIdx;
	UIndex					pgonIdx;
	UIndex					pind;
	const Modeler::Elem		*elem;
	const Modeler::MeshBody	*body;
	const Modeler::PGON		*pgon;

	double					xMin, yMin, xMax, yMax;
	enum Winner {
		X, Y, Z
	}						winner;
	bool					revert;

	bool					solid;

	GS::ArrayFB<Int32, 8>	zeroPedg;


	PolyPosData () : elemIdx (0), bodyIdx (0), pgonIdx (0), pind (0),
		elem (nullptr), body (nullptr), pgon (nullptr),
		xMin (0.0), yMin (0.0), xMax (-1.0), yMax (-1.0), winner (X), revert (false), solid (true)
	{
	}
};

// KD-tree Key type
class KDPoly : public Geometry::KDTreeKeyTemplate<double, 10>
{
private:
	KDPoly (double c0, double c1, double c2, double c3, double c4, double c5, double c6, double c7, double c8, double c9);
public:
	KDPoly () = default; //lint !e1401 [member not initialized by constructor]; hatekonysagi okok miatt eltekintunk tole
	KDPoly (double xMin, double yMin, double zMin, double xMax, double yMax, double zMax, const Vector3D& nv, double dist);

	// Get opposite normal vector boundbox 
	KDPoly GetOppositeBoundBoxMin () const;
	KDPoly GetOppositeBoundBoxMax () const;

	KDPoly GetBoundBoxMin () const;
	KDPoly GetBoundBoxMax () const;
};


// KD-tree Key type wrapper
class KDPolyWrapper {
private:
	const KDPoly	&poly;

public:
	static const short			DimCnt = KDPoly::DimCnt;
	typedef KDPoly::DimType		DimType;

	KDPolyWrapper (const KDPoly& poly) : poly (poly) {}

	inline DimType	GetDimVal (UInt32 dimIdx) const
	{
		return poly.GetDimVal (dimIdx);
	}
};


// KD-tree Value type
struct KDPGON
{
	PolyPosData		*ppd;

	KDPGON () : ppd (nullptr) {}
	KDPGON (PolyPosData *ppd) : ppd (ppd) {}
};


// KD-tree Key and Value pair
struct TreeData {
	KDPoly	kdPoly;
	KDPGON	pgon;

	TreeData () : kdPoly (), pgon () {}
	TreeData (const KDPoly& kdPoly, const KDPGON& pgon) : kdPoly (kdPoly), pgon (pgon) {}
};


class RangeQueryAdapter
{
private:
	GS::Array<KDPGON*>& array;
	RangeQueryAdapter& operator= (const RangeQueryAdapter& other);
public:
	RangeQueryAdapter (GS::Array<KDPGON*>& results) : array (results) {}
	void operator () (KDPGON* item) { array.Push (item); }
};


// for the optimized Rebuild
class TreeSource {
private:
	TreeData*	treeData;
	USize		dataCount;
public:

	TreeSource (TreeData* treeData, USize dataCount) : treeData (treeData), dataCount (dataCount) {}

	KDPolyWrapper	GetKey (UIndex index) const { return KDPolyWrapper (treeData[index].kdPoly); }
	KDPGON*			GetValue (UIndex index) const { return &treeData[index].pgon; }
	USize			GetSize ()				const { return dataCount; }
};


// the coplanar PGON-finder KD-tree type
class MODEL3D_DLL_EXPORT PGONElimTree
{
private:
	typedef Geometry::KDTree<KDPolyWrapper, KDPGON*, Geometry::KDTreeNodeBoundsEnabled, Geometry::KDTreeNormalUpdate, Geometry::KDTreeElemMultiset>		TreeType;
	static bool CmpKDPGON (const KDPGON* p1, const KDPGON* p2);
	GS::AutoPtr<TreeType>	pgonElimKDTree;

	PGONElimTree (const PGONElimTree &);				// disabled
	PGONElimTree &operator = (const PGONElimTree &);	// disabled

public:

	PGONElimTree ();

	void			AddMultipleElements (TreeData* data, USize dataCount);
	void			Dump (GS::OChannel& ch) const;

	bool			IsEmpty (void) const;
	void			FindCoplanarPossibleOverlappingPGONs (const KDPoly& poly, GS::Array<KDPGON*>& coplanarPGONs) const;
	virtual void	FindCoplanarPossibleCrossingPGONs (const KDPoly& poly, GS::Array<KDPGON*>& coplanarPGONs) const;
};


typedef std::function<UInt32 (const Modeler::Model3DViewer& viewer, const GS::HashTable<UIndex, bool> *elimElems, bool skipGDLElements)> ValidPolygonEnumeratorFunction;
class MODEL3D_DLL_EXPORT KDTreeBuilder {
private:
	ValidPolygonEnumeratorFunction enumFunction;


	PGONElimination::PGONElimTree*			tree;

	UInt32							size;
public:
	KDTreeBuilder (ValidPolygonEnumeratorFunction enumFunction);
	~KDTreeBuilder();
	void										BuildKDTree (GS::ProcessControl& processControl, 
															 const Modeler::Model3DViewer& viewer, 
															 const GS::HashTable<UIndex, bool> *elimElems,
															 bool skipGDLElements,
															 GS::Array<PGONElimination::TreeData>&	 treeData,
															 GS::Array<PGONElimination::PolyPosData>&  polyPosData);
	PGONElimination::PGONElimTree*				GetKDTree () {return tree;}
	UInt32										GetSize ();

};

}

#endif