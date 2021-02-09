// *********************************************************************************************************************
// Description:		http://wiki/w/CopyOnWrite
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	BIM
//
// *********************************************************************************************************************

#ifndef COPYONWRITE_HPP
#define COPYONWRITE_HPP

#pragma once


#include "AtomicCounter.hpp"
#include "Ref.hpp"
#include "TypeTraits.hpp"


namespace GS {


// === class CopyOnWrite ===============================================================================================

template <class Type, class CounterType = AtomicCounter>
class CopyOnWrite final {
private:
	Ref<Type, CounterType>	object;

	static_assert (IsDecayed<Type>, "'Type' should be decayed.");

	template <class... Args>
	using IsEnabledArgs = EnableIf<!(ParameterPackSize<Args...> == 1 && IsSame<Decay<ParameterPackType<0, Args...>>, CopyOnWrite>)>;

	template <class SourceType>
	using IsEnabledSourceType = EnableIf<!IsSame<Decay<SourceType>, CopyOnWrite>>;

public:
	CopyOnWrite ();

	template <class... Args, class = IsEnabledArgs<Args...>>
	CopyOnWrite (Args&&... args);

	template <class SourceType, class = IsEnabledSourceType<SourceType>>
	CopyOnWrite&	operator= (SourceType&& value);

	const Type&		operator* () const;
	const Type*		operator-> () const;
	Type*			operator() ();
};


// === CopyOnWrite methods =============================================================================================

template <class Type, class CounterType>
CopyOnWrite<Type, CounterType>::CopyOnWrite () :
	object (new Type ())
{
}


template <class Type, class CounterType>
template <class... Args, class /*= IsEnabledArgs<Args...>*/>
CopyOnWrite<Type, CounterType>::CopyOnWrite (Args&&... args) :
	object (new Type (std::forward<Args> (args)...))
{
}


template <class Type, class CounterType>
template <class SourceType, class /*= IsEnabledSourceType<SourceType>*/>
CopyOnWrite<Type, CounterType>&		CopyOnWrite<Type, CounterType>::operator= (SourceType&& value)
{
	object = new Type (std::forward<SourceType> (value));
	return *this;
}


template <class Type, class CounterType>
const Type&		CopyOnWrite<Type, CounterType>::operator* () const
{
	return *object;
}


template <class Type, class CounterType>
const Type*		CopyOnWrite<Type, CounterType>::operator-> () const
{
	return &*object;
}


template <class Type, class CounterType>
Type*	CopyOnWrite<Type, CounterType>::operator() ()
{
	if (object.GetCounter () > 1)
		object = new Type (*object);

	return &*object;
}


}	// namespace GS


#endif
