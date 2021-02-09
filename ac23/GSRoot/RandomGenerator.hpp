
// *****************************************************************************
//
//					Random generator
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	SzeA
//
//
// *****************************************************************************


#ifndef	RANDOMGENERATOR_HPP
#define	RANDOMGENERATOR_HPP

#pragma once

// from GRoot
#include "GSDebug.hpp"

#include "Definitions.hpp"

namespace GS {

class GSROOT_DLL_EXPORT RandomGenerator {
private:
	UInt32 w;	// used for random number generation
	UInt32 z;	// used for random number generation

	UInt32	Generate ();

public:
	RandomGenerator (UInt32 seed = 1);

	void	SetSeed	(UInt32 seed);

	UInt32	GetUInt ();
	UInt32	GetUInt	(UInt32 from, UInt32 to);

	Int32	GetInt ();
	Int32	GetInt (Int32 from, Int32 to);

	double	GetDouble (double from, double to);

	bool	GetBool	();
	bool	GetBool	(UInt32 trueProbability);		// 0% <= trueProbability <= 100%

	UInt32	GetLog2 ();								// 50% => 0, 25% => 1, 12.5% => 2 ...
};

}	// namespace GS

#endif