#ifndef _VBELEMOPERATIONS_GDLOPERATIONSTYPES_HPP_
#define _VBELEMOPERATIONS_GDLOPERATIONSTYPES_HPP_


namespace LibCtrl {
	enum MigrationDirection {
		NotMigration,
		ForwardMigration,
		BackwardMigration
	};
	enum ParamListMode { 
		ParamListFixMode,
		ParamListEditingMode
	};
}

namespace GDLOperations {
	enum RunVLScriptMode {
		RVLMRunUntilNoChange,
		RVLMRunOnlyOnce		
	};

	enum ForceVLScriptMode {
		FVLMForceRunUntilNoChange,
		FVLMForceRunOnlyOnce,
		FVLMDontForce
	};

	enum	class ViewType {
		VT_FloorPlanDDD,
		VT_Other
	};
}


#endif