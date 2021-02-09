// *********************************************************************************************************************
// Description:		WallConnectionData for wPoly
//
// Module:			VBElemOperations
// Namespace:		VBD
// Contact person:	LB
//
// SG compatible
// *********************************************************************************************************************

#ifndef	WALLVRDDATAADDITIONALPARAMETERS_HPP
#define	WALLVRDDATAADDITIONALPARAMETERS_HPP

#pragma once

#include "WallConnectionList.hpp"
#include "ConnectionData.hpp"


namespace VBD {

class VB_ELEM_OPERATIONS_DLL_EXPORT WallConnectionData : public VBD::ConnectionData
{
private:
	VBElem::WallConnectionLists lists;

public:
	WallConnectionData (const VBElem::WallConnectionLists& source);
	
	const VBElem::WallConnectionLists& GetWallConnectionList () const { return lists; }

	virtual ~WallConnectionData ();
};		//WallConnectionData

}		//namespace VBD


#endif