
// *****************************************************************************
//
//                 Interfaces Enumerator, ConversionEnumerator
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB
//
// SG compatible
//
// *****************************************************************************


#if !defined ENUMERATOR_HPP
#define ENUMERATOR_HPP

#pragma once

// --------------------------------- Includes ----------------------------------

#include "Definitions.hpp"

#include <functional>


// =========================== Interface Enumerator ============================

//lint -save -e1553 [struct declared as extern "C" contains C++ substructure] valamiert hibat ad ra itt

namespace GS {

template <class Type>
class Enumerator {
protected:
	Enumerator () {}
	Enumerator (const Enumerator&) {}
	Enumerator&	operator= (const Enumerator&) { return *this; }

public:
	virtual void	Enumerate (const std::function<void (Type&)>& processor) = 0;
};


template <class Type>
class ConstEnumerator {
protected:
	ConstEnumerator () {}
	ConstEnumerator (const ConstEnumerator&) {}
	ConstEnumerator&	operator= (const ConstEnumerator&) { return *this; }

public:
	virtual void	Enumerate (const std::function<void (const Type&)>& processor) const = 0;
};

}	// namespace GS


// ====================== Interface ConversionEnumerator =======================

namespace GS {

template <class Type> class ConversionFilterEnumerator;

template <class Type>
class ConversionEnumerator: public ConstEnumerator<Type> {
public:
	ConversionFilterEnumerator<Type>	Filter (const std::function<bool (const Type&)>& passCondition) const;
};


template <class Type>
class ConversionFilterEnumerator: public ConversionEnumerator<Type> {
private:
	const ConversionEnumerator<Type>&		 source;
	const std::function<bool (const Type&)>& passCondition;

public:
	ConversionFilterEnumerator (const ConversionEnumerator<Type>& source, const std::function<bool (const Type&)>& passCondition);

	virtual void	Enumerate (const std::function<void (const Type&)>& processor) const override;
};


template <class Type>
ConversionFilterEnumerator<Type>::ConversionFilterEnumerator (const ConversionEnumerator<Type>& source, const std::function<bool (const Type&)>& passCondition):
	source (source),
	passCondition (passCondition)
{
}


template <class Type>
void	ConversionFilterEnumerator<Type>::Enumerate (const std::function<void (const Type&)>& processor) const
{
	source.Enumerate ([&] (const Type& item) {
		if (passCondition (item))
			processor (item);
	});
}


template <class Type>
ConversionFilterEnumerator<Type>	ConversionEnumerator<Type>::Filter (const std::function<bool (const Type&)>& passCondition) const
{
	return ConversionFilterEnumerator<Type> (*this, passCondition); 
}

}	// namespace GS


//lint -restore


#endif
