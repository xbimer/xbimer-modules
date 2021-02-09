// *********************************************************************************************************************
// Description:		Coord, Vector template based definitions
//
// Module:			Geometry
// Namespace:		Geometry
// Contact person:	FGY
//
// *********************************************************************************************************************

#if !defined (GS_VECTOR2D_HPP)
#define GS_VECTOR2D_HPP

#pragma once

#include <cstddef>

// from GSRoot
#include "GSDebug.hpp"
#include "Channel.hpp"
#include "XMLChannel.hpp"
#include "ObjectState.hpp"

// from Geometry
#include "Real.hpp"
#include "Length.hpp"


struct Vector2dData { // use for unions
	double x;
	double y;
};

namespace Geometry {

template<typename Coordinate>
class Vector2 {
public:
	Coordinate	x;
	Coordinate	y;

public:
	Vector2 (void);
	Vector2 (const Vector2& src);
	template<typename Vector2LikeType> explicit Vector2 (const Vector2LikeType& src) : x(src.x), y(src.y) {}
	Vector2 (const Coordinate& x, const Coordinate& y);
	explicit Vector2 (const Vector2dData& src);
	operator Vector2dData () const;

	~Vector2 ();

	Vector2& operator= (const Vector2& src);
	template<typename Number2> Vector2& operator= (const Vector2<Number2>& src) { x = src.x; y = src.y; return *this; }

	void GetCoords (Coordinate* x, Coordinate* y) const;
	void Set (Coordinate x, Coordinate y);

	Length GetLength (void) const;
	Coordinate GetLengthSqr (void) const;
	double CalcAngleToReference (const Vector2 <Coordinate> & refVector) const;		//returns angle from 0 to 2PI
	Vector2 GetUnitVector (void) const;
	bool Normalize (void);
	Vector2 Rotate (const Coordinate& si, const Coordinate& co) const;
	Vector2 GetNormalVector (void) const;

	Vector2		operator- () const;
	Vector2		operator+ (const Vector2&) const;
	Vector2		operator- (const Vector2&) const;

	Coordinate		operator* (const Vector2&) const;			// dot product (skalar szorzas)
	Coordinate		operator^ (const Vector2&) const;			// cross product (vektorialis szorzas), !! has low precedence !!

	Vector2		operator* (const Coordinate&) const;
	Vector2		operator/ (const Coordinate&) const;

	const Vector2&	operator+= (const Vector2&);
	const Vector2&	operator-= (const Vector2&);
	const Vector2&	operator*= (const Coordinate&);
	const Vector2&	operator/= (const Coordinate&);

	bool	operator== (const Vector2&) const;
	bool	operator!= (const Vector2&) const;

	GSErrCode Read	(GS::IChannel& ic);
	GSErrCode Write (GS::OChannel& oc) const;

	GSErrCode ReadXML	(GS::XMLIChannel& ic, const char* tagName = nullptr);
	GSErrCode WriteXML	(GS::XMLOChannel& oc, const char* tagName = nullptr) const;

	GS_FORCE_INLINE Coordinate& operator [] (UInt32 i)
	{
		DBASSERT (i < 3);
#if (defined (_MSC_VER) || defined (_LIBCPP_VERSION)) && !(defined(lint) || defined(_lint)) 
		static_assert (offsetof (Vector2<Coordinate>, y) - offsetof (Vector2<Coordinate>, x) == sizeof (Coordinate), "offset");
#endif
		return (&x)[i];
	}

	GS_FORCE_INLINE const Coordinate& operator [] (UInt32 i) const
	{
		DBASSERT (i < 3);
#if (defined (_MSC_VER) || defined (_LIBCPP_VERSION)) && !(defined(lint) || defined(_lint)) 
		static_assert (offsetof (Vector2<Coordinate>, y) - offsetof (Vector2<Coordinate>, x) == sizeof (Coordinate), "offset");
#endif
		return (&x)[i];
	}

};

template<typename Coordinate>
Vector2<Coordinate> operator* (const Coordinate&, const Vector2<Coordinate>&);

template<typename Coordinate>
Coordinate     GetDeterminant (const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2); // vectorial product

template<typename Coordinate>
bool		   IsColinear (const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2); // same or opposite direction

typedef Vector2<double>		Vector2d;
typedef Vector2<float>		Vector2f;

template<typename Coordinate>
bool	IsNear			(const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2);

template<typename Coordinate>
bool	IsNearZero		(const Vector2<Coordinate>& r);

template<typename Coordinate>
bool	IsNear			(const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2, double givenEps);

template<typename Coordinate>
bool	IsNearZero		(const Vector2<Coordinate>& v, double givenEps);


template<typename Coordinate>
GSErrCode Read (GS::IChannel& ic, Vector2<Coordinate>& v);

template<typename Coordinate>
GSErrCode Write (GS::OChannel& oc, const Vector2<Coordinate>& v);

template<typename Coordinate>
GSErrCode Vector2<Coordinate>::ReadXML	(GS::XMLIChannel& ic, const char* tagName)
{
	GSErrCode errCode = NoError;
	GS::XMLITag tag (ic, (tagName == nullptr ? "Vertex" : tagName));

	if (errCode == NoError)
		errCode = ic.ReadXML ("X", x);
	if (errCode == NoError)
		errCode = ic.ReadXML ("Y", y);

	return errCode;
}

template<typename Coordinate>
GSErrCode Vector2<Coordinate>::WriteXML	(GS::XMLOChannel& oc, const char* tagName) const
{
	GSErrCode errCode = NoError;
	GS::XMLTag tag (oc, (tagName == nullptr ? "Vertex" : tagName));
	
	if (errCode == NoError)
		errCode = oc.WriteXML ("X", x);
	if (errCode == NoError)
		errCode = oc.WriteXML ("Y", y);

	return errCode;
}

template<typename Coordinate>
inline Vector2<Coordinate>::Vector2 (void)
: x(), y()
{}


template<typename Coordinate>
inline Vector2<Coordinate>::Vector2 (const Vector2<Coordinate>& src)
: x(src.x), y(src.y)
{}


template<typename Coordinate>
inline Vector2<Coordinate>::Vector2 (const Coordinate& x, const Coordinate& y)
: x(x), y(y)
{}


template<typename Coordinate>
inline Vector2<Coordinate>::Vector2 (const Vector2dData& src)
: x(src.x), y(src.y)
{}


template<typename Coordinate>
inline Vector2<Coordinate>::operator Vector2dData () const
{
	Vector2dData v;
	v.x = x;
	v.y = y;

	return v;
}


template<typename Coordinate>
inline Vector2<Coordinate>::~Vector2 ()
{
#if defined (DEBUVERS)
	x = static_cast<Coordinate> (0.0);
	y = static_cast<Coordinate> (0.0);
#endif
}


template<typename Coordinate>
inline Vector2<Coordinate>& Vector2<Coordinate>::operator= (const Vector2<Coordinate>& src)
{
	x = src.x;
	y = src.y;
	return *this;
}


template<typename Coordinate>
inline void Vector2<Coordinate>::GetCoords (Coordinate* x, Coordinate* y) const
{
	DBASSERT (x != nullptr && y != nullptr);
	*x = this->x;
	*y = this->y;
}


template<typename Coordinate>
inline void Vector2<Coordinate>::Set (Coordinate x, Coordinate y)
{
	this->x = x;
	this->y = y;
}


template<typename Coordinate>
inline Coordinate Vector2<Coordinate>::GetLengthSqr (void) const
{
	return x * x + y * y;
}

template<typename Coordinate>
inline double Vector2<Coordinate>::CalcAngleToReference (const Vector2 <Coordinate> & refVector) const //returns angle from 0 to 2PI
{
#define  PI_over_2 (atan2 (1.0, 0.0))
	//invRotMat[0][0] = invRotMat[1][1] = refVector.x / lenRefVector
	//invRotMat[0][1] = refVector.y / lenRefVector;
	//invRotMat[1][0] = -refVector.y / lenRefVector;

	double rt_x, rt_y; //will be the inverse rotated version of this vector
	//const double lenRef = refVector.GetLength (); 
	rt_x = ( refVector.x * x + refVector.y * y) /*/ lenRefVector*/; 
	rt_y = (-refVector.y * x + refVector.x * y) /*/ lenRefVector*/; 

	//instead of (rt_x,rt_y) we do this calculation with -(rt_x,rt_y): this corresponds to 180 degree rotation, which we will compensate immediately. (numerical reasons!)
	double a = atan2 (-rt_y,-rt_x) + 2.0 * PI_over_2; 
	if (a < 0.0) {
		DBBREAK_STR ("Unexpected: a should have already be positiv"); 
		a += 4.0 * PI_over_2;
	}

	return a;

#undef PI_over_2
}


template<typename Coordinate>
inline Length Vector2<Coordinate>::GetLength (void) const
{
	return Length (Sqrt (x * x + y * y));
}

template<typename Coordinate>
inline Vector2<Coordinate> Vector2<Coordinate>::GetUnitVector (void) const
{
	Length		l;

	l = this->GetLength ();
	if (l.IsPositive (SmallEps)) {
		return Vector2 <Coordinate> (static_cast<Coordinate>(x / l), static_cast<Coordinate>(y / l));
	} else {
		DBTRACE ("ERROR -> UnitVector divided by zero!");
		return Vector2 <Coordinate> (1.0, 0.0);
	}
}


template<typename Coordinate>
inline Vector2<Coordinate> Vector2<Coordinate>::GetNormalVector (void) const
{
	return Vector2 (-y, x);
}


template<typename Coordinate>
inline Vector2<Coordinate> Vector2<Coordinate>::operator- () const
{
	Vector2 res;

	res.x = -x;
	res.y = -y;

	return res;
}

template<typename Coordinate>
inline Vector2<Coordinate> Vector2<Coordinate>::operator+ (const Vector2& c) const
{
	Vector2 res;

	res.x = x + c.x;
	res.y = y + c.y;

	return res;
}

template<typename Coordinate>
inline Vector2<Coordinate> Vector2<Coordinate>::operator- (const Vector2& c) const
{
	Vector2 res;

	res.x = x - c.x;
	res.y = y - c.y;

	return res;
}

template<typename Coordinate>
inline Coordinate Vector2<Coordinate>::operator* (const Vector2& c) const		// dot product (skalar szorzas)
{
	return x * c.x + y * c.y;
}


template<typename Coordinate>
inline Coordinate Vector2<Coordinate>::operator^ (const Vector2& c) const	// cross product (vektorialis szorzas), !! has low precedence !!
{
	return x * c.y - y * c.x;
}


template<typename Coordinate>
inline bool Vector2<Coordinate>::Normalize (void)	// returns: vector was non-null-vector
{
	Length l = this->GetLength ();
	if (l.IsPositive (SmallEps)) {
		x /= static_cast<Coordinate>(l);
		y /= static_cast<Coordinate>(l);
		return true;
	} else {
		DBTRACE ("ERROR -> UnitVector divided by zero!");
		x = y = 0;
		return false;
	}
}

template<typename Coordinate>
Vector2<Coordinate> Vector2<Coordinate>::Rotate (const Coordinate& si, const Coordinate& co) const
{
	Vector2		c2;

	c2.x = x * co - y * si;
	c2.y = y * co + x * si;
	return c2;
}

template<typename Coordinate>
inline Vector2<Coordinate> Vector2<Coordinate>::operator* (const Coordinate& k) const
{
	Vector2 res;

	res.x = x * k;
	res.y = y * k;

	return res;
}

template<typename Coordinate>
inline Vector2<Coordinate> Vector2<Coordinate>::operator/ (const Coordinate& k) const
{
	Vector2 res;

	res.x = x / k;
	res.y = y / k;

	return res;
}

template<typename Coordinate>
inline const Vector2<Coordinate>& Vector2<Coordinate>::operator+= (const Vector2& c)
{
	x += c.x;
	y += c.y;

	return *this;
}

template<typename Coordinate>
inline const Vector2<Coordinate>& Vector2<Coordinate>::operator-= (const Vector2& c)
{
	x -= c.x;
	y -= c.y;

	return *this;
}

template<typename Coordinate>
inline const Vector2<Coordinate>& Vector2<Coordinate>::operator*= (const Coordinate& k)
{
	x *= k;
	y *= k;

	return *this;
}

template<typename Coordinate>
inline const Vector2<Coordinate>& Vector2<Coordinate>::operator/= (const Coordinate& k)
{
	x /= k;
	y /= k;

	return *this;
}

template<typename Coordinate>
inline bool Vector2<Coordinate>::operator== (const Vector2& v) const
{
	return x == v.x && y == v.y;
}

template<typename Coordinate>
inline bool Vector2<Coordinate>::operator!= (const Vector2& v) const
{
	return x != v.x || y != v.y;
}

template<typename Coordinate>
inline Vector2<Coordinate> operator* (const Coordinate& k, const Vector2<Coordinate>& c)
{
	Vector2<Coordinate> res;

	res.x = k * c.x;
	res.y = k * c.y;

	return res;
}


template<typename Coordinate>
inline bool	IsNear (const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2)
{
	return IsNear (v1, v2, GetEps<Coordinate> ());
}


template<typename Coordinate>
inline bool	IsNearZero (const Vector2<Coordinate>& v)
{
	return IsNearZero (v, GetEps<Coordinate> ());
}


template<typename Coordinate>
inline bool	IsNear (const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2, double givenEps)
{
	bool isNear = false;
	Coordinate distx = v1.x - v2.x;
	if (IsNearZero (distx, givenEps)) {
		Coordinate disty = v1.y - v2.y;
		if (IsNearZero (disty, givenEps)) {
			Coordinate cubicDist = distx * distx + disty * disty;
			Coordinate cubicEps = givenEps * givenEps;
			if (IsNearZero (cubicDist, cubicEps)) {
				isNear = true;
			}
		}
	}
	return isNear;
}


template<typename Coordinate>
inline bool	IsNearZero (const Vector2<Coordinate>& v, double givenEps)
{
	bool isNearZero = false;
	Coordinate distx = v.x;
	if (IsNearZero (distx, givenEps)) {
		Coordinate disty = v.y;
		if (IsNearZero (disty, givenEps)) {
			Coordinate cubicDist = distx * distx + disty * disty;
			Coordinate cubicEps = givenEps * givenEps;
			if (IsNearZero (cubicDist, cubicEps)) {
				isNearZero = true;
			}
		}
	}
	return isNearZero;
}


template<typename Coordinate>
inline bool	IsEqualVector (const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2, double givenEps)
{
	return IsNearZero ((v1 - v2).GetLengthSqr (), givenEps * givenEps);
}


// same as GetVectorProduct
template<typename Coordinate>
Coordinate GetDeterminant (const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2)
{
	Coordinate retNum = v1.x * v2.y - v1.y * v2.x;
	return retNum;
}


template<typename Coordinate>
bool IsColinear (const Vector2<Coordinate>& v1, const Vector2<Coordinate>& v2)
{
	bool isColinear;
	if (IsNearZero (v1, SmallEps) || IsNearZero (v1, SmallEps)) {
		isColinear = true;
	} else {
		Vector2<Coordinate> v1unit = v1.GetUnitVector ();
		Vector2<Coordinate> v2unit = v2.GetUnitVector ();
		Coordinate retNum = GetDeterminant (v1unit, v2unit);

		isColinear = IsNearZero (retNum);
	}
	return isColinear;
}


template<typename Coordinate>
GSErrCode Vector2<Coordinate>::Read (GS::IChannel& ic)
{
	GSErrCode err = ic.Read (x);
	if (err == NoError)
		err = ic.Read (y);

	return err;
}


template<typename Coordinate>
GSErrCode Vector2<Coordinate>::Write (GS::OChannel& oc) const
{
	GSErrCode err = oc.Write (x);
	if (err == NoError)
		err = oc.Write (y);

	return err;
}

template<typename Coordinate>
GSErrCode Read (GS::IChannel& ic, Vector2<Coordinate>& v)
{
	return v.Read (ic);
}

template<typename Coordinate>
GSErrCode Write (GS::OChannel& oc, const Vector2<Coordinate>& v)
{
	return v.Write (oc);
}


template<typename Coordinate>
GSErrCode Store (GS::ObjectState& os, const Vector2<Coordinate>& v)
{
	Coordinate x = Coordinate ();
	Coordinate y = Coordinate ();

	v.GetCoords (&x, &y);

	os.Add ("x", x);
	os.Add ("y", y);

	return NoError;
}


template<typename Coordinate>
GSErrCode Restore (const GS::ObjectState& os, Vector2<Coordinate>& v)
{
	Coordinate x = Coordinate ();
	Coordinate y = Coordinate ();

	if (!os.Get ("x", x) || !os.Get ("y", y))
		return Error;

	v.Set (x, y);

	return NoError;
}

} // namespace Geometry

typedef Geometry::Vector2<double>		Vector;

#endif

