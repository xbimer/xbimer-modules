// *****************************************************************************
// Fill attribute
// VBAttributes, platform-independent
//
// Namespaces:        Contact person:
//	Fill					CSF
//	FillField				CSF
//	FillFieldMask			CSF
//	FillObserver			CSF
//	FillChanges				CSF
//	FillSet					CSF
//	FillSetObserver			CSF
//
// [SG compatible]
// *****************************************************************************

#if !defined (FILL_HPP)
#define FILL_HPP

#pragma once

//============================== Includes =================================

#include	"VBAttributesEnv.hpp"
#include	"generalattribute.hpp"
#include	"GeneralAttributeObserver.hpp"
#include	"basicgeometry.h"
#include	"VectorFillData.hpp"
#include	"SymbolFillData.hpp"
#include	"ImageFillData.hpp"
#include	"AttributeTypeSet.hpp"
#include	"GDLParamInterface.hpp"

// Pattern
#include	"GSPix.h"
#include	"FillTypeDefinitions.hpp"
#include	"PatternTypeDefinitions.hpp"

//===================== Begin of the VBAttr namespace =====================

namespace VBAttr {

//========================== Class Fill  =============================

class FillObserver;
class FillSet;
class FillConversionData;

class VBATTRIBUTES_DLL_EXPORT Fill : public ADB::GeneralAttribute {
	DECLARE_DYNAMIC_CLASS_INFO (Fill)

public:
		// constants

	DECLARE_FIELDMASK_FUNCTION (Fill)

public:
	static const Field Pattern;
	static const Field XSpacingFactor;
	static const Field YSpacingFactor;
	static const Field Angle;
	static const Field FirstOffset;
	static const Field SecondOffset;
	static const Field Type;
	static const Field VectorFillMemo;
	static const Field SymbolFillMemo;
	static const Field ImageFillMemo;
	static const Field Percent;
	static const Field TextureName;
	static const Field TextureStatus;
	static const Field TextureWidth;
	static const Field TextureHeight;
	static const Field TextureRotationAngle;
	static const Field AdditionalParameters;

#define _FILL_DATA_MEMBERS_																										\
private:																														\
		/* data members */																										\
																																\
	GSPattern		bitPat;			/* pattern of the fill */																	\
	double			hXSpac;			/* spacing factor of the fill in the X direction */											\
	double			hYSpac;			/* spacing factor of the fill in the Y direction */											\
	double			hAngle;			/* angle of the fill */																		\
	Geometry::Vector2d			c1;				/* symbol fill offset */														\
	Geometry::Vector2d			c2;				/* symbol fill offset */														\
	GX::Pattern::FillTypes		fillType;		/* type of the fill	(GX::Pattern::VectorFill etc.) */							\
	mutable short	chSolidFillSize;/* size in pixels, when it can be substituted with solid fill */							\
	Int32			dummy2;			/* not used */																				\
	double			percent;		/* percent for background and foreground */													\
																																\
	GS::UniString	textureName;		/* name of the texture */																\
	short			textureStatus;		/* texture status TEXMIRRORY | TEXMIRRORX \Sources\GDL\GDLDLL\iHeaders\image.h */		\
	double			textureXSize;		/* x size of the picture in model space */												\
	double			textureYSize;		/* y size of the picture in model space */												\
	double			textureRotAng;		/* rotation angle */																	\
																																\
	VectorFillData	vectorFill;		/* vector fill memo id (used only if fillType == GX::Pattern::VectorFill) */				\
	SymbolFillData	symbolFill;		/* symbol fill memo id (used only if fillType == GX::Pattern::SymbolFill) */				\
	ImageFillData	imageFill;		/* image fill memo id (used only if fillType == GX::Pattern::ImageFill) */					\
	GDL::Parameters	additionalParameters;	/* additional parameters from GDL */


	_FILL_DATA_MEMBERS_

// Transaction management class

private:
	class VBATTRIBUTES_DLL_EXPORT IsolatedState : public ADB::GeneralAttribute::IsolatedState {
		private:
			friend class Fill;

			_FILL_DATA_MEMBERS_

		public:
			virtual ~IsolatedState ();
	};

public:
		// constructors

									Fill ();
									Fill (const Fill& source);
								   ~Fill ();

		// operator overloading

	Fill&							operator= (const Fill& source);

		// get methods

	GSType							GetTypeId (void) const  override { return TypeId_Fill; }

	inline	void					GetPattern (GSPattern* pattern) const { if (pattern != nullptr) BNCopyMemory (pattern, &bitPat, sizeof (GSPattern)); }
	inline	double					GetXSpacingFactor (void) const		{ return hXSpac; }
	inline	double					GetYSpacingFactor (void) const		{ return hYSpac; }
	inline	double					GetAngle (void) const				{ return hAngle; }
	inline	const Geometry::Vector2d&			GetFirstOffset (void) const			{ return c1; }
	inline	const Geometry::Vector2d&			GetSecondOffset (void) const		{ return c2; }
	inline	const double			GetPercent (void) const				{ return percent; }
	inline	void					GetTextureName (GS::UniString* imageFillTextureName) const { *imageFillTextureName = textureName; }
	inline	short					GetTextureStatus (void) const		{ return textureStatus; }
	inline	double					GetTextureWidth (void) const		{ return textureXSize; }
	inline	double					GetTextureHeight (void) const		{ return textureYSize; }
	inline	double					GetTextureRotationAngle (void) const { return textureRotAng; }

	inline	GX::Pattern::FillTypes				GetType (void) const				{ return fillType; }
	inline	bool					IsVectorFill (void) const			{ return (fillType == GX::Pattern::VectorFill); }
	inline	bool					IsSymbolFill (void) const			{ return (fillType == GX::Pattern::SymbolFill); }
	inline	bool					IsImageFill (void) const			{ return (fillType == GX::Pattern::ImageFill); }
	inline	bool					IsSolidFill (void) const			{ return (fillType == GX::Pattern::SolidFill); }
	inline	bool					IsSolidFgFill (void) const			{ return (fillType == GX::Pattern::SolidFill && fabs (percent - 1.0) < Eps); }
	inline	bool					IsSolidBgFill (void) const			{ return (fillType == GX::Pattern::SolidFill && fabs (percent) < Eps); }
	inline	bool					IsGradientFill (void) const			{ return IsLinearGradientFill () || IsRadialGradientFill (); };
	inline	bool					IsLinearGradientFill (void) const	{ return (fillType == GX::Pattern::LinearGradientFill); }
	inline	bool					IsRadialGradientFill (void) const	{ return (fillType == GX::Pattern::RadialGradientFill); }
	bool							IsPercentFill (double *percentValue = nullptr) const;

	inline	const VectorFillData&	GetVectorFillData (void) const		{ return vectorFill; }
	GSErrCode						GetVectorFillDetails (GX::Pattern::VectorFillDetails* fillDetails) const;

	inline	const SymbolFillData&	GetSymbolFillData (void) const		{ return symbolFill; }
	GSErrCode						GetSymbolFillDetails (GX::Pattern::SymbolFillDetails* fillDetails) const;

	inline	const ImageFillData&	GetImageFillData (void) const		{ return imageFill; }
	GSErrCode						GetImageFillDetails (ImageFillDetails* fillDetails);

	inline	bool					IsScaledFill (void) const			{ return (GetFlags () & ScaledFill) > 0; }
	inline	bool					IsSectionFill (void) const			{ return (GetFlags () & SectionFill) > 0; }
	inline	bool					IsDrawingFill (void) const			{ return (GetFlags () & DrawingFill) > 0; }
	inline	bool					IsSurfaceFill (void) const			{ return (GetFlags () & SurfaceFill) > 0; }

	inline short					GetChangeToSolidFillSize (void) const { return chSolidFillSize; }
	inline	const GDL::Parameters&	GetAdditionalParameters (void) const { return additionalParameters; }


	FieldMaskRef					Compare (ADB::GeneralAttributeConstRef attribute) const override;

	Fill*							Clone (void) const override;

		// set methods

	void							SetPattern (const GSPattern& newPattern);
	void							SetXSpacingFactor (double newXSpacingFactor);
	void							SetYSpacingFactor (double newYSpacingFactor);
	void							SetAngle (double newAngle);
	void							SetFirstOffset (const Geometry::Vector2d& newFirstOffset);
	void							SetSecondOffset (const Geometry::Vector2d& newSecondOffset);
	void							SetPercent (double newPercent);
	void							SetType (GX::Pattern::FillTypes newType);
	GSErrCode						SetVectorFillDetails (const GX::Pattern::VectorFillDetails& newFillDetails);
	GSErrCode						SetSymbolFillDetails (const GX::Pattern::SymbolFillDetails& newFillDetails);
	GSErrCode						SetSymbolFillMotifs (const GX::Pattern::SFMotifs& newFillMotifs);
	GSErrCode						SetImageFillDetails (const ImageFillDetails& newFillDetails);

	void							SetTextureName (const GS::UniString& newTextureName);
	void							SetTextureStatus (short newTextureStatus);
	void							SetTextureWidth (double newTextureWidth);
	void							SetTextureHeight (double newTextureHeight);
	void							SetTextureRotationAngle (double newTextureRotationAngle);

	inline void						SetChangeToSolidFillSize (short size) const { chSolidFillSize = size; }
	void							SetAdditionalParameters (const GDL::Parameters& newAdditionalParameters);

	void							Synchronize (ADB::GeneralAttributeConstRef newAttribute, const FieldMask& changes) override;

		// observer methods

	void							Attach (FillObserver& observer);
	void							Detach (FillObserver& observer);

	void							Notify (GS::EventObserver* observer, const FieldMask* changes) const override;

		// memo methods

	bool							HasMemoData () const override;
	bool							HasSameMemoData (ADB::GeneralAttributeConstRef attribute) const override;

	void							GenerateBitmapPattern ();

		// used libpart methods
	virtual void					CollectUsedLibPartNames (ADB::IUsedLibPartCollection& outputInterface) const override;
		// read / write methods

	GSErrCode						Read (GS::IChannel& ic) override;
	GSErrCode						Write (GS::OChannel& oc) const override;
	GSErrCode						ReadXML (GS::XMLIChannel& ic) override;
	GSErrCode						WriteXML (GS::XMLOChannel& oc) const override;

private:
	bool							IsOldTypePercentFill (double *percentValue = nullptr ) const;
	bool							GenerateOldPercentFill (double percentValue, GSPattern& bitPat) const;
	void							CorrectSolidFill ();

	GSErrCode						ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadVersion2 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteVersion2 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadVersion3 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteVersion3 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadVersion4 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteVersion4 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadVersion5 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteVersion5 (GS::OChannel& oc, const GS::OutputFrame& frame) const;

	GSErrCode						ReadVersion7 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteVersion7 (GS::OChannel& oc, const GS::OutputFrame& frame) const;

	GSErrCode						WriteCoreIsolatedStateVersion6 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadCoreIsolatedStateVersion6 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteCoreIsolatedStateVersion7 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadCoreIsolatedStateVersion7 (GS::IChannel& ic, const GS::InputFrame& frame);

	GSErrCode						WriteIsolatedStateVersion6 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadIsolatedStateVersion6 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteIsolatedStateVersion7 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadIsolatedStateVersion7 (GS::IChannel& ic, const GS::InputFrame& frame);

	GSErrCode						WriteTWIsolatedStateVersion6 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadTWIsolatedStateVersionUpTo6 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode						WriteTWIsolatedStateVersion7 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode						ReadTWIsolatedStateVersion7 (GS::IChannel& ic, const GS::InputFrame& frame);


		// convert methods

	short							ConvertFillTypesToShort (GX::Pattern::FillTypes fillTypes) const;
	GX::Pattern::FillTypes			ConvertShortToFillTypes (short shortFillType) const;

public:
		// copy methods

	void							Copy (ADB::GeneralAttributeConstRef newAttribute) override;

		// memo methods
	GSErrCode						Destruct (void) override;

	static bool						EqualPatterns (const GSPattern &p1, const GSPattern &p2);	// for IsPercentFill

		// Transaction management methods

	virtual	GSErrCode					StoreIsolatedState   			(ODB::Object::IsolatedState* isolatedState) const override;
	virtual	GSErrCode					RestoreIsolatedState 			(const ODB::Object::IsolatedState* isolatedState) override;
	virtual	ODB::Object::IsolatedState*	CreateIsolatedState				(void) const override;
	virtual USize						GetIsolatedSize					(void) const override;
	virtual	GSErrCode					WriteIsolatedState   			(GS::OChannel& oc) const override;
	virtual	GSErrCode					ReadIsolatedState 				(GS::IChannel& ic) override;

		// TWObject methods

	GSErrCode						ReadTWIsolatedState  (GS::IChannel& ic) override;
	GSErrCode						WriteTWIsolatedState (GS::OChannel& oc) const override;
};

//--------------------------------------------------------------------------

typedef ODB::Ref<Fill>			FillRef;
typedef ODB::ConstRef<Fill>		FillConstRef;

//--------------------------------------------------------------------------

typedef FieldMaskT<VBAttr::Fill>		FillFieldMask;

//============================ Class FillObserver ==========================
//
// Observer class for Fill
//
//==========================================================================

class VBATTRIBUTES_DLL_EXPORT FillObserver : public ADB::GeneralAttributeObserver {
protected:
		// destructor

	virtual ~FillObserver () {}

public:
		// changed methods

	virtual void	PatternChanged (FillConstRef);
	virtual void	XSpacingFactorChanged (FillConstRef);
	virtual void	YSpacingFactorChanged (FillConstRef);
	virtual void	AngleChanged (FillConstRef);
	virtual void	FirstOffsetChanged (FillConstRef);
	virtual void	SecondOffsetChanged (FillConstRef);
	virtual void	PercentChanged (FillConstRef);
	virtual void	TypeChanged (FillConstRef);
	virtual void	VectorFillChanged (FillConstRef);
	virtual void	SymbolFillChanged (FillConstRef);
	virtual void	ImageFillChanged (FillConstRef);

	virtual void	TextureNameChanged (FillConstRef);
	virtual void	TextureStatusChanged (FillConstRef);
	virtual void	TextureWidthChanged (FillConstRef);
	virtual void	TextureHeightChanged (FillConstRef);
	virtual void	TextureRotationAngleChanged (FillConstRef);
	virtual void	AdditionalParametersChanged (FillConstRef);
};

//============================ Class FillChanges ===========================
//
// An implemented observer class for Fill
//
//==========================================================================

class VBATTRIBUTES_DLL_EXPORT FillChanges : public FillObserver {
private:
		// data members

	FillFieldMask	changes;

public:
		// constructors

	FillChanges (Fill* fill);
   ~FillChanges ();

		// set methods

	void	ResetChanges (void);

		// get methods

	const FillFieldMask&	GetChanges (void) const;

		// changed methods (ADB::GeneralAttributeObserver interface)

	void	NameChanged (ADB::GeneralAttributeConstRef) override;
	void	IndexChanged (ADB::GeneralAttributeConstRef) override;
	void	FlagsChanged (ADB::GeneralAttributeConstRef) override;
	void	ModificationTimeChanged (ADB::GeneralAttributeConstRef) override;
	void	UserDataChanged (GSType owner1, GSType owner2) override;

		// changed methods (FillObserver interface)

	void	PatternChanged (FillConstRef) override;
	void	XSpacingFactorChanged (FillConstRef) override;
	void	YSpacingFactorChanged (FillConstRef) override;
	void	AngleChanged (FillConstRef) override;
	void	FirstOffsetChanged (FillConstRef) override;
	void	SecondOffsetChanged (FillConstRef) override;
	void	PercentChanged (FillConstRef) override;
	void	TypeChanged (FillConstRef) override;
	void	VectorFillChanged (FillConstRef) override;
	void	SymbolFillChanged (FillConstRef) override;
	void	ImageFillChanged (FillConstRef) override;

	void	TextureNameChanged (FillConstRef) override;
	void	TextureStatusChanged (FillConstRef) override;
	void	TextureWidthChanged (FillConstRef) override;
	void	TextureHeightChanged (FillConstRef) override;
	void	TextureRotationAngleChanged (FillConstRef) override;
	void	AdditionalParametersChanged (FillConstRef) override;
};

//======================= Class FillConversionData =========================
//
// Conversion class for reading old version of Fill
//
//==========================================================================

class VBATTRIBUTES_DLL_EXPORT FillConversionData : public ODB::ConversionData {
	DECLARE_DYNAMIC_CLASS_INFO (FillConversionData);

	double	thermalConductivity;
	double	density;
	double	heatCapacity;

public:
	FillConversionData ();
	virtual ~FillConversionData ();

	FillConversionData (const FillConversionData& source);

	virtual GS::Object* Clone (void) const override;

	virtual GSErrCode Write (GS::OChannel& oc) const override;
	virtual GSErrCode Read (GS::IChannel& ic) override;

	const double	GetThermalConductivity (void) const;
	const double	GetDensity (void) const;
	const double	GetHeatCapacity (void) const;

	void	SetThermalConductivity (double value);
	void	SetDensity (double value);
	void	SetHeatCapacity (double value);
};


//====================== End of the VBAttr namespace =======================

}


//================== External functions for class Fill  ====================

VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	SearchEmptyHatchFailureProof (ADB::AttributeSetConstRef actualAttributeSet);
VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	SearchEmptyHatch (const ADB::AttributeSetConstRef actualAttributeSet);
VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	SearchEmptyHatchExhaustive (ADB::AttributeSetConstRef currentAttributeSet);
VBATTRIBUTES_DLL_EXPORT	const ADB::AttributeIndex	CreateEmptyHatch (ADB::AttributeSetRef currentAttributeSet);
VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	SearchSolidHatchFailureProof (ADB::AttributeSetConstRef actualAttributeSet);
VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	SearchSolidHatchExhaustive (ADB::AttributeSetConstRef currentAttributeSet);
VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	CreateSolidHatch (ADB::AttributeSetRef currentAttributeSet);
VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	SearchPredefinedPercentFillFailureProof (ADB::AttributeSetConstRef currentAttributeSet, ADB::PercentFillTypes percent);
VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	SearchPredefinedPercentFill (ADB::AttributeSetConstRef currentAttributeSet, ADB::PercentFillTypes percent);
VBATTRIBUTES_DLL_EXPORT void						CreatePredefinedPercentFill (ADB::AttributeSetRef currentAttributeSet, ADB::PercentFillTypes percent);
VBATTRIBUTES_DLL_EXPORT const ADB::AttributeIndex	SearchPercentFill (ADB::AttributeSetConstRef currentAttributeSet, double percent);
VBATTRIBUTES_DLL_EXPORT void						CreateGradientHatchIfNecessary (ADB::AttributeSetRef actualAttributeSet);
VBATTRIBUTES_DLL_EXPORT void						CreateDefaultFillsIfNeeded (ADB::AttributeSetRef attributeSet);

VBATTRIBUTES_DLL_EXPORT GSErrCode					AttributeSortByFillType (ADB::AttributeSetConstRef	attributeSet,
																			ADB::IDAttribute			attributeTypeId,
																			GS::Array <ADB::AttributeIndex>& indexes);

//-----------------------------------------------------------------------------

namespace VBAttr {
	VBATTRIBUTES_DLL_EXPORT FillRef					GetFill (ADB::AttributeSetConstRef attributeSet, const ADB::AttributeIndex& index);
	VBATTRIBUTES_DLL_EXPORT FillConstRef			GetConstFill (ADB::AttributeSetConstRef attributeSet, const ADB::AttributeIndex& index);
	VBATTRIBUTES_DLL_EXPORT GS::UniString			GetFillName (ADB::AttributeSetConstRef attributeSet, const ADB::AttributeIndex& index);
	VBATTRIBUTES_DLL_EXPORT bool					IsValidFillIndex (const ADB::AttributeIndex& index);
	VBATTRIBUTES_DLL_EXPORT bool					IsValidAndExistFillIndex (ADB::AttributeSetConstRef attributeSet, const ADB::AttributeIndex& index);
	VBATTRIBUTES_DLL_EXPORT void					ConvertFillAttrToFillData (VBAttr::FillConstRef fillAttrRef, GX::Pattern::FillData& fillData);
	VBATTRIBUTES_DLL_EXPORT void					ConvertFillDataToFillAttr (const GX::Pattern::FillData& fillData, VBAttr::Fill& fillAttrRef);
}


#endif

