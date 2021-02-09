// *********************************************************************************************************************
// Description:		PGON Elim tree
//
// Module:			Model3D
// Contact person:	PP
//
// SG compatible
// *********************************************************************************************************************

#ifndef PGON_ELIM_TREE_HPP
#define PGON_ELIM_TREE_HPP

#include	"SharedObject.hpp"
#include	"PagedArray.hpp"
#include	"HashTable.hpp"
#include	"Pair.hpp"

// from GSRoot
#include	"GSPref.hpp"
#include	"BMX.hpp"
#include	"AutoPtr.hpp"
#include	"System.hpp"
#include	"GSProcessControl.hpp"

// from Geometry
#include	"Polygon2DOperations.h"

// from Model3D
#include	"Model3D.hpp"
#include	"PGONElimKDTree.hpp"
#include	"PGONElimValidPgons.hpp"

namespace Modeler {
	class Model3DViewer;
	class ElemElimData;
	class Elem;

	typedef GS::SharedPtr<ElemElimData>			ElemElimDataPtr;
	typedef GS::PagedArray<ElemElimDataPtr>		ElimDataArray;
}

namespace GS {
	class ProcessControl;
}

namespace PGONElimination {
	// poly position data


class EliminateTask
{
	bool							*eliminatedPGONs;
	const PGONElimTree				*tree;
	GS::AtomicCounter				*counter;

public:
	TreeData						*treeData;
	UInt32							treeDataCount;

	EliminateTask (	bool* eliminatedPGONs,
		const PGONElimTree *tree,
		TreeData* treeData,
		UInt32 treeDataCount,
		GS::AtomicCounter *counter);

	void	Eliminate (GS::ProcessControl& processControl);

private:
	bool	ProcessTreeData (GS::ProcessControl& processControl, UInt32 i);
};


class EliminateRunnable : public TT::Runnable {
	EliminateTask*	task;
public:
	EliminateRunnable (EliminateTask* task);
	virtual void Run () override;
};

}

MODEL3D_DLL_EXPORT
GSErrCode	MODEL3D_CALL	PGONElim (GS::ProcessControl& processControl, const Modeler::Model3DViewer& viewer, Modeler::ElimDataArray& data, const GS::HashTable<UIndex, bool> *elimElems);


#endif	// PGON_ELIM_TREE_HPP
