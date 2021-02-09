#ifndef TRANMAT_TO_TRANSFORMATION
#define TRANMAT_TO_TRANSFORMATION

#pragma once

#include "GeometricDefinitions.h"

struct TRANMAT;

namespace Geometry {

	class Transformation2D;
	class Transformation3D;

	GEOMETRY_DLL_EXPORT	Transformation2D TranMatToTransformation2D (const TRANMAT& tranMat);
	GEOMETRY_DLL_EXPORT Transformation3D TranMatToTransformation3D (const TRANMAT& tranMat);
}

#endif