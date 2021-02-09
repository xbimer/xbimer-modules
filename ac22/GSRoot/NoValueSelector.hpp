// *********************************************************************************************************************
// Description:		Declaration of GS::NoValue, which is used by GS::Optional and GS::OptionalOnHeap.
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	BIM
//
// *********************************************************************************************************************


#ifndef NOVALUESELECTOR_HPP
#define NOVALUESELECTOR_HPP

#pragma once


namespace GS {


// === NoValue =========================================================================================================

class NoValueSelector {
public:
	constexpr NoValueSelector () {}
};


constexpr NoValueSelector	NoValue;


}	// namespace GS


#endif
