#ifndef ASSEMBLYSEGMENT_OPERATIONS_HPP
#define ASSEMBLYSEGMENT_OPERATIONS_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "ProfileTypes.hpp"
#include "AssemblySegmentTypes.hpp"
#include "AssemblySegmentShapeTypes.hpp"
#include "VBElemSurfaceDescriptor.hpp"
#include "Vector3D.hpp"
#include "ComponentIndex.hpp"
#include "Box2DData.h"
#include "VBElemOperationsTypes.hpp"
#include "EffectiveComposite.hpp"
#include "TypeSafeUnion.hpp"
#include "EllipseArc3D.hpp"
#include "ProfileGeometry3D.hpp"

#include "AssemblySegmentDefault.hpp"

namespace ADB {
class AttributeSet;
typedef ODB::ConstRef<AttributeSet> AttributeSetConstRef;
}

namespace VBElem {
class ConstParametricProfileWrapper;
}


class ProfileInterface;
class ComponentIndex;


class VB_ELEM_OPERATIONS_DLL_EXPORT AssemblySegmentOperations
{
public:
// TODO TBa
enum MaterialSelector {
	TopMaterial,
	RightMaterial,
	BotMaterial,
	LeftMaterial,
	SideMaterial,
	InsideMaterial,
	WrappingTopMaterial,
	WrappingBotMaterial,
	CropUpwardsMaterial,
	CropDownwardsMaterial,
	CutElemMaterial,
	OutsideMaterial // returns veneer material if has veneer, returns column material otherwise
};

static GSUtils::Variant<Sector3D, Geometry::EllipseArc3D> GetReferenceLineGeometry (const VBElem::AssemblySegmentConstRef& segment);

static GS::Optional<VBEO::ModelElemStructureRef> GetSegmentModelElemStructure (const VBElem::AssemblySegmentConstRef& segment);
static GS::Optional<VBEO::ModelElemStructureRef> GetSegmentModelElemStructure (const VBElem::AssemblySegmentDefault& segment);

static Geometry::Vector3D			GetSegmentVector (const VBElem::AssemblySegmentConstRef& segment);

static void							OverrideSegmentDefaultMaterials (const GS::Array<VBElem::AssemblySegmentDefault*>& segmentsToModify, const ADB::AttributeIndex& materialIndex);

static VBElem::ShapePosition		 CoordToSegmentShapeDimensionPosition (const VBElem::AssemblySegmentConstRef&, const Coord3D&);

static VBElem::ConstParametricProfileWrapper	GetParametricProfile (const VBElem::AssemblySegmentConstRef& segment, const VBElem::ShapePosition position);
static ProfileVectorImage			CaptureProfile (const VBElem::AssemblySegmentConstRef& segment, const VBElem::ShapePosition position);

static Geometry::Vector2d			GetAnchorOffset (const VBElem::AssemblySegmentConstRef& assemblySegment, const VBElem::ShapePosition shapePos);

static Geometry::Vector2d			GetAnchorOffset (const VBElem::AssemblySegmentConstRef& assemblySegment, const double refLinePointParam /*[0-BegC, 1-EndC]*/);

static double	CalculateActualCutAngle (const VBElem::AssemblySegmentConstRef& segment, const VBElem::ShapePosition position);
static double	CalculateActualCutAngle (const VBElem::AssemblySegmentDefault& segment, const VBElem::ShapePosition position);

static Box3D								GetBoundBoxOfCorners (const VBElem::AssemblySegmentConstRef& segment);
static Box3D								CornersToBox3D (const VBEO::ProfileGeometry3D::Corners& corners);
static VBEO::ProfileGeometry3D::Corners		GetBeginCorners (const VBElem::AssemblySegmentConstRef& segment);
static VBEO::ProfileGeometry3D::Corners		GetEndCorners (const VBElem::AssemblySegmentConstRef& segment);

};

#endif // ASSEMBLYSEGMENT_OPERATIONS_HPP