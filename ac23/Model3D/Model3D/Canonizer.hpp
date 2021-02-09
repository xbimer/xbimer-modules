// *********************************************************************************************************************
// Description:		Elem and Body canonizer
//
// Module:			Modeler
// Namespace:		Modeler
// Contact person:	PP
//
// SG compatible
// *********************************************************************************************************************

#if !defined (GS_MODELER_BODYCANONIZER_HPP)
#define GS_MODELER_BODYCANONIZER_HPP

#pragma once


// from Model3D
#include	"Model3D/Model3DPrivExp.h"
#include	"Model3DFwd.hpp"
#include	"SharedPtr.hpp"
#include	"Model3D/DataCmpOption.hpp"
#include	"MeshBodyUtilTemplates.hpp"

namespace Modeler {

class Elem;
typedef GS::ConstSharedPtr<Elem> ConstElemPtr;


enum class ReindexOption {
	SetAttributes,
	IgnoreAttributes
};


MODEL3D_PRIVATE_EXPORT
ConstElemPtr MOD3D_PRIV CanonizeElem (const ConstElemPtr  elem,
									  const DataCmpOption cmpOption,
									  const ReindexOption reindexOpt);

MODEL3D_PRIVATE_EXPORT
ConstModel3DPtr	MOD3D_PRIV CanonizeModel (const ConstModel3DPtr model);


}	// namespace Modeler


#endif // GS_MODELER_BODYCANONIZER_HPP
