// *********************************************************************************************************************
// Description:		Extensions for VRDData class to create specific View Related Derived Data 
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************

#ifndef	VRDDataCREATIONEXTENSIONS_HPP
#define	VRDDataCREATIONEXTENSIONS_HPP

#pragma once

#include "WallVRDData.hpp"
#include "Wall.hpp"
#include "Extension.hpp"

namespace VBD {


class VB_ELEM_OPERATIONS_DLL_EXPORT VRDDataCreationExtension : public GS::Extension
{
public:
	static GS::ExtensionId id;

	VRDDataCreationExtension (GS::ClassInfo* target) : GS::Extension (&id, target) {}
	virtual ~VRDDataCreationExtension ();

	virtual VRDData*  Create (VBElem::ModelElementConstRef) = 0;
};


}		//namespace VBD

#endif