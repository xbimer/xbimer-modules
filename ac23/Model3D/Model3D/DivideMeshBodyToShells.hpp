#ifndef DIVIDEMESHBODYTOSHELLS_HPP
#define DIVIDEMESHBODYTOSHELLS_HPP

#include "Model3DPrivExp.h"
#include "Array.hpp"


namespace Modeler {
	class MeshBody;
}


namespace Modeler {


struct ShellOfMeshBody {
	GS::Array<UIndex> polygonIndices;
	bool isOpen;
	double volume;

	ShellOfMeshBody ();
};


MODEL3D_PRIVATE_EXPORT
GS::Array<ShellOfMeshBody>	MOD3D_PRIV DivideMeshBodyToShells (const MeshBody& body);

MODEL3D_PRIVATE_EXPORT
GS::Array<ShellOfMeshBody>	MOD3D_PRIV DivideMeshBodyVisiblePartToShells (const MeshBody& body);

}	// namespace Modeler


#endif