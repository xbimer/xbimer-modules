// *****************************************************************************
// Composite Info for elem connections
//
// Namespaces:			Contact person:
//		CompInfo			PT
//
// [SG compatible]
// *****************************************************************************

#if !defined (COMPOSITEINFO_HPP)
#define COMPOSITEINFO_HPP

#include	"Array.hpp"
#include	"ProfileFillData.hpp"

#include	"VBElemOperationsExport.hpp"
#include	"VBElementsBase.hpp"

// from VBDocument
#include	"CurrentStoryCutLevels.hpp"



#define		MaxComp		127



namespace CompInfo {

class SkinFaceLine {
private:
	ADB::AttributeIndex	lineType;	// line type index of the skin face line
	short				pen;		// pen index of the skin face line
	bool				skip;		// skip this line

public:
	SkinFaceLine ();
	SkinFaceLine (const ADB::AttributeIndex& inLineType, short inPen, bool toSkip = false);

	inline	const ADB::AttributeIndex&	GetLineTypeIndex (void) const	{ return lineType;	}
	inline	short						GetPenIndex (void) const		{ return pen;		}
	inline	bool						IsLineToSkip (void) const		{ return skip;		}

	void			SetLineTypeIndex (const ADB::AttributeIndex& newLineType);
	void			SetPenIndex (short newPen);
	void			SetToSkip (bool toSkip);
};


enum SkinFlags {
	SFHatchAngle	= 1,
	SFCore			= 2,
	SFFinish		= 32,
	SFHatchFit		= 64
};


enum SkinType {
	Skin_Normal = 0,
	Skin_Upper  = 1,
	Skin_Down   = 2,
	Skin_Empty	= 3
};


class Skin {
private:
	CutSurface				cutSurface;
	short					endLinePen;		// Index of the outside frame pen
	ADB::AttributeIndex		endLineType;	// Index of the outside frame linetype
	short					flagBits;		// skin flags
	double					fillThick;		// absolute thickness of the skin in m
	Int32					connPriority;	// skin connection priority
	SkinType				skinType;		// Skin_Normal, Skin_Upper, Skin_Down, Skin_Empty
	Int32					hatchId;

public:
	Skin ();
	Skin	(VBAttr::BuildingMaterialConstRef buildMatRef,
			 const VBAttr::OverriddenPen& inFillPen,
			 const VBAttr::OverriddenPen& inFillBkgPen,
			 short inEndLinePen, const ADB::AttributeIndex& inEndLineType,
			 short inFlagBits, double inFillThick, Int32 inConnPriority, SkinType inSkinType, Int32 HatchId);

	Skin (
		const CutSurface & cutSurface_init,
		short inEndLinePen, const ADB::AttributeIndex& inEndLineType,
		short inFlagBits, double inFillThick, Int32 inConnPriority, CompInfo::SkinType inSkinType, Int32 HatchId);

	inline	const ADB::AttributeIndex&	GetPatternIndex (void) const		{ return cutSurface.GetFillIndex ();				}
	inline	const ADB::AttributeIndex&	GetBuildMatIndex (void) const		{ return cutSurface.GetBuildingMaterialIndex ();	}
	inline	void						GetCutSurface (CutSurface& actCutSurface) const		{ actCutSurface = cutSurface;		}
	inline	short						GetEndFacePenIndex (void) const 	{ return endLinePen;	}
	inline	const ADB::AttributeIndex&	GetEndFaceLTIndex (void) const 		{ return endLineType;	}
	inline	short						GetFlags (void) const				{ return flagBits;		}
	inline	double						GetFillThickness (void) const		{ return fillThick;		}
	inline	Int32						GetConnPriority (void) const		{ return connPriority;	}
	inline	SkinType					GetSkinType (void) const			{ return skinType;		}
	inline	Int32						GetHatchId (void) const				{ return hatchId; }

	inline	VBAttr::OverriddenExtendedPen	GetFillExtendedPen (void) const		{ return cutSurface.GetOverridenFgExtendedPen ();	}
	inline	VBAttr::OverriddenPen			GetFillBGPenIndex (void) const		{ return cutSurface.GetOverridenBkgPen ();	}

	inline	bool	IsCoreSkin		 (void) const		{ return (flagBits & SFCore) > 0; }
	inline	bool	IsHatchAngleSkin (void) const		{ return (flagBits & SFHatchAngle) > 0; }
	inline	bool	IsFinishSkin	 (void) const		{ return (flagBits & SFFinish) > 0; }
	inline	bool	IsHatchFitSkin	 (void) const		{ return (flagBits & SFHatchFit) > 0; }

	inline	bool	IsSkinNormal (void) const			{ return (skinType == CompInfo::Skin_Normal);	}
	inline	bool	IsSkinUpper (void) const			{ return (skinType == CompInfo::Skin_Upper);	}
	inline	bool	IsSkinDown (void) const				{ return (skinType == CompInfo::Skin_Down);	}
	inline	bool	IsSkinEmpty (void) const			{ return (skinType == CompInfo::Skin_Empty);	}

	inline	bool	HasProperty (short feature) const	{ return (flagBits & feature) > 0; }

	void			SetEndFacePenIndex (short newEndFacePenIndex);
	void			SetEndFaceLTIndex (const ADB::AttributeIndex& newEndFaceLTIndex);
	void			SetFillFgExtendedPenIndex (const VBAttr::ExtendedPen& newFillPenIndex);
	void			SetFillBGPenIndex (short newFillBGPenIndex);
	void			SetFlags (short newFlags);
	void			SetFillThickness (double newFillThickness);
	void			SetConnPriority (Int32 newConnPriority);
	void			SetSkinType (SkinType newSkinType);
	void			SetHatchId (const Int32& newHatchId);
	void			SetOpaqueCutFill ();
};


class VB_ELEM_OPERATIONS_DLL_EXPORT AttributeGetter {
protected:
	static const Int32 defaultConnPriority = Priority_Invalid;
public:
	virtual ~AttributeGetter ();

	virtual bool								IsTransparentCutFillBackground (const VBAttr::BuildingMaterialConstRef& buildMatRef) = 0;
	virtual bool								IsTransparentMaterial (const VBAttr::BuildingMaterialConstRef& buildMatRef) = 0;
	virtual bool								IsTransparentCutFill (const VBAttr::BuildingMaterialConstRef& buildMatRef) = 0;
	virtual VBAttr::BuildingMaterialConstRef	GetConstBuildingMaterial (const ADB::AttributeSetConstRef& attributeSet, const ADB::AttributeIndex& index) = 0;
	virtual Int32								GetBuildingMaterialConnectionPriority (const ADB::AttributeSetConstRef& attributeSet, const ADB::AttributeIndex& index, const Int32 defaultReturnVal = defaultConnPriority) = 0;
	virtual short								GetSkinAndBuildingMaterialFlags (const VBAttr::CompWallConstRef& compWall, short skinIndex) = 0;
	virtual VBAttr::CompWallConstRef			GetConstCompWall (const ADB::AttributeSetConstRef& attributeSet, const ADB::AttributeIndex& index) = 0;
	virtual short								GetBasicCutFillPen (const ADB::AttributeSetConstRef& attributeSet, const ADB::AttributeIndex& index) = 0;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT DefaultAttributeGetter: public AttributeGetter {
public:
	virtual bool								IsTransparentCutFillBackground (const VBAttr::BuildingMaterialConstRef& buildMatRef) override;
	virtual bool								IsTransparentMaterial (const VBAttr::BuildingMaterialConstRef& buildMatRef) override;
	virtual bool								IsTransparentCutFill (const VBAttr::BuildingMaterialConstRef& buildMatRef) override;
	virtual VBAttr::BuildingMaterialConstRef	GetConstBuildingMaterial (const ADB::AttributeSetConstRef& attributeSet, const ADB::AttributeIndex& index) override;
	virtual Int32								GetBuildingMaterialConnectionPriority (const ADB::AttributeSetConstRef& attributeSet, const ADB::AttributeIndex& index, const Int32 defaultReturnVal = defaultConnPriority) override;
	virtual short								GetSkinAndBuildingMaterialFlags (const VBAttr::CompWallConstRef& compWall, short skinIndex) override;
	virtual VBAttr::CompWallConstRef			GetConstCompWall (const ADB::AttributeSetConstRef& attributeSet, const ADB::AttributeIndex& index) override;
	virtual short								GetBasicCutFillPen (const ADB::AttributeSetConstRef& attributeSet, const ADB::AttributeIndex& index) override;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT CompositeInfo {
private:
	double					totalThick;				// thickness of the whole wall
	double					maxX;					// min X of the first skin
	short					nSkins;					// number of skins
	GS::Array<Skin>			skins;					// skin array
	GS::Array<SkinFaceLine>	skinFaceLines;			// skin face line array

public:
	CompositeInfo ();

	CompositeInfo (
		const VBAttr::CompWallConstRef& compWall,
		const VBAttr::OverriddenPen& fillPen,
		const VBAttr::OverriddenPen& fillBkgPen,
		AttributeGetter& attributeGetter);
	
	CompositeInfo (
		const VBElem::WallConstRef& pWall,
		const VBD::FloorPlanCutLevelsConstRef& floorCut,
		const VBElem::ElemDisplayOptions& displayOption,
		const VBElem::ElemViewDepthLimitations& viewDepth,
		const EffectiveComposite::Structure& structureDisplay,
		double relLevel,
		AttributeGetter& attributeGetter);

	CompositeInfo (const CompositeInfo&);

	inline	double				GetTotalThickness (void) const		{ return totalThick;	}
	inline	double				GetMaxX (void) const				{ return maxX;			}
	inline	short				GetSkinNumber (void) const			{ return nSkins;		}
	inline	short				GetSkinFaceLineNumber (void) const	{ return nSkins + 1;	}

	const Skin*					GetSkin (short skinNumber) const;
	const SkinFaceLine*			GetSkinFaceLine (short skinFaceLineNumber) const;

	Skin*						GetSkin (short skinNumber);
	SkinFaceLine*				GetSkinFaceLine (short skinFaceLineNumber);

	CompositeInfo&				operator = (const CompositeInfo&);
};

}		// namespace CompInfo

bool IsTransparentInCutPlane (VBAttr::BuildingMaterialConstRef & buildMatRef, const CutSurface & cutSurf, CompInfo::AttributeGetter& attributeGetter);

#endif		// COMPOSITEINFO_HPP
