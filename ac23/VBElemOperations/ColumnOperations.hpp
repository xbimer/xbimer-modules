#ifndef COLUMN_OPERATIONS_HPP
#define COLUMN_OPERATIONS_HPP

#include "VBElemOperationsExport.hpp"

// from VBElemOperations
#include "GDLGlobalCalculations.hpp"
#include "ProfileInterface.hpp"
#include "ProfileCoordinateSystem.hpp"
#include "VBElemOperations.hpp"
#include "VBElemSurfaceDescriptor.hpp"

// from VBAttributes
#include "BuildingMaterial.hpp"

// from VBElements
#include "Column.hpp"
#include "ColumnSegmentTypes.hpp"
#include "AssemblySegmentShapeTypes.hpp"

class VB_ELEM_OPERATIONS_DLL_EXPORT ColumnOperations {
public:
	enum MaterialSelector {
		ExtrusionMaterial,
		SideMaterial,
		EndMaterial,
		TopMaterial,
		BotMaterial,
		InsideMaterial,
		WrappingTopMaterial,
		WrappingBotMaterial,
		CropUpwardsMaterial,
		CropDownwardsMaterial,
		CutElemMaterial,
		OutsideMaterial // returns veneer material if has veneer, returns column material otherwise
	};

	static void		GetStandaloneVisibleMaterials (const VBElem::ColumnConstRef& column, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& materials);
	static void		GetSegmentStandaloneVisibleMaterials (const VBElem::ColumnSegmentConstRef& columnSegment, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& materials);

	static UInt32	GetComponentNumber (const VBElem::ColumnSegmentConstRef& column);

	static short	GetComponentFlags (const VBElem::ColumnSegmentConstRef& column, const StructureComponentId& structureCompomemtId);

	static void		SetHomeStory (VBElem::ColumnRef column, const short homeStoryNum);

	static Vector3D	GetColumnDirection (const VBElem::ColumnConstRef& column);

	static Vector3D GetSegmentVector (const VBElem::ColumnSegmentConstRef& segment);

	static Box3DType GetRawBounds3D (const VBElem::ColumnConstRef& column);
	static Box3DType GetRawBounds3D (const VBElem::ColumnSegmentConstRef& column);

	static ProfileCoordinateSystem	GetProfileCoordinateSystem (VBElem::ColumnSegmentConstRef segment, 
																UInt32 createFlags, 
																VBElemOperations::RefPointLocation location);

	static bool HasVisibleSkin (const VBElem::ColumnConstRef& elemRef, EffectiveComposite::Structure structure);
	static bool HasSegmentVisibleSkin (const VBElem::ColumnSegmentConstRef& elemRef, EffectiveComposite::Structure structure);

	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const VBElem::ColumnConstRef& column);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(const VBElem::ColumnRef& column);

	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const VBElem::ColumnDefault& columnDef);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(VBElem::ColumnDefault& columnDef);

	static bool	Calc_Corners (const VBElem::ColumnSegmentConstRef& segment, Point2D* core4, Point2D* ven4, const double* wrapWidth = nullptr);
	static bool	Calc_Corners (const VBElem::ColumnSegmentConstRef& segment, VBElem::ShapePosition shapePos, Point2D* core4, Point2D* ven4, const double* wrapWidth = nullptr);
	static void	CalcCrossCoords (const VBElem::ColumnSegmentConstRef& segment, GS::ArrayFB<Coord, 4>& begCoords, GS::ArrayFB<Coord, 4>& endCoords, double interp);

	static Vector	FloorPlan2DTransVectFromHeight (const VBElem::ColumnSegmentConstRef& segment, const double height);

	static VBElem::ColumnSegmentConstRef GetSegmentAtCutLevel (const VBElem::ColumnConstRef& column, double z);

	static bool IsSupportedInLegacyMode (const VBElem::ColumnConstRef& column);

	static VBElemSurfaceDescriptor		GetMaterial (const VBElem::ColumnSegmentConstRef& segment,
													const ADB::AttributeSetConstRef& attributeSet,
													MaterialSelector selector,
													bool pbcConversion,
													const ComponentIndex& componentIndex = ComponentIndex::EntireElement,
													const ProfileInterface* profile3DData = nullptr);

	static VBElemSurfaceDescriptor		GetMaterial (const VBElem::ColumnSegmentDefault& segmentDef,
													const ADB::AttributeSetConstRef& attributeSet,
													MaterialSelector selector,
													bool pbcConversion,
													const ComponentIndex& componentIndex = ComponentIndex::EntireElement,
													const ProfileInterface* profile3DData = nullptr);
};


#endif // COLUMN_OPERATIONS_HPP
