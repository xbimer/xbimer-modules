// *********************************************************************************************************************
// Description:		Wall View Related Derived Data
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************

#ifndef	WALLVRDDATA_HPP
#define	WALLVRDDATA_HPP

#pragma once

// from Geometry
#include "IrregularPolygon2DClassDefinition.hpp"

// from VBElemOperations
#include "VBElemOperationsExport.hpp"
#include "VRDData.hpp"
#include "WallFloorPlanCalculations.hpp"

// --- Type definitions	------------------------------------------------------------------------------------------------

namespace VBD {
	class WallVRDData;
	typedef ODB::Ref<WallVRDData> WallVRDDataRef;
	typedef ODB::ConstRef<WallVRDData> WallVRDDataConstRef;
}

#include "WallWPolyCalculations.hpp"

// --- Forward declarations	--------------------------------------------------------------------------------------------

namespace VBElem {
class Wall;
typedef ODB::ConstRef<Wall> WallConstRef;
}

namespace VBCalculations {
	class WallNotViewRelatedPolygons;
}

namespace VBD {
	class WallConnectionData;
}

// === WallVRDData class ===============================================================================================

namespace VBD {

class VB_ELEM_OPERATIONS_DLL_EXPORT WallVRDData : public VRDData
{
	DECLARE_DYNAMIC_CLASS_INFO (WallVRDData)

	//wPoly calculation
	friend VBD::WallVRDDataRef GetNotFullyCalculatedViewRelatedDerivedData (VBElem::ModelElementConstRef elem, VBD::ViewConstRef view);
	friend void FinalizeAndStoreWPoly (Coord* p, bool* arcWall, Int32* pind, double* tp, VBD::WallVRDDataRef vrdData,
									   VBElem::WallConstRef wall, VBD::ViewConstRef view, const VBCalculations::WallCutPlaneData* wCutPlane, 
									   VBCalculations::WallNotViewRelatedPolygons* wallNotVRDPolys);

public:
	class IsolatedState : public VRDData::IsolatedState
	{
		friend class WallVRDData;

	public:
		Geometry::IrregularPolygon2D	wPoly;
		Geometry::IrregularPolygon2D	poly;

		virtual ~IsolatedState ();
	};

private:
	Geometry::IrregularPolygon2D	wPoly;
	Geometry::IrregularPolygon2D	poly; //= empty if the underlying wall is polygonal

	GS::ErrCode								CalculatePolygon	(VBElem::WallConstRef wall, VBD::ViewConstRef view);
	GS::ErrCode								CalculateWPolygon	(VBElem::WallConstRef wall, VBD::ViewConstRef view, const WallConnectionData* params);
	void									CopyPoly2WPoly		(void);

	virtual GS::ErrCode						CalculateData (VBElem::ModelElementConstRef elem, VBD::ViewConstRef view, const ConnectionData* params) override;

	void									SetPolygon (Geometry::IrregularPolygon2D source);
	void									SetWPolygon (const Geometry::IrregularPolygon2D& source);

	Geometry::IrregularPolygon2D&			GetPolygon ();
	Geometry::IrregularPolygon2D&			GetWPolygon () { return wPoly; };

public:
	WallVRDData ();
	WallVRDData (const WallVRDData& source);
	~WallVRDData ();

	static void								SetupAndStartWPolyCalculation (VBElem::WallConstRef								wall,
																		   VBD::ViewConstRef								view,
																		   const WallConnectionData*						params,
																		   WallVRDDataRef									vrdData,
																		   const VBCalculations::WallCutPlaneData*			wCutPlane = nullptr,
																		   VBCalculations::WallNotViewRelatedPolygons*		wallNotVRDPolys = nullptr,
																		   VBCalculations::WallComputingEnvir* const		plusWce = nullptr,
																		   VBCalculations::WallBeamConnectionEnvir* const	plusWbce = nullptr);

	virtual WallVRDData*					Clone (void) const override;

	virtual GSErrCode						StoreIsolatedState (ODB::Object::IsolatedState*		iState) const override;
	virtual GSErrCode						RestoreIsolatedState (const ODB::Object::IsolatedState*	iState) override;
	virtual GSErrCode						WriteIsolatedState (GS::OChannel&						oc) const override;
	virtual GSErrCode						ReadIsolatedState (GS::IChannel&						ic) override;
	virtual ODB::Object::IsolatedState*		CreateIsolatedState (void) const override;
	virtual USize							GetIsolatedSize (void) const override;

	const Geometry::IrregularPolygon2D&		GetPolygon () const;
	const Geometry::IrregularPolygon2D&		GetWPolygon () const { return wPoly; };

	virtual void							Transform (const TRANMAT* tran) override;

	static void								InvalidateByView (VBD::ViewRef view);

private:
	virtual bool							IsCalculated () const override;
};

}		//namespace VBD


#endif