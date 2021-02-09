
// *****************************************************************************
//
// Declaration and implementation of Ref class
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	SN
//
// *****************************************************************************

#ifndef GS_REF_HPP
#define GS_REF_HPP

#pragma once


// --- Includes ----------------------------------------------------------------

#include "CountedPtrImpl.hpp"



// --- Ref class ---------------------------------------------------------------

namespace GS {

template<typename T, typename CounterT = Counter>
class Ref : public CountedPtrImpl<T, CounterT, RefBehaviour> {

// Construction / destruction:
public:
	using CountedPtrImpl<T, CounterT, RefBehaviour>::CountedPtrImpl;
	Ref () {} // clang 3.8 workaround

// Operator overloading:
public:
	Ref&			operator =  (Null*);
	Ref&			operator =  (const Null&);
	template<typename U>
	Ref&			operator =  (U* instance);
	template<typename U, typename BehaviourU>
	Ref&			operator =  (const CountedPtrImpl<U, CounterT, BehaviourU>& rhs);
	template<typename U, typename BehaviourU>
	Ref&			operator =  (CountedPtrImpl<U, CounterT, BehaviourU>&& rhs);

	bool			operator == (const Null&) const;
	bool			operator != (const Null&) const;

};

////////////////////////////////////////////////////////////////////////////////
// Ref implementation

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
inline Ref<T, CounterT>&	Ref<T, CounterT>::operator = (Null*)
{
	CountedPtrImpl<T, CounterT, RefBehaviour>::operator = (nullptr);

	return *this;
}

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
inline Ref<T, CounterT>&	Ref<T, CounterT>::operator = (const Null&)
{
	CountedPtrImpl<T, CounterT, RefBehaviour>::operator = (nullptr);

	return *this;
}

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
template<typename U>
inline Ref<T, CounterT>&	Ref<T, CounterT>::operator = (U* instance)
{
	CountedPtrImpl<T, CounterT, RefBehaviour>::operator = (instance);

	return *this;
}

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
template<typename U, typename BehaviourU>
inline Ref<T, CounterT>&	Ref<T, CounterT>::operator = (const CountedPtrImpl<U, CounterT, BehaviourU>& rhs)
{
	CountedPtrImpl<T, CounterT, RefBehaviour>::operator = (rhs);

	return *this;
}

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
template<typename U, typename BehaviourU>
inline Ref<T, CounterT>&	Ref<T, CounterT>::operator = (CountedPtrImpl<U, CounterT, BehaviourU>&& rhs)
{
	CountedPtrImpl<T, CounterT, RefBehaviour>::operator = (std::move (rhs));

	return *this;
}

// -----------------------------------------------------------------------------
// operator ==
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
inline bool		Ref<T, CounterT>::operator == (const Null&) const
{
	return CountedPtrImpl<T, CounterT, RefBehaviour>::operator == (nullptr);
}

// -----------------------------------------------------------------------------
// operator !=
// -----------------------------------------------------------------------------

template<typename T, typename CounterT>
inline bool		Ref<T, CounterT>::operator != (const Null&) const
{
	return CountedPtrImpl<T, CounterT, RefBehaviour>::operator != (nullptr);
}


}


// --- Global functions --------------------------------------------------------

namespace GS {

// -----------------------------------------------------------------------------
// StaticCast
// -----------------------------------------------------------------------------

template<class ToType, class FromType, class CounterT>
inline Ref<ToType, CounterT>	StaticCast (const Ref<FromType, CounterT>& from)
{
	return Ref<ToType, CounterT> (from, StaticCastSelector ());
}

// -----------------------------------------------------------------------------
// ConstCast
// -----------------------------------------------------------------------------

template<class ToType, class FromType, class CounterT>
inline Ref<ToType, CounterT>	ConstCast (const Ref<FromType, CounterT>& from)
{
	return Ref<ToType, CounterT> (from, ConstCastSelector ());
}

// -----------------------------------------------------------------------------
// DynamicCast
// -----------------------------------------------------------------------------

template<class ToType, class FromType, class CounterT>
inline Ref<ToType, CounterT>	DynamicCast (const Ref<FromType, CounterT>& from)
{
	return Ref<ToType, CounterT> (from, DynamicCastSelector ());
}


// -----------------------------------------------------------------------------
// IsType
// -----------------------------------------------------------------------------

template<class T1, class T2, class CounterT>
inline bool	IsType (const Ref<T2, CounterT>& ref)
{
	return (ref != nullptr) && (dynamic_cast<T1*> (ref.operator-> ()) != nullptr);
}


// -----------------------------------------------------------------------------
// NewRef
// -----------------------------------------------------------------------------

template<class T, class CounterT = Counter, typename... Args>
inline Ref<T, CounterT>	NewRef (Args&&... args)
{
	return Ref<T, CounterT> (new T (std::forward<Args> (args)...));
}


}

#endif // GS_REF_HPP
