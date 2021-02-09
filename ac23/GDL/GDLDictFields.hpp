#ifndef GDL_DICT_FIELDS_HPP
#define	GDL_DICT_FIELDS_HPP

#pragma once

#include "GDLExport.h"
#include "GDLDictionary.hpp"

namespace GDLDictFields {

static const GDL::FieldName userData ("userData");

}

namespace GDLCoord2DFields {

static const GDL::FieldName x ("x");
static const GDL::DictionaryPath xPath (x);

static const GDL::FieldName y ("y");
static const GDL::DictionaryPath yPath (y);

}

namespace GDLPolyDictFields {

static const GDL::FieldName contour ("contour");
static const GDL::DictionaryPath contourPath (contour);

static const GDL::FieldName isClosed ("isClosed");
static const GDL::DictionaryPath isClosedPath (isClosed);

static const GDL::FieldName edges ("edges");
static const GDL::DictionaryPath edgesPath (edges);

static const GDL::FieldName type ("type");
static const GDL::DictionaryPath typePath (type);

static const GDL::FieldName begPoint ("begPoint");
static const GDL::DictionaryPath begPointPath (begPoint);

static const GDL::FieldName arcAngle ("arcAngle");
static const GDL::DictionaryPath arcAnglePath (arcAngle);

static const GDL::FieldName holes ("holes");
static const GDL::DictionaryPath holesPath (holes);

static const GDL::FieldName useEdgeInfo ("useEdgeInfo");
static const GDL::DictionaryPath useEdgeInfoPath (useEdgeInfo);

static const GDL::FieldName defaultInhEdgeInfo ("defaultInhEdgeInfo");
static const GDL::DictionaryPath defaultInhEdgeInfoPath (defaultInhEdgeInfo);

static const GDL::FieldName edgeInfo ("edgeInfo");
static const GDL::DictionaryPath edgeInfoPath (edgeInfo);

}

namespace GDLDictFields {

static const GDL::FieldName toHomeStory ("toHomeStory");
static const GDL::DictionaryPath toHomeStoryPath (toHomeStory);

static const GDL::FieldName toProjectZero ("toProjectZero");
static const GDL::DictionaryPath toProjectZeroPath (toProjectZero);

static const GDL::FieldName toWallBottom ("toWallBottom");
static const GDL::DictionaryPath toWallBottomPath (toWallBottom);

static const GDL::FieldName toWallTop ("toWallTop");
static const GDL::DictionaryPath toWallTopPath (toWallTop);

static const GDL::FieldName breakMarks ("breakMarks");
static const GDL::DictionaryPath breakMarksPath (breakMarks);

static const GDL::FieldName isVisible ("isVisible");
static const GDL::DictionaryPath isVisiblePath (isVisible);

}

namespace GDLSegmentedAssemblyDictFields {

static const GDL::FieldName	segmentList ("segments");
static const GDL::DictionaryPath segmentListPath (segmentList);

static const GDL::FieldName segment_Curvature ("curvature");
static const GDL::DictionaryPath segment_CurvaturePath (segment_Curvature);

static const GDL::FieldName segment_Tapered ("tapered");
static const GDL::DictionaryPath segment_TaperedPath (segment_Tapered);

static const GDL::FieldName segment_CrossSection ("crossSection");
static const GDL::DictionaryPath segment_CrossSectionPath (segment_CrossSection);

static const GDL::FieldName segment_CrossSectionType ("type");
static const GDL::DictionaryPath segment_CrossSectionTypePath ({segment_CrossSection, segment_CrossSectionType});

static const GDL::FieldName segment_CrossSectionStartWidth ("startWidth");
static const GDL::DictionaryPath segment_CrossSectionStartWidthPath ({segment_CrossSection, segment_CrossSectionStartWidth});

static const GDL::FieldName segment_CrossSectionEndWidth ("endWidth");
static const GDL::DictionaryPath segment_CrossSectionEndWidthPath ({segment_CrossSection, segment_CrossSectionEndWidth});

static const GDL::FieldName segment_CrossSectionStartHeight ("startHeight");
static const GDL::DictionaryPath segment_CrossSectionStartHeightPath ({segment_CrossSection, segment_CrossSectionStartHeight});

static const GDL::FieldName segment_CrossSectionEndHeight ("endHeight");
static const GDL::DictionaryPath segment_CrossSectionEndHeightPath ({segment_CrossSection, segment_CrossSectionEndHeight});

static const GDL::FieldName segment_RefLineLength ("refLineLength");
static const GDL::DictionaryPath segment_RefLineLengthPath (segment_RefLineLength);
}

namespace GDLGeometryDictFields {

static const GDL::FieldName referenceLine2D ("referenceLine2D");
static const GDL::DictionaryPath referenceLine2DPath (referenceLine2D);

static const GDL::FieldName polygon2D ("polygon2D");
static const GDL::DictionaryPath polygon2DPath (polygon2D);

static const GDL::FieldName boundingBox2D ("boundingBox2D");
static const GDL::DictionaryPath boundingBox2DPath (boundingBox2D);

static const GDL::FieldName polyLine2D ("polyLine2D");
static const GDL::DictionaryPath polyLine2DPath (polyLine2D);

}

namespace GDLBoundingBoxFields
{

static const GDL::FieldName xmin ("xmin");
static const GDL::DictionaryPath xminPath (xmin);

static const GDL::FieldName ymin ("ymin");
static const GDL::DictionaryPath yminPath (ymin);

static const GDL::FieldName xmax ("xmax");
static const GDL::DictionaryPath xmaxPath (xmax);

static const GDL::FieldName ymax ("ymax");
static const GDL::DictionaryPath ymaxPath (ymax);

}


#endif	//GDL_DICT_FIELDS_HPP