#ifndef GDL_SEGMENTEDASSEMBLYCONVERSIONS_HPP
#define	GDL_SEGMENTEDASSEMBLYCONVERSIONS_HPP

#pragma once

#include "GDLExport.h"
#include "GDLDictionary.hpp"
#include "GDLDefs.h"

#include "VBElements/AssemblySegmentShapeTypes.hpp"
#include "VBElements/AssemblySegmentTypes.hpp"
#include "VBElements/SegmentedAssemblyTypes.hpp"

namespace VBElemGDLOperations {

class GDLAssemblySegmentCrossSectionType {
public:
	static const Int32 RECTANGULAR_TYPE = 1;
	static const Int32 ROUNDED_TYPE = 2;
	static const Int32 COMPLEX_TYPE = 3;

	static	GDL::Dictionary		FromInternalToGDL (const VBElem::AssemblySegmentConstRef& segment);
};

class GDLAssemblySegmentCurvatureType {
public:
	static const Int32 STRAIGHT_TYPE = 0;
	static const Int32 HORIZONTALLYCURVED_TYPE = 1;
	static const Int32 VERTICALLYCURVED_TYPE = 2;

	static	bool					IsValidGDLId (Int32 gdlId);
};

class GDLAssemblySegmentGeometryType {
public:
	static const Int32 HOMOGENEOUS_TYPE = 0;
	static const Int32 TAPERED_TYPE		= 1;

	static	bool				IsValidGDLId (Int32 gdlId);
};


enum SegmentInfoDataContainment {
	IncludeCrossSection,
	IncludeCurvature,
	IncludeGeometryType,
	IncludeReferenceLineLength
};

GDL::Dictionary		CreateSegmentsInfoDictionary (const VBElem::SegmentedAssemblyConstRef& segmentedAssembly, const GS::Array<SegmentInfoDataContainment>& infoContainment);

}

#endif	//GDL_PROPERTYDICTCONVERSIONS_HPP
