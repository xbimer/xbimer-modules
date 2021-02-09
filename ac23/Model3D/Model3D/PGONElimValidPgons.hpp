// *********************************************************************************************************************
// Description:		Searching Valid Pgons for PGONElim
//
// Module:			Model3D
// Contact person:	PP, HR
//
// SG compatible
// *********************************************************************************************************************

#if !defined (PGONELIMVALIDPGONS_HPP)
#define PGONELIMVALIDPGONS_HPP

#include "PGONElimKDTree.hpp"



// from Brep
#include	"MeshBrepAccessors.hpp"

// from Model3D
#include	"Model3D/ElimUtils.hpp"
#include	"Model3D/Model3DExport.h"

namespace PGONElimination {



// PGON collector for the KD-tree
class PGONCollector {
private:
	GS::Array<TreeData>&				data;
	GS::Array<PolyPosData>&				polyPosData;
	const Modeler::Model3DViewer&		viewer;
	GS::AtomicCounter					insertIndex;

public:
	PGONCollector (GS::Array<TreeData>& data, GS::Array<PolyPosData>& polyPosData, const Modeler::Model3DViewer& viewer);

	void operator () (UInt32 elemIdx, UInt32 bodyIdx, UInt32 pgonIdx);

private:
	void AddPgon (UInt32 elemIdx, UInt32 bodyIdx, UInt32 pgonIdx, const Vector3D& n, const Coord3D& c, const Box3D& box);
};


class CollectValidPGONs : public TT::Runnable {
private:
	const Modeler::Model3DViewer&	viewer;
	PGONCollector&					pgonCollector;
	bool							skipGDLElements;
	Modeler::Attributes::Viewer&	attrViewer;
	const GS::Array<UIndex>*		elemIndices;
	GS::AtomicCounter&				counter;

	void Run ()  override;
public:
	CollectValidPGONs (const Modeler::Model3DViewer& viewer, PGONCollector& pgonCollector, bool skipGDLElements,
		Modeler::Attributes::Viewer& attrViewer, const GS::Array<UIndex>* elemIndices, GS::AtomicCounter& counter);

	virtual ~CollectValidPGONs ();
};


class MODEL3D_DLL_EXPORT PGONRelPosCalculator {
public:
	virtual Geometry::PGRelPolyPosExt	GetPGONRelPos (PolyPosData &p1, PolyPosData &p2, GS::ProcessControl& processControl);
	virtual Geometry::PGRelPolyPosExt	GetPGONRelPosMulti (PolyPosData &p1, GS::Array<PolyPosData*> &p2arr, GS::ProcessControl& processControl);

	template <typename Op>
	static void EnumerateElemPGONs (const Modeler::Elem& elem, UIndex elemIdx, const Modeler::Attributes::Viewer& attrViewer,
		bool skipGDLElements, Op& op);
};


template <typename Op>
void PGONRelPosCalculator::EnumerateElemPGONs (const Modeler::Elem& elem, UIndex elemIdx, const Modeler::Attributes::Viewer& attrViewer,
	bool skipGDLElements, Op& op)
{
	if (elem.IsInvalid ())
		return;

	const short elemType = elem.GetElemId ().etype;

	for (UIndex bodyIdx = 0; bodyIdx < elem.GetTessellatedBodyCount (); ++bodyIdx) {
		const Modeler::MeshBody& body = elem.GetConstTessellatedBody (bodyIdx);

		if (!body.HasVertices ())
			continue;

		const short ibtype = body.GetShapeType ();

		if (Modeler::SkipBody (elemType, ibtype, skipGDLElements) == Modeler::Skip)
			continue;

		bool transparent = false;
		for (UIndex pgonIdx = 0; pgonIdx < body.GetPolygonCount (); ++pgonIdx) {
			GSAttributeIndex iumat = body.GetConstPolygonAttributes (pgonIdx).GetMaterialIndex ();
			if (iumat < 1 || ULong (iumat) > attrViewer.GetMaterialCount ())
				continue;

			const UMAT*	aumat = attrViewer.GetConstMaterialPtr (iumat);
			if (aumat->GetTransparency () > 0) {
				transparent = true;
				break;
			}
		}
		if (transparent)
			continue;

		for (UIndex pgonIdx = 0; pgonIdx < body.GetPolygonCount (); ++pgonIdx) {
			op (elemIdx, bodyIdx, pgonIdx);
		}
	}
}

}

#endif