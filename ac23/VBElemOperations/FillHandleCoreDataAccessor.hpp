#ifndef FILLHANDLE_CORE_DATA_ACCESSOR_HPP
#define FILLHANDLE_CORE_DATA_ACCESSOR_HPP

#pragma once

// === Includes ========================================================================================================

#include "VBElemOperationsExport.hpp"
#include "Ref.hpp"
#include "Point2D.hpp"
#include "UnitVector2D.hpp"
#include "GenArc2DData.h"
#include "Matrix.hpp"
#include "TypeSafeUnion.hpp"
#include "EDBGeneralDefinitions.hpp"
#include "HatchTran.hpp"

// =====================================================================================================================

namespace VBEO {
namespace FillHandle {

struct VB_ELEM_OPERATIONS_DLL_EXPORT GlobalOrigoData
{

};

class VB_ELEM_OPERATIONS_DLL_EXPORT RotationData
{
public:
	RotationData (const Point2D& localOrigo, const UnitVector_2D& direction, double scale);
	RotationData (const Point2D& localOrigo, double directionAngle, double scale);

	double		GetScale () const;
	Point2D		GetLocalOrigo () const;
	void		SetLocalOrigo (const Point2D& newLocalOrigo);

	double		GetDirectionAngle () const;
	Point2D		GetScaledDirectionVectorEndPoint () const;
	void		SetScaledDirectionVectorEndPoint (const Point2D& newEndPoint);

	UnitVector_2D GetDirection () const;

private:
	Point2D localOrigo;
	UnitVector_2D direction;
	double scale;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT DistorsionData
{
public:
	DistorsionData (const Point2D& localOrigo, const Vector2D& xAxis, const Vector2D& yAxis, double scale);

	double		GetScale () const;
	Point2D		GetLocalOrigo () const;
	void		SetLocalOrigo (const Point2D& newLocalOrigo);
	
	Point2D		GetScaledXAxisEndPoint () const;
	void		SetScaledXAxisEndPoint (const Point2D& newEndPoint);	
	Point2D		GetScaledYAxisEndPoint () const;
	void		SetScaledYAxisEndPoint (const Point2D& newEndPoint);

	Vector2D	GetXAxis () const;
	Vector2D	GetYAxis () const;

	Geometry::Matrix<2, 2, double>	GetDistorsionMatrix () const;

private:
	Point2D localOrigo;
	Vector2D xAxis;
	Vector2D yAxis;
	double scale;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT LinearGradientData
{
public:
	LinearGradientData (const Point2D& localOrigo, const Vector2D& gradient);

	Point2D		GetLocalOrigo () const;
	void		SetLocalOrigo (const Point2D& newLocalOrigo);

	Vector2D	GetGradient () const;
	Point2D		GetGradientVectorEndPoint () const;
	void		SetGradientVectorEndPoint (const Point2D& newEndPoint);

private:
	Point2D localOrigo;
	Vector2D gradient;	
};

class VB_ELEM_OPERATIONS_DLL_EXPORT RadialGradientData
{
public:
	RadialGradientData (const Point2D& localOrigo, double firstRadius, double secondRadius);

	Point2D		GetLocalOrigo () const;
	void		SetLocalOrigo (const Point2D& newLocalOrigo);
	
	double		GetFirstRadius () const;
	void		SetFirstRadius (double newRadius);
	double		GetSecondRadius () const;
	void		SetSecondRadius (double newRadius);

	GenArc		GetFirstCircle () const;
	GenArc		GetSecondCircle () const;

private:
	Point2D localOrigo;
	double firstRadius;
	double secondRadius;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT RadialDistorsionData
{
public:
	RadialDistorsionData (const Point2D& localOrigo, const Point2D& patternStartPoint, double xAxisLength, double yAxisLength);

	Point2D		GetLocalOrigo () const;
	void		OffsetLocalOrigoTo (const Point2D& newLocalOrigo);

	Point2D		GetPatternStartPoint () const;
	void		SetPatternStartPoint (const Point2D& newPoint);

	Point2D		GetYAxisEndPoint () const;
	void		SetYAxisEndPoint (const Point2D& newEndPoint);

	double							GetRadius () const;
	Geometry::Matrix<2, 2, double>	GetDistorsionMatrix () const;

private:
	Vector2D		GetXAxis () const;
	Vector2D		GetYAxis () const;
	UnitVector_2D	GetXAxisDirection () const;
	UnitVector_2D	GetYAxisDirection () const;

	Point2D localOrigo;
	Point2D patternStartPoint;
	double xAxisLength;
	double yAxisLength;
};

using TransformationData = GSUtils::Variant<GlobalOrigoData, RotationData, DistorsionData, LinearGradientData, RadialGradientData, RadialDistorsionData>;


class VB_ELEM_OPERATIONS_DLL_EXPORT TransformationQuery
{
public:
	static GS::Ref<TransformationQuery> Create (const EDB::GeneralElemConstRef& elem);

	virtual ~TransformationQuery ();

	virtual TransformationData GetTransformationData () const = 0;

	GX::Pattern::HatchTran CalculateHatchTrafo () const;
};

class VB_ELEM_OPERATIONS_DLL_EXPORT TransformationModifier : public TransformationQuery
{
public:
	static GS::Ref<TransformationModifier> Create (const EDB::GeneralElemRef& elem);

	virtual ~TransformationModifier ();

	virtual void ResetToDefault () = 0;
	virtual void OffsetLocalOrigo (const Point2D& newLocalOrigo) = 0;
	virtual void SetupByRotationData (const RotationData& rotationData) = 0;
	virtual void SetupByDistorsionData (const DistorsionData& distorsionData) = 0;
	virtual void SetupByLinearGradientData (const LinearGradientData& linearGradientData) = 0;
	virtual void SetupByRadialGradientData (const RadialGradientData& radialGradientData) = 0;
	virtual void SetupByRadialDistorsionData (const RadialDistorsionData& radialDistorsionData) = 0;
};

}
}

#endif
