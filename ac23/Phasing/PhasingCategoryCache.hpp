#ifndef PHASING_PHASINGCATEGORYCACHE_HPP
#define PHASING_PHASINGCATEGORYCACHE_HPP

#pragma once

#include "PhasingExport.hpp"

#include "PhasingTypes.hpp"


namespace Phasing {

class PHASING_DLL_EXPORT PhasingCategoryCache final {
	ODB::IdSpace*						iDSpaceForCategory = nullptr;
	Phasing::RenovationStatusConstRef	category;
public:
	const Phasing::RenovationStatusConstRef& Get (ODB::IdSpace* iDSpaceForCategory);
};

} // namespace Phasing

#endif
