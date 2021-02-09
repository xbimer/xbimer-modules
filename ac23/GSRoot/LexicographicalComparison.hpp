// *********************************************************************************************************************
// Description:		Lexicographical comparison support.
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	BIM
//
// *********************************************************************************************************************


#ifndef LEXICOGRAPHICALCOMPARISON_HPP
#define LEXICOGRAPHICALCOMPARISON_HPP

#pragma once


namespace GS {


// === Implementation details for LexicographicalIsLess () =============================================================

namespace ImpLex {


template <class Type, class MemberVariableType>
bool	IsEqual (const Type& leftOp, const Type& rightOp, MemberVariableType Type::*memberPointer)
{
	return leftOp.*memberPointer == rightOp.*memberPointer;
}


template <class Type, class MemberFunctionReturnType>
bool	IsEqual (const Type& leftOp, const Type& rightOp, MemberFunctionReturnType (Type::*memberPointer) () const)
{
	return (leftOp.*memberPointer) () == (rightOp.*memberPointer) ();
}


template <class Type, class MemberVariableType>
bool	IsLess (const Type& leftOp, const Type& rightOp, MemberVariableType Type::*memberPointer)
{
	return leftOp.*memberPointer < rightOp.*memberPointer;
}


template <class Type, class MemberFunctionReturnType>
bool	IsLess (const Type& leftOp, const Type& rightOp, MemberFunctionReturnType (Type::*memberPointer) () const)
{
	return (leftOp.*memberPointer) () < (rightOp.*memberPointer) ();
}


}	// namespace ImpLex


// === LexicographicalIsLess () ========================================================================================

template <class Type, class MemberPointer>
bool	LexicographicalIsLess (const Type& leftOp, const Type& rightOp, MemberPointer memberPointer)
{
	return ImpLex::IsLess (leftOp, rightOp, memberPointer);
}


template <class Type, class MemberPointer, class... MemberPointers>
bool	LexicographicalIsLess (const Type& leftOp, const Type& rightOp, MemberPointer memberPointer, MemberPointers... memberPointers)
{
	if (!ImpLex::IsEqual (leftOp, rightOp, memberPointer))
		return ImpLex::IsLess (leftOp, rightOp, memberPointer);

	return LexicographicalIsLess (leftOp, rightOp, memberPointers...);
}


}	// namespace GS


#endif
