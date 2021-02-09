#ifndef GDL_DICT_FIELDS_A185_HPP
#define	GDL_DICT_FIELDS_A185_HPP

#pragma once

#include "GDLExport.h"
#include "GDLDictionary.hpp"

// TODO A-185 VS: Attenni a GDLDictFields.hpp-be, ha megerkezik A180-rol!
namespace GDLPropertyDictFields {

static const GDL::FieldName componentCollectMode ("collectMode");

static const GDL::FieldName	componentId ("componentId");
static const GDL::FieldName componentIdList ("componentIds");
static const GDL::FieldName componentId_uniqueId ("id");

static const GDL::FieldName	propertyIdList ("propertyIds");
static const GDL::FieldName	propertyId_uniqueId ("id");

static const GDL::FieldName	propertyValueList ("propertyValues");
static const GDL::FieldName propertyValue_Status ("value_status");
static const GDL::FieldName propertyValue_Type ("type");
static const GDL::FieldName propertyValue_ArrValue ("value");

}

#endif	//GDL_DICT_FIELDS_A185_HPP
