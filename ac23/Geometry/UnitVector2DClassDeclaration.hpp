#ifndef _UNITVECTOR_2D_CLASS_DECLARATION_HPP
#define _UNITVECTOR_2D_CLASS_DECLARATION_HPP
#pragma once

// === Includes ========================================================================================================

#include "Vector2D.hpp"
#include "UnitVectorCreationError.hpp"

// =====================================================================================================================

namespace Geometry {

template <class CoordinateType>
class CustomUnitVector2D
{
public:
	static const CustomUnitVector2D<CoordinateType> XAxis;
	static const CustomUnitVector2D<CoordinateType> YAxis;

	static CustomUnitVector2D Create (CoordinateType x, CoordinateType y, double nullVectorLengthLimit = SmallEps)	CAN_THROW (UnitVectorCreationError);
	static CustomUnitVector2D Create (const Vector2<CoordinateType>& v, double nullVectorLengthLimit = SmallEps)	CAN_THROW (UnitVectorCreationError);

	inline Vector2<CoordinateType> ToVector2D () const;
	inline operator Vector2<CoordinateType> () const;

	bool IsParallelWith		(const CustomUnitVector2D& other, double eps = Eps) const;
	bool IsEqualWith		(const CustomUnitVector2D& other, double eps = Eps) const;
	bool IsPerpendicularTo	(const CustomUnitVector2D& other, double eps = Eps) const;

	CustomUnitVector2D operator- () const;

private:
	template<class T>
	friend GS::Optional<CustomUnitVector2D<T>> ConvertToUnitVector (const Vector2<T>&, double);

	CustomUnitVector2D () = delete;
	CustomUnitVector2D (CoordinateType x, CoordinateType y);

	template<class T>
	using ValidCoordinateType = EnableIf<GS::IsSame<T, float> || GS::IsSame<T, double>, T>;

	ValidCoordinateType<CoordinateType> x, y;
};


template<class CoordinateType> GS::Optional<CustomUnitVector2D<CoordinateType>> ConvertToUnitVector (const Vector2<CoordinateType>& v, double nullVectorLengthLimit = SmallEps);
template<class CoordinateType> CustomUnitVector2D<CoordinateType> ConvertToUnitVector (const Vector2<CoordinateType>& v, const CustomUnitVector2D<CoordinateType>& resultIfInputIsNull);	// to keep the functionality of CustomUnitVector2D function

template<class CoordinateType> Vector2<CoordinateType>	operator* (double d, const CustomUnitVector2D<CoordinateType>& u);
template<class CoordinateType> Vector2<CoordinateType>	operator* (const CustomUnitVector2D<CoordinateType>& u, double d);
template<class CoordinateType> Vector2<CoordinateType>	operator+ (const CustomUnitVector2D<CoordinateType>& u1, const CustomUnitVector2D<CoordinateType>& u2);
template<class CoordinateType> Vector2<CoordinateType>	operator- (const CustomUnitVector2D<CoordinateType>& u1, const CustomUnitVector2D<CoordinateType>& u2);
template<class CoordinateType> CoordinateType			operator* (const CustomUnitVector2D<CoordinateType>& u1, const CustomUnitVector2D<CoordinateType>& u2);


typedef CustomUnitVector2D<double> UnitVector_2D;
typedef CustomUnitVector2D<float> F_UnitVector_2D;

}

using Geometry::UnitVector_2D;

#endif

