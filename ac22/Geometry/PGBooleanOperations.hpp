// *****************************************************************************
// Polygon Boolean Operations
//
// Module: Geometry
// Contact person: HK
//
// *****************************************************************************

#ifndef PGBOOLEANOPERATIONS_H
#define PGBOOLEANOPERATIONS_H

#include "Polygon2DOperations.h"

namespace Geometry {

enum FillingMethod { Alternative, WindingRulePos, WindingRule };
typedef bool (*BoolFunctionType) (bool, bool);

class GEOMETRY_DLL_EXPORT PGBooleanOperations
{
	void* pgBooleanObject;

public:
	PGBooleanOperations (GS::AbortInterfaceBase* breaker);
	~PGBooleanOperations ();

	PGRelPolyPosExt	DoBoolean (const PGPOLYEXT *poly1,
							   const PGPOLYEXT *poly2,
							   FillingMethod f1,
							   FillingMethod f2,
							   BoolFunctionType boolFunction,
							   bool* isPolyRegular = nullptr,
							   PolyHoleHandling holeHandling = ReturnHoles);					// can throw

	void			GetResultPolygons (PGRESPOLYEXT* newPolygons);			// can throw
	void			PrintBooleanReport (const char* opName, PGRelPolyPosExt status, GS::OChannel& outChannel);
};

GEOMETRY_DLL_EXPORT
void GEOM_CALL	PolyUnion (const PGPOLYEXT &poly1,
						   const PGPOLYEXT &poly2,
						   PGRESPOLYEXT* newPolygons,
						   PGRelPolyPosExt *status,
						   GS::AbortInterfaceBase* breaker = nullptr);			// can throw

GEOMETRY_DLL_EXPORT
void GEOM_CALL	PolyIntersection (const PGPOLYEXT &poly1,
								  const PGPOLYEXT &poly2,
								  PGRESPOLYEXT* newPolygons,
								  PGRelPolyPosExt *status,
								  GS::AbortInterfaceBase* breaker = nullptr);		// can throw

GEOMETRY_DLL_EXPORT
void GEOM_CALL	PolyDifference (const PGPOLYEXT &poly1,
								const PGPOLYEXT &poly2,
								PGRESPOLYEXT* newPolygons,
								PGRelPolyPosExt *status,
								GS::AbortInterfaceBase* breaker = nullptr);		// can throw

GEOMETRY_DLL_EXPORT
void GEOM_CALL	PolyRegularize (const PGPOLYEXT			&poly,
								PGRESPOLYEXT*			newPolygons,
								PGRegulHoleHandling		regul,
								GS::AbortInterfaceBase*	breaker = nullptr,
								PGRelPolyPosExt*		status = nullptr,
								PolyRegularizationMode	mode = PolyRegularizationMode_Default);// can throw

GEOMETRY_DLL_EXPORT
void GEOM_CALL	PolyPosition (const PGPOLYEXT			&poly1,
							  const PGPOLYEXT			&poly2,
							  PGRelPolyPosExt			*status,
							  GS::AbortInterfaceBase	*breaker = nullptr);			// can throw

GEOMETRY_DLL_EXPORT
bool GEOM_CALL	IsPolyRegular (const PGPOLYEXT &poly, GS::AbortInterfaceBase* breaker = nullptr);			// can throw


namespace ResultSegmentPos {
	enum Et { Empty = 0x00, Inside = 0x01, Outside = 0x02, OnEdge = 0x04,
					InsideOrOn = Inside | OnEdge, OutsideOrOn = Outside | OnEdge, NotOnEdge = Inside | Outside };
}

GEOMETRY_DLL_EXPORT
PGPositionExt GEOM_CALL		SectorInPoly (const PGPOLYEXT &poly,
										  FillingMethod fMeth,
										  const Sector &sector,
										  ResultSegmentPos::Et pos,
										  PGRESSECTOREXT *newSectors,
										  GS::AbortInterfaceBase* breaker = nullptr);			// can throw

struct CircleArc
{
	Coord	begC, endC;
	Coord	origo;
};

GEOMETRY_DLL_EXPORT
PGPositionExt GEOM_CALL		ArcInPoly (const PGPOLYEXT &poly,
									   FillingMethod fMeth,
									   const CircleArc &arc,
									   ResultSegmentPos::Et pos,
									   GS::Array<CircleArc> *newArcs,
									   GS::AbortInterfaceBase* breaker = nullptr);			// can throw

}

#endif
