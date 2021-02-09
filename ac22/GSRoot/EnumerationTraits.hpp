// *********************************************************************************************************************
// Description:		A traits class, that can be specialized to tell whether a class is enumerable (i.e. a container)
//					or not. Enables different function overloads for enumerable and not enumerable types.
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB, KKr
//
// *********************************************************************************************************************

#ifndef ENUMERATIONTRAITS_HPP
#define ENUMERATIONTRAITS_HPP

#pragma once


namespace GS {

namespace Imp {

template <class T>
constexpr bool	IsEnumerableExternal (decltype (bool { begin (std::declval<T> ()) != end (std::declval<T> ()) })*) { return true; }

template <class T>
constexpr bool	IsEnumerableExternal (...) { return false; }

template <class T>
constexpr bool	IsEnumerableInternal (decltype (bool { std::declval<T> ().begin () != std::declval<T> ().end () })*) { return true; }

template <class T>
constexpr bool	IsEnumerableInternal (...) { return false; }

template <class T>
constexpr bool	IsEnumerable = IsEnumerableExternal<T> (nullptr) || IsEnumerableInternal<T> (nullptr);		// this intermediate definition is needed due to a Developer Studio internal compiler error

template <class T, size_t N>
constexpr bool IsEnumerable<T[N]> = true;

} // namespace Imp

template <class T>
constexpr bool	IsEnumerable = Imp::IsEnumerable<T>;

template <class T>
using Enumerable = typename Enable<IsEnumerable<T>>::TrueCondition;			// for convenient compile time function selection (like enable_if)

template <class T>
using NotEnumerable = typename Enable<IsEnumerable<T>>::FalseCondition;		// for convenient compile time function selection (like enable_if)

} // namespace GS


#endif
