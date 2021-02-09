// *****************************************************************************
//
//				Dimension Standards class declarations
//
// Module:			AC
// Namespace:		-
// Contact person:	JG, NYD
//
// SG compatible
//
// *****************************************************************************

#ifndef DIMESION_STANDARDS_TYPES_HPP
#define DIMESION_STANDARDS_TYPES_HPP

#include "ODBReference.hpp"
#include "ODBRefList.hpp"

namespace VBD
{

class DimensionStandards;

class DimensionStandardsObserver;

class DimensionStandardsSet;

class ApplyDimensionStandardsChangesCommand;

//--------------------------------------------------------------------

typedef ODB::Ref<DimensionStandards>			DimensionStandardsRef;
typedef ODB::ConstRef<DimensionStandards>		DimensionStandardsConstRef;
typedef ODB::RefList<DimensionStandards>		DimensionStandardsRefList;
typedef ODB::ConstRefList<DimensionStandards>	DimensionStandardsConstRefList;

//--------------------------------------------------------------------

typedef ODB::Ref<DimensionStandardsSet>			DimensionStandardsSetRef;
typedef ODB::ConstRef<DimensionStandardsSet>	DimensionStandardsSetConstRef;

//--------------------------------------------------------------------

const GS::Guid	DimensionStandardsSetGuid	("EAC3B35D-8132-4E06-8B57-B3E15D241C4F");

}


#endif
