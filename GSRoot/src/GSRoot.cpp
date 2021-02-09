#include "stdafx.h"

#if !defined(ARCHICAD_NONE)
#include "cast-interface.h"
#endif

#include "Python.h"
#include "pybind11.h"

// https://pybind11.readthedocs.io/en/stable/
using namespace pybind11;

#define PROJECT_VERSION "1.0.0"


PYBIND11_MODULE(GSRoot, m) {
	m.add_object("version", str(PROJECT_VERSION));//Please do not delete the module version number
}