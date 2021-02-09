// *********************************************************************************************************************
// Description:		The class template GS::OptionalOnHeap manages an optional contained value, i.e. a value that may or
//					may not be present. The value is stored in heap memory, thus its type can be incomplete.
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	BIM
//
// *********************************************************************************************************************


#ifndef OPTIONALONHEAP_HPP
#define OPTIONALONHEAP_HPP

#pragma once


#include "Channel.hpp"
#include "GSDebug.hpp"
#include "NoValueSelector.hpp"
#include "TypeTraits.hpp"


namespace GS {


// === class OptionalOnHeap ============================================================================================

template <class Type>
class OptionalOnHeap final {
private:
	Type*		value;

	template <class T>	friend class OptionalOnHeap;

	template <class SourceType>
	using IsEnabledSourceType = EnableIf<!IsSame<Decay<SourceType>, OptionalOnHeap> && IsConvertible<SourceType, Type>>;

public:
	OptionalOnHeap ();
	OptionalOnHeap (NoValueSelector);
	OptionalOnHeap (const Type& value);
	OptionalOnHeap (Type&& value);

	template <class SourceType, class = IsEnabledSourceType<SourceType>>
	OptionalOnHeap (SourceType&& value);

	OptionalOnHeap (const OptionalOnHeap& source);
	OptionalOnHeap (OptionalOnHeap&& source);
	template <class SourceType>  OptionalOnHeap (const OptionalOnHeap<SourceType>& source);
	template <class SourceType>  OptionalOnHeap (OptionalOnHeap<SourceType>&& source);
	~OptionalOnHeap ();

	void			operator= (NoValueSelector);
	void			operator= (const Type& newValue);
	void			operator= (Type&& newValue);

	template <class SourceType, class = IsEnabledSourceType<SourceType>>
	void			operator= (SourceType&& newValue);

	void			operator= (const OptionalOnHeap& source);
	void			operator= (OptionalOnHeap&& source);
	template <class SourceType> void  operator= (const OptionalOnHeap<SourceType>& source);
	template <class SourceType> void  operator= (OptionalOnHeap<SourceType>&& source);

	template <class... Args>
	void			New (Args&&... args);

	void			Clear ();

	bool			HasValue () const;
	explicit operator bool () const;

	Type&			Get () &;
	const Type&		Get () const&;
	Type&&			Get () &&;
	const Type&		Get (const Type& defaultValue) const&;
	Type&&			Get (Type&& defaultValue) &&;

	Type&			operator* () &;
	const Type&		operator* () const&;
	Type&&			operator* () &&;

	Type*			operator-> ();
	const Type*		operator-> () const;

	GSErrCode		Read  (IChannel& ic);
	GSErrCode		Write (OChannel& oc) const;
};


// === OptionalOnHeap methods ==========================================================================================

template <class Type>
OptionalOnHeap<Type>::OptionalOnHeap () :
	value (nullptr)
{
}


template <class Type>
OptionalOnHeap<Type>::OptionalOnHeap (NoValueSelector) :
	value (nullptr)
{
}


template <class Type>
OptionalOnHeap<Type>::OptionalOnHeap (const Type& value) :
	value (new Type (value))
{
}


template <class Type>
OptionalOnHeap<Type>::OptionalOnHeap (Type&& value) :
	value (new Type (std::move (value)))
{
}


template <class Type>
template <class SourceType, class /*= IsEnabledSourceType<SourceType>*/>
OptionalOnHeap<Type>::OptionalOnHeap (SourceType&& value) :
	value (new Type (std::forward<SourceType> (value)))
{
}


template <class Type>
OptionalOnHeap<Type>::OptionalOnHeap (const OptionalOnHeap& source)
{
	if (source.value != nullptr)
		value = new Type (*source.value);
	else
		value = nullptr;
}


template <class Type>
OptionalOnHeap<Type>::OptionalOnHeap (OptionalOnHeap&& source)
{
	value = source.value;
	source.value = nullptr;
}


template <class Type>
template <class SourceType>
OptionalOnHeap<Type>::OptionalOnHeap (const OptionalOnHeap<SourceType>& source)
{
	static_assert (IsConvertible<SourceType, Type>, "'SourceType' is not convertible to 'Type'.");

	if (source.value != nullptr)
		value = new Type (*source.value);
	else
		value = nullptr;
}


template <class Type>
template <class SourceType>
OptionalOnHeap<Type>::OptionalOnHeap (OptionalOnHeap<SourceType>&& source)
{
	static_assert (IsConvertible<SourceType, Type>, "'SourceType' is not convertible to 'Type'.");

	if (source.value != nullptr) {
		value = new Type (std::move (*source.value));
		delete source.value;
		source.value = nullptr;
	} else {
		value = nullptr;
	}
}


template <class Type>
OptionalOnHeap<Type>::~OptionalOnHeap ()
{
	delete value;
}


template <class Type>
void	OptionalOnHeap<Type>::operator= (NoValueSelector)
{
	Clear ();
}


template <class Type>
void	OptionalOnHeap<Type>::operator= (const Type& newValue)
{
	if (value != nullptr)
		*value = newValue;
	else
		value = new Type (newValue);
}


template <class Type>
void	OptionalOnHeap<Type>::operator= (Type&& newValue)
{
	if (value != nullptr)
		*value = std::move (newValue);
	else
		value = new Type (std::move (newValue));
}


template <class Type>
template <class SourceType, class /*= IsEnabledSourceType<SourceType>*/>
void	OptionalOnHeap<Type>::operator= (SourceType&& newValue)
{
	if (value != nullptr)
		*value = std::forward<SourceType> (newValue);
	else
		value = new Type (std::forward<SourceType> (newValue));
}


template <class Type>
void	OptionalOnHeap<Type>::operator= (const OptionalOnHeap& source)
{
	if (value != nullptr && source.value != nullptr) {
		*value = *source.value;
	} else if (value != nullptr && source.value == nullptr) {
		delete value;
		value = nullptr;
	} else if (value == nullptr && source.value != nullptr) {
		value = new Type (*source.value);
	}
}


template <class Type>
void	OptionalOnHeap<Type>::operator= (OptionalOnHeap&& source)
{
	delete value;

	value = source.value;
	source.value = nullptr;
}


template <class Type>
template <class SourceType>
void	OptionalOnHeap<Type>::operator= (const OptionalOnHeap<SourceType>& source)
{
	static_assert (IsConvertible<SourceType, Type>, "'SourceType' is not convertible to 'Type'.");

	if (value != nullptr && source.value != nullptr) {
		*value = *source.value;
	} else if (value != nullptr && source.value == nullptr) {
		delete value;
		value = nullptr;
	} else if (value == nullptr && source.value != nullptr) {
		value = new Type (*source.value);
	}
}


template <class Type>
template <class SourceType>
void	OptionalOnHeap<Type>::operator= (OptionalOnHeap<SourceType>&& source)
{
	static_assert (IsConvertible<SourceType, Type>, "'SourceType' is not convertible to 'Type'.");

	if (value != nullptr && source.value != nullptr) {
		*value = std::move (*source.value);
		delete source.value;
		source.value = nullptr;
	} else if (value != nullptr && source.value == nullptr) {
		delete value;
		value = nullptr;
	} else if (value == nullptr && source.value != nullptr) {
		value = new Type (std::move (*source.value));
		delete source.value;
		source.value = nullptr;
	}
}


template <class Type>
template <class... Args>
void	OptionalOnHeap<Type>::New (Args&&... args)
{
	Clear ();

	value = new Type (std::forward<Args> (args)...);
}


template <class Type>
void	OptionalOnHeap<Type>::Clear ()
{
	delete value;
	value = nullptr;
}


template <class Type>
bool	OptionalOnHeap<Type>::HasValue () const
{
	return value != nullptr;
}


template <class Type>
OptionalOnHeap<Type>::operator bool () const
{
	return value != nullptr;
}


template <class Type>
Type&		OptionalOnHeap<Type>::Get () &
{
	DBASSERT (value != nullptr);
	return *value;
}


template <class Type>
const Type&		OptionalOnHeap<Type>::Get () const&
{
	DBASSERT (value != nullptr);
	return *value;
}


template <class Type>
Type&&		OptionalOnHeap<Type>::Get () &&
{
	DBASSERT (value != nullptr);
	return std::move (*value);
}


template <class Type>
const Type&		OptionalOnHeap<Type>::Get (const Type& defaultValue) const&
{
	if (value != nullptr)
		return *value;
	else
		return defaultValue;
}


template <class Type>
Type&&		OptionalOnHeap<Type>::Get (Type&& defaultValue) &&
{
	if (value != nullptr)
		return std::move (*value);
	else
		return std::move (defaultValue);
}


template <class Type>
Type&		OptionalOnHeap<Type>::operator* () &
{
	DBASSERT (value != nullptr);
	return *value;
}


template <class Type>
const Type&		OptionalOnHeap<Type>::operator* () const&
{
	DBASSERT (value != nullptr);
	return *value;
}


template <class Type>
Type&&		OptionalOnHeap<Type>::operator* () &&
{
	DBASSERT (value != nullptr);
	return std::move (*value);
}


template <class Type>
Type*		OptionalOnHeap<Type>::operator-> ()
{
	DBASSERT (value != nullptr);
	return value;
}


template <class Type>
const Type*		OptionalOnHeap<Type>::operator-> () const
{
	DBASSERT (value != nullptr);
	return value;
}


template <class Type>
GSErrCode	OptionalOnHeap<Type>::Read (IChannel& ic)
{
	GSErrCode err;

	bool readHasValue;
	err = ic.Read (readHasValue);
	if (err != NoError)
		return err;

	if (readHasValue) {
		if (!HasValue ())
			New ();

		err = GS::ReadType (ic, Get ());
		if (err != NoError)
			return err;
	} else {
		Clear ();
	}

	return NoError;
}


template <class Type>
GSErrCode	OptionalOnHeap<Type>::Write (OChannel& oc) const
{
	GSErrCode err;
	
	err = oc.Write (HasValue ());
	if (err != NoError)
		return err;

	if (HasValue ()) {
		err = GS::WriteType (oc, Get ());
		if (err != NoError)
			return err;
	}

	return NoError;
}


// === global functions ================================================================================================

template <class LeftType>
bool	operator== (const OptionalOnHeap<LeftType>& leftOp, NoValueSelector)
{
	return !leftOp.HasValue ();
}


template <class LeftType, class RightType>
bool	operator== (const OptionalOnHeap<LeftType>& leftOp, const RightType& rightOp)
{
	return leftOp.HasValue () && *leftOp == rightOp;
}


template <class LeftType, class RightType>
bool	operator== (const LeftType& leftOp, const OptionalOnHeap<RightType>& rightOp)
{
	return rightOp.HasValue () && leftOp == *rightOp;
}


template <class LeftType, class RightType>
bool	operator== (const OptionalOnHeap<LeftType>& leftOp, const OptionalOnHeap<RightType>& rightOp)
{
	return leftOp.HasValue () == rightOp.HasValue () && (!leftOp.HasValue () || *leftOp == *rightOp);
}


template <class LeftType>
bool	operator!= (const OptionalOnHeap<LeftType>& leftOp, NoValueSelector)
{
	return leftOp.HasValue ();
}


template <class LeftType, class RightType>
bool	operator!= (const OptionalOnHeap<LeftType>& leftOp, const RightType& rightOp)
{
	return !(leftOp == rightOp);
}


template <class LeftType, class RightType>
bool	operator!= (const LeftType& leftOp, const OptionalOnHeap<RightType>& rightOp)
{
	return !(leftOp == rightOp);
}


template <class LeftType, class RightType>
bool	operator!= (const OptionalOnHeap<LeftType>& leftOp, const OptionalOnHeap<RightType>& rightOp)
{
	return !(leftOp == rightOp);
}


}	// namespace GS


#endif
