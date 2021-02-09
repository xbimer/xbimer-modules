// *********************************************************************************************************************
// Additional info for profile data
//
// Module:			VectorImage
// Namespace:		VectorImage
// Contact person:	PT
//
// [SG compatible]
// *********************************************************************************************************************

#if !defined (PROFILEADDITIONALINFO_H)
#define PROFILEADDITIONALINFO_H

#pragma once

#include "VectorImageExport.h"

// from Geometry
#include "Coord.h"
#include "Sector2DData.h"
#include "TRANMAT.h"

// from GSRoot
#include "HashCommon.hpp"

#include "GSUtilsDefs.h"


namespace GS {
	class IChannel;
	class OChannel;
	class XMLIChannel;
	class XMLOChannel;
}

// Additional info for SyHatch

class VI_DLL_CLASS_EXPORT ProfileEdgeData {
private:
	short	material;
	short	pen;
	short	lineType;
	short	filler;			//not in use (AACS)
	Int32	flags;

public:
	enum class SideSelector	{
		Top,
		Right,
		Bottom,
		Left,
		Undefined
	};

	enum Flag {
		IsInnerLineFlag						= 0x00000001,
		IsCurvedFlag						= 0x00000002,
		SurfaceFromBuildMatFlag				= 0x00000004,		// B-215
		IsVisibleLineFlag					= 0x00000008,		// B-270 M2
		IsCutEndLineFlag					= 0x00000010,
		IsTopEdgeFlag						= 0x00000020,
		IsRightEdgeFlag						= 0x00000040,
		IsBottomEdgeFlag					= 0x00000080,
		IsLeftEdgeFlag						= 0x00000100,
		IsCoveredByCoreLineFlag				= 0x00000200,
		IsCoveredByFinishLineFlag			= 0x00000400,
		IsCoveredByOtherLineFlag			= 0x00000800,
		TemporaryNegativeOrientationFlag	= 0x80000000		// B-213
	};

	ProfileEdgeData ()
		: material (0)
		, pen (Pen_First)
		, lineType (SolidLineIndex)
		, filler (0)
		, flags (0)
	{
	}

	ProfileEdgeData (short material_init, short pen_init, short lineType_init, Int32 flags_init)
		: material (material_init)
		, pen (pen_init)
		, lineType (lineType_init)
		, filler (0)
		, flags (flags_init)
	{
	}

	bool	operator ==							(const ProfileEdgeData& other)		const	{ return material == other.material && pen == other.pen && lineType == other.lineType && flags == other.flags;	}
	bool	operator !=							(const ProfileEdgeData& other)		const	{ return !operator== (other);																					}
			operator GS::HashValue				(void)								const;

	short	GetMaterial							(void)								const	{ return material;																								}
	short	GetPen								(void)								const	{ return pen;																									}
	short	GetLineType							(void)								const	{ return lineType;																								}
	Int32	GetFlags							(void)								const	{ return flags;																									}

	void	SetMaterial							(short newMaterial)							{ material	= newMaterial;																						}
	void	SetPen								(short newPen)								{ pen		= newPen;																							}
	void	SetLineType							(short newLineType)							{ lineType	= newLineType;																						}
	void	SetFlags							(Int32 newFlags)							{ flags		= newFlags;																							}

	void	SetInnerLine						(bool value)								{ SetFlag (value, IsInnerLineFlag);						}
	void	SetCurved							(bool value)								{ SetFlag (value, IsCurvedFlag);						}
	void	SetSurfaceFromBuildMat				(bool value)								{ SetFlag (value, SurfaceFromBuildMatFlag);				}
	void	SetVisibleLine						(bool value)								{ SetFlag (value, IsVisibleLineFlag);					}
	void	SetCoveredByCoreLine				(bool value)								{ SetFlag (value, IsCoveredByCoreLineFlag);				}
	void	SetCoveredByFinishLine				(bool value)								{ SetFlag (value, IsCoveredByFinishLineFlag);			}
	void	SetCoveredByOtherLine				(bool value)								{ SetFlag (value, IsCoveredByOtherLineFlag);			}
	void	SetCutEndLine						(bool value)								{ SetFlag (value, IsCutEndLineFlag);					}
	void	SetTemporaryNegativeOrientation		(bool value)								{ SetFlag (value, TemporaryNegativeOrientationFlag);	}
	void	SetSideSelector						(SideSelector value);

	bool	IsInnerLine						() const { return (flags & IsInnerLineFlag)						!= 0; }
	bool	IsCurved						() const { return (flags & IsCurvedFlag)						!= 0; }
	bool	IsSurfaceFromBuildMat			() const { return (flags & SurfaceFromBuildMatFlag)				!= 0; }
	bool	IsVisibleLine					() const { return (flags & IsVisibleLineFlag)					!= 0; }
	bool	IsCoveredByCoreLine				() const { return (flags & IsCoveredByCoreLineFlag)				!= 0; }
	bool	IsCoveredByFinishLine			() const { return (flags & IsCoveredByFinishLineFlag)			!= 0; }
	bool	IsCoveredByOtherLine			() const { return (flags & IsCoveredByOtherLineFlag)			!= 0; }
	bool	IsCutEndLine					() const { return (flags & IsCutEndLineFlag)					!= 0; }
	bool	IsTemporaryNegativeOrientation	() const { return (flags & TemporaryNegativeOrientationFlag)	!= 0; }

	bool	IsCoveredLine					() const { return IsCoveredByCoreLine () || IsCoveredByFinishLine () || IsCoveredByOtherLine (); }
	bool	IsVisibleUnCoveredLine			() const { return IsVisibleLine () && !IsCoveredLine (); }
	
	SideSelector GetSideSelector () const;

	void	IV									(GS::PlatformSign from, GS::PlatformSign to);

private:
	void		SetFlag							(bool value, Int32 flag) { flags = (value) ? flags | flag : flags & ~flag; }

public:
	GSErrCode	ReadVersion1					(GS::IChannel& ic);
	GSErrCode	WriteVersion1					(GS::OChannel& oc)					const;

	GSErrCode	ReadXML							(GS::XMLIChannel& ic);
	GSErrCode	WriteXML						(GS::XMLOChannel& oc)				const;

	void		Print							(GS::OChannel& oc)					const;
};

#define ProfileItemVersion_Invalid	0		// invalid Profile Item
#define ProfileItemVersion_1		1		// original Profile Item
#define ProfileItemVersion_2		2		// Sy_Hatch contains building material and remove ProfileItemIsHatchAngle and ProfileItemIsHatchFit from ProfileItem

#define ProfileItemVersion			2


class VI_DLL_CLASS_EXPORT SkinProjectionProperties {

public:
	enum SkinProjectionType {
		SPT_Line,
		SPT_Circle
	};

private:
	enum {
		HasProjectionShapeFlag = 0x00000001
	};

private:
	Int32		flags;
	Int32 		projectionType;

	Coord		lineCoord1;
	Coord		lineCoord2;

	Coord		circleOrigo;
	double		circleRadius;

public:
	SkinProjectionProperties ();

	bool				HasProjectionShape		(void) const	{ return (flags & HasProjectionShapeFlag) != 0; }
	SkinProjectionType	GetProjectionType		(void) const	{ return (SkinProjectionType) projectionType; }
	const Coord&		GetLineCoord1			(void) const	{ return lineCoord1; }
	const Coord&		GetLineCoord2			(void) const	{ return lineCoord2; }
	const Coord&		GetCircleOrigo			(void) const	{ return circleOrigo; }
	double				GetCircleRadius			(void) const	{ return circleRadius; }

	void				SetHasProjectionShape	(bool value)				{ value ? flags |= HasProjectionShapeFlag : flags &= ~HasProjectionShapeFlag; }
	void				SetProjectionType		(SkinProjectionType type)	{ projectionType = (Int32) type; }
	void				SetLineCoord1			(const Coord& coord)		{ lineCoord1 = coord; }
	void				SetLineCoord2			(const Coord& coord)		{ lineCoord2 = coord; }
	void				SetCircleOrigo			(const Coord& coord)		{ circleOrigo = coord; }
	void				SetCircleRadius			(double radius)				{ circleRadius = radius; }

	bool				IsValid					(double maxCoordValue) const;
	void				Transform				(const TRANMAT* tran);

	void				IV						(GS::PlatformSign from, GS::PlatformSign to);

	GSErrCode			ReadXML					(GS::XMLIChannel& ic);
	GSErrCode			WriteXML				(GS::XMLOChannel& oc) const;

	bool				operator ==				(const SkinProjectionProperties& other) const;

	Sector				GetScaledLine			(void) const;
};


struct VI_DLL_CLASS_EXPORT ProfileItem {
public:
	Int32						profileItemVersion;
	Int32						obsoletePriorityValue;
private:
	Int32						flags;
	short						cutEndLinePen;
	short						cutEndLineType;

	SkinProjectionProperties	skinProjectionProperties;

public:
	enum {
		IsCoreFlag					= 0x00000001,
		IsFinishFlag				= 0x00000002,
		IsHatchAngleObsoleteFlag	= 0x00000010,
		IsHatchFitObsoleteFlag		= 0x00000020,
		VisibleCutEndLinesFlag		= 0x00000100
	};

	ProfileItem ()
		: profileItemVersion (ProfileItemVersion)
		, obsoletePriorityValue (0)
		, flags (VisibleCutEndLinesFlag)
		, cutEndLinePen (Pen_First)
		, cutEndLineType (SolidLineIndex)
	{
	}

	ProfileItem (short cutEndLinePen_init, short cutEndLineType_init, Int32 flags_init)
		: profileItemVersion (ProfileItemVersion)
		, obsoletePriorityValue (0)
		, flags (flags_init)
		, cutEndLinePen (cutEndLinePen_init)
		, cutEndLineType (cutEndLineType_init)
	{
	}

	bool							operator ==					(const ProfileItem& other)	const;
	bool							operator !=					(const ProfileItem& other)	const		{ return !operator== (other);															}
									operator GS::HashValue		(void)						const;

	Int32							GetFlags					(void)						const		{ return flags;																			}
	void							SetFlags					(Int32 newFlags)						{ flags = newFlags;																		}

	void							SetCore						(bool value)							{ flags = (value) ? flags | IsCoreFlag				: flags & ~IsCoreFlag;				}
	void							SetFinish					(bool value)							{ flags = (value) ? flags | IsFinishFlag			: flags & ~IsFinishFlag;			}
	void							SetVisibleCutEndLines		(bool value)							{ flags = (value) ? flags | VisibleCutEndLinesFlag	: flags & ~VisibleCutEndLinesFlag;	}

	bool							IsCore						(void)						const		{ return (flags & IsCoreFlag)				!= 0;										}
	bool							IsFinish					(void)						const		{ return (flags & IsFinishFlag)				!= 0;										}
	bool							IsVisibleCutEndLines		(void)						const		{ return (flags & VisibleCutEndLinesFlag)	!= 0;										}

	short							GetCutEndLinePen			(void)						const		{ return cutEndLinePen;																	}
	void							SetCutEndLinePen			(short value)							{ cutEndLinePen = value;																}

	short							GetCutEndLineType			(void)						const		{ return cutEndLineType;																}
	void							SetCutEndLineType			(short value)							{ cutEndLineType = value;																}

	const SkinProjectionProperties&	GetSkinProjectionProperties	(void)						const		{ return skinProjectionProperties;														}
	void							SetSkinProjectionProperties (const SkinProjectionProperties& value) { skinProjectionProperties = value;														}

	void							IV							(GS::PlatformSign from, GS::PlatformSign to);

	GSErrCode						ReadXML						(GS::XMLIChannel& ic);
	GSErrCode						WriteXML					(GS::XMLOChannel& oc)		const;

	static ProfileItem*				NewAndCopy					(const ProfileItem* source);
};

#endif
