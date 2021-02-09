// *********************************************************************************************************************
// Description:		Wall Not View Related Polygons
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	VaLa
//
// SG compatible
// *********************************************************************************************************************

#ifndef	WALLNOTVIEWRELATEDPOLYGONS_HPP
#define	WALLNOTVIEWRELATEDPOLYGONS_HPP

#pragma once

// --- Includes	--------------------------------------------------------------------------------------------------------

//from GSRoot
#include "HashTable.hpp"
#include "Pair.hpp"

//from Geometry
#include "IrregularPolygon2DClassDefinition.hpp"

//from VBElemOperations
#include "WallFloorPlanCalculations.hpp"
#include "WallConnectionData.hpp"
#include "WallWPolyCalculations.hpp"

// --- Forward declarations	--------------------------------------------------------------------------------------------
namespace VBD {
	class WallVRDData;
	typedef ODB::Ref<VBD::WallVRDData> WallVRDDataRef;
}
// --- Type definitions	------------------------------------------------------------------------------------------------

namespace VBCalculations {
	typedef GS::Pair<const VBCalculations::WallCutPlaneData, VBElem::WallConstRef> WallCutPlaneWallPair;
}

// === WallNotViewRelatedPolygons class ================================================================================

namespace VBCalculations {

class VB_ELEM_OPERATIONS_DLL_EXPORT WallNotViewRelatedPolygons {

	friend void VBD::FinalizeAndStoreWPoly (Coord*	p, bool* arcWall, Int32* pind, double* tp, VBD::WallVRDDataRef vrdData,
											VBElem::WallConstRef wall, VBD::ViewConstRef view, const VBCalculations::WallCutPlaneData* wCutPlane, 
											VBCalculations::WallNotViewRelatedPolygons* wallNotVRDPolys);

private:
	short floorNum;

	GS::HashTable<WallCutPlaneWallPair, Geometry::IrregularPolygon2D> polys;
	GS::HashTable<WallCutPlaneWallPair, Geometry::IrregularPolygon2D> wPolys;

public:
	WallNotViewRelatedPolygons (short flNum);

	short									GetFloorNum () { return floorNum; }

private:
	void									SetPolygon	(const VBCalculations::WallCutPlaneData*	wCutPlane,
														 VBElem::WallConstRef						wall,
														 const Geometry::IrregularPolygon2D&		poly);
	void									SetWPolygon (const VBCalculations::WallCutPlaneData*	wCutPlane,
														 VBElem::WallConstRef						wall,
														 const Geometry::IrregularPolygon2D&		wPoly);

public:
	const Geometry::IrregularPolygon2D&		GetPolygon	(VBElem::WallConstRef						wall, 
														 VBD::ViewConstRef							baseView, 
														 const VBCalculations::WallCutPlaneData*	wCutPlane);
	const Geometry::IrregularPolygon2D&		GetWPolygon (VBElem::WallConstRef							wall,
														 VBD::ViewConstRef								baseView,
														 const VBD::WallConnectionData*					params,
														 const VBCalculations::WallCutPlaneData*		wCutPlane,
														 VBCalculations::WallComputingEnvir* const		plusWce = nullptr,
														 VBCalculations::WallBeamConnectionEnvir* const	plusWbce = nullptr);
};

}

#endif