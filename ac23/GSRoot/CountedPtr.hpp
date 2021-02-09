
// *****************************************************************************
//
// Declaration and implementation of CountedPtr class
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	SN
//
// *****************************************************************************

#ifndef GS_COUNTEDPTR_HPP
#define GS_COUNTEDPTR_HPP

#pragma once


// --- Includes ----------------------------------------------------------------

#include "CountedPtrImpl.hpp"



// --- CountedPtr class --------------------------------------------------------

namespace GS {

template<typename T, typename CounterT = Counter>
class CountedPtr : public CountedPtrImpl<T, CounterT, PtrBehaviour> {

// Construction / destruction:
public:
	using CountedPtrImpl<T, CounterT, PtrBehaviour>::CountedPtrImpl;
	CountedPtr () {} // clang 3.8 workaround

// Operator overloading:
public:
	CountedPtr&			operator = (Null*);
	template<typename U>
	CountedPtr&			operator = (U* instance);
	template<typename U, typename BehaviourU>
	CountedPtr&			operator = (const CountedPtrImpl<U, CounterT, BehaviourU>& rhs);
	template<typename U, typename BehaviourU>
	CountedPtr&			operator = (CountedPtrImpl<U, CounterT, BehaviourU>&& rhs);
};

////////////////////////////////////////////////////////////////////////////////
// CountedPtr implementation

////////////////////////////////////////////////////////////////////////////////
// Construction / destruction
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Operator overloading
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
inline CountedPtr<T, CounterT>&	CountedPtr<T, CounterT>::operator = (Null*)
{
	CountedPtrImpl<T, CounterT, PtrBehaviour>::operator = (nullptr);

	return *this;
}

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
template<typename U>
inline CountedPtr<T, CounterT>&	CountedPtr<T, CounterT>::operator = (U* instance)
{
	CountedPtrImpl<T, CounterT, PtrBehaviour>::operator = (instance);

	return *this;
}

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
template<typename U, typename BehaviourU>
inline CountedPtr<T, CounterT>&	CountedPtr<T, CounterT>::operator = (const CountedPtrImpl<U, CounterT, BehaviourU>& rhs)
{
	CountedPtrImpl<T, CounterT, PtrBehaviour>::operator = (rhs);

	return *this;
}

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
template<typename U, typename BehaviourU>
inline CountedPtr<T, CounterT>&	CountedPtr<T, CounterT>::operator = (CountedPtrImpl<U, CounterT, BehaviourU>&& rhs)
{
	CountedPtrImpl<T, CounterT, PtrBehaviour>::operator = (std::move (rhs));

	return *this;
}

}



// --- Global functions --------------------------------------------------------

namespace GS {

// -----------------------------------------------------------------------------
// StaticCast
// -----------------------------------------------------------------------------

template<class ToType, class FromType>
inline CountedPtr<ToType>	StaticCast (const CountedPtr<FromType>& from)
{
	return CountedPtr<ToType> (from, StaticCastSelector ());
}

// -----------------------------------------------------------------------------
// ConstCast
// -----------------------------------------------------------------------------

template<class ToType, class FromType>
inline CountedPtr<ToType>	ConstCast (const CountedPtr<FromType>& from)
{
	return CountedPtr<ToType> (from, ConstCastSelector ());
}

// -----------------------------------------------------------------------------
// DynamicCast
// -----------------------------------------------------------------------------

template<class ToType, class FromType>
inline CountedPtr<ToType>	DynamicCast (const CountedPtr<FromType>& from)
{
	return CountedPtr<ToType> (from, DynamicCastSelector ());
}

// -----------------------------------------------------------------------------
// GenerateHashValue
// -----------------------------------------------------------------------------

template<class Type, class CounterT>
ULong GenerateHashValue(CountedPtr<Type, CounterT> ptr)
{
	return GenerateHashValue (ptr.operator -> ());
}

}

#endif // GS_COUNTEDPTR_HPP
