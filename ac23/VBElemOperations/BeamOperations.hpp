#ifndef BEAM_OPERATIONS__HPP
#define BEAM_OPERATIONS__HPP

// from VBElemOperations
#include "VBElemOperationsExport.hpp"
#include "VBElemOperations.hpp"
#include "ProfileInterface.hpp"
#include "VBElemSurfaceDescriptor.hpp"

// from VBElements
#include "AssemblySegmentTypes.hpp"
#include "AssemblySegmentShapeTypes.hpp"
#include "BeamTypes.hpp"
#include "BeamSegmentTypes.hpp"
#include "CrossSectType.hpp"
#include "GeneralElem.hpp"

// from VBAttributes
#include "BuildingMaterial.hpp"

// from VectorImage
#include "StructureComponentId.hpp"

// =====================================================================================================================
//  BeamOperations
// =====================================================================================================================

class VB_ELEM_OPERATIONS_DLL_EXPORT BeamOperations
{
public:
	enum MaterialSelector {
		LeftMaterial,
		TopMaterial,
		RightMaterial,
		BotMaterial,
		EndMaterial,
		SideMaterial,
		CutElemMaterial,
		HoleMaterial,
		CropMaterial,
		ExtrusionMaterial
	};

	enum SubGroupType {
		HoleGroupType = 0,
		SegmentGroupType,
		NumberOfGroupTypes
	};

	static VBElemSurfaceDescriptor		GetMaterial (const VBElem::BeamSegmentConstRef& segment,
													 const ADB::AttributeSetConstRef& attributeSet,
													 MaterialSelector selector,
													 bool pbcConversion,
													 const ComponentIndex& componentIndex = ComponentIndex::EntireElement,
													 const ProfileInterface* profile3DData = nullptr);

	static VBElemSurfaceDescriptor		GetMaterial (const VBElem::BeamSegmentDefault& segmentDef,
													 const ADB::AttributeSetConstRef& attributeSet,
													 MaterialSelector selector,
													 bool pbcConversion,
													 const ComponentIndex& componentIndex = ComponentIndex::EntireElement,
													 const ProfileInterface* profile3DData = nullptr);

	static VBElem::BeamSegmentConstRef	GetSegmentOfHole (const VBElem::BeamConstRef& beam, const VBElem::BeamHole& hole, double* positionOnSegment = nullptr);
	static UInt32						GetHoleNumOfSegment (const VBElem::BeamSegmentConstRef& beamSegment);

	static bool						GetWidthAtHole 		 (const VBElem::BeamConstRef& beam, const VBElem::BeamHole& hole, double& width);
	static bool						IsHoleOnBeam (const VBElem::BeamConstRef& beam, const VBElem::BeamHole& hole);
	static void						DragHoles (VBElem::BeamRef beam, double x, double z);
	
	static void						SetSlantedHeight (const VBElem::BeamRef& beam, double z, VBElemOperations::SetHeightLocation location);

	static void						SetBeginSlantAngle (const VBElem::BeamRef& beam, double angle);

	static void						SetBeginSlantAngle (const VBElem::BeamRef& beam, double angle, double originalEndAltitude);

	static bool SetVerticalCurveDataWithBeginSlantAngle (const VBElem::BeamRef& beam, const Coord& newBegC, const Coord& newEndC, double newCurveAngle, double newBeginSlantAngle, double originalEndAltitude);

	static UInt32					GetComponentNumber (const VBElem::BeamSegmentConstRef& beam);

	static short					GetComponentFlags (const VBElem::BeamSegmentConstRef& beam, const StructureComponentId& structureComponentId);

	static void						GetStandaloneVisibleMaterials (const VBElem::BeamConstRef& beam, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& materials);
	static void						GetStandaloneVisibleMaterials (const VBElem::BeamSegmentConstRef& beamSegment, bool pbcConversion, GS::HashSet<ADB::AttributeIndex>& materials);

	static UnitVector_3D			GetDirection3D (const VBElem::BeamConstRef beam, VBElemOperations::RefPointLocation location);

	static GS::Optional<GenArc>		GetReferenceLineArc (const VBElem::BeamConstRef& beam);
	static GS::Optional<GenArc>		GetReferenceLineArc (const VBElem::BeamSegmentConstRef& segment);

	static GenArc					GetSlantedCurve (const VBElem::BeamConstRef& beam);
	static GenArc					GetSlantedCurve (const VBElem::BeamSegmentConstRef& segment);
	
	static GenArc					GetVerticalSideViewCurveAtOrigin (const VBElem::AssemblySegmentConstRef& segment);

	static TRANMAT					GetBeamHoleTransform3D (const VBElem::BeamConstRef& beam, const VBElem::BeamHole& hole);
	static TRANMAT					GetBeamHoleTransform2D (const VBElem::BeamConstRef& beam, const VBElem::BeamHole& hole);

	static bool HasInhomogenousSegment (const VBElem::BeamConstRef& elemRef);

	static bool HasVisibleSkin				  (const VBElem::BeamConstRef& beam, EffectiveComposite::Structure structure);
	static bool HasSegmentVisibleSkin				  (const VBElem::BeamSegmentConstRef& elemRef, EffectiveComposite::Structure structure);

	static Box3DType GetRawBounds3D (const VBElem::BeamConstRef& beam);
	static Box3DType GetRawBounds3D (const VBElem::BeamSegmentConstRef& beam);

	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const VBElem::BeamConstRef& beam);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(const VBElem::BeamRef& beam);

	static GS::Optional<VBEO::FloorPlanAppearanceRef>			GetFloorPlanAppearance			(const VBElem::BeamDefault& beamDef);
	static GS::Optional<VBEO::FloorPlanAppearanceModifierRef>	GetFloorPlanAppearanceModifier	(VBElem::BeamDefault& beamDef);

	static double	GetRefLineLevelAtDistance (const VBElem::BeamSegmentConstRef& segment, const double distanceFromBegC, const double radiusModifier = 0.0);
	static Geometry::Plane	GetVerticalBeamPlane (const VBElem::BeamConstRef& beam);
	static Geometry::Plane	GetVerticalBeamSegmentPlane (const VBElem::BeamSegmentConstRef& segment);

	static double	GetDirectionAngle (const VBElem::BeamConstRef& beam);

	static Box2DData GetBeamSegmentMaxCrossectionBounds (const VBElem::BeamSegmentConstRef& beamSegment, EffectiveComposite::Structure structureDisplay);
	static Box2DData GetBeamMaxCrossectionBounds (const VBElem::BeamConstRef& beam, EffectiveComposite::Structure structureDisplay);

	static Int32 CalculateSubGroupId (const Int32 subElemId, const SubGroupType groupTypeId);
	static bool  IsSubGroupType (const Int32 subGroupId, const SubGroupType groupTypeId);
	static UInt32 GetSubElemIdFromSubGroupId (const Int32 subGroupId);

	static bool IsSupportedInLegacyMode (const VBElem::BeamConstRef& beam);

	class VB_ELEM_OPERATIONS_DLL_EXPORT BaseTransformOffsetGuard {
		double					formerLength;
		const Point2D			formerBegC;
		const VBElem::BeamRef&	beam;

	public:
		BaseTransformOffsetGuard (const VBElem::BeamRef& inBeam);
		void Commit ();
	};

	class VB_ELEM_OPERATIONS_DLL_EXPORT BaseOffset_KeepProportion_Scope {
	private:
		enum State { Started, Finished, Aborted };

		const VBElem::BeamRef&	beam;
		const double			formerLength;
		const double			formerBaseOffset;
		State					state;

	public:
		BaseOffset_KeepProportion_Scope (const VBElem::BeamRef& inBeam);
		~BaseOffset_KeepProportion_Scope ();
		
		void Finish ();
		void Abort ();
	};

	static GS::FixArray<Coord, 4> GetCrossSectCorners (VBElem::BeamSegmentConstRef beamSegment,
														EffectiveComposite::Structure structureDisplay,
														VBElem::ShapePosition shapePos);

	static Box2DData CalcAndGetCrossSectBoundingBox		(VBElem::BeamSegmentConstRef beamSegment,
														EffectiveComposite::Structure structureDisplay,
														VBElem::ShapePosition shapePos);

	static double	CalcAndGetRefLineOffsetFromMidline (VBElem::BeamSegmentConstRef beamSegment,
														EffectiveComposite::Structure structureDisplay,
														VBElem::ShapePosition shapePos,
														double& width);

	static void		CalcPoints							(VBElem::BeamSegmentConstRef	segmentRef, 
														 Coord*							corners, 
														 EffectiveComposite::Structure	structureDisplay, 
														 const bool*					custNegArc = nullptr);
};


#endif // BEAM_OPERATIONS__HPP 