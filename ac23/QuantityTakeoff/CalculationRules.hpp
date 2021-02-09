#ifndef CALCULATIONRULES_HPP
#define CALCULATIONRULES_HPP
#pragma once

// === Includes ========================================================================================================

// from GSRoot
#include "HashSet.hpp"
#include "Object.hpp"

// from QuantityTakeOff
#include "QuantityTakeoffExport.hpp"

#include "ADBAttributeIndex.hpp"

// =====================================================================================================================

class QUANTITYTAKEOFF_DLL_EXPORT CalculationRules : public GS::Object {
		DECLARE_CLASS_INFO

private:
	double	wallHoleMinVolume;
	bool	wallHoleMinVolumeFlag;
	double	wallHoleMinSurface;
	bool	wallHoleMinSurfaceFlag;
	double	wallHoleMinLength;
	bool	wallHoleMinLengthFlag;

	double	ceilHoleMinVolume;
	bool	ceilHoleMinVolumeFlag;
	double	ceilHoleMinSurface;
	bool	ceilHoleMinSurfaceFlag;

	double	roofHoleMinVolume;
	bool	roofHoleMinVolumeFlag;
	double	roofHoleMinSurface;
	bool	roofHoleMinSurfaceFlag;

	double	shellHoleMinVolume;
	bool	shellHoleMinVolumeFlag;
	double	shellHoleMinSurface;
	bool	shellHoleMinSurfaceFlag;

	double	beamHoleMinVolume;
	bool	beamHoleMinVolumeFlag;

	GS::HashSet<ADB::AttributeIndex>		wallInsulationFills;
	GS::HashSet<ADB::AttributeIndex>		wallAirFills;
	GS::HashSet<ADB::AttributeIndex>		roofInsulationFills;
	GS::HashSet<ADB::AttributeIndex>		shellInsulationFills;

	GSErrCode							ReadVersion1 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode							WriteVersion1 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode							ReadVersion2 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode							WriteVersion2 (GS::OChannel& oc, const GS::OutputFrame& frame) const;
	GSErrCode							ReadVersion3 (GS::IChannel& ic, const GS::InputFrame& frame);
	GSErrCode							WriteVersion3 (GS::OChannel& oc, const GS::OutputFrame& frame) const;

public:
	CalculationRules ();
	~CalculationRules ();

	CalculationRules&					operator= (const CalculationRules& source);

	virtual	GSErrCode					Read (GS::IChannel& ic) override;
	virtual	GSErrCode					Write (GS::OChannel& oc) const override;

	virtual	GSErrCode					ReadXML (GS::XMLIChannel& ic);
	virtual	GSErrCode					WriteXML (GS::XMLOChannel& oc) const;

	inline	double						GetWallHoleMinVolume () const						{ return wallHoleMinVolume;			}
	inline	void						SetWallHoleMinVolume (double value)					{ wallHoleMinVolume = value;		}
	inline	bool						GetWallHoleMinVolumeFlag () const					{ return wallHoleMinVolumeFlag;		}
	inline	void						SetWallHoleMinVolumeFlag (bool value)				{ wallHoleMinVolumeFlag = value;	}
	inline	double						GetWallHoleMinSurface () const						{ return wallHoleMinSurface;		}
	inline	void						SetWallHoleMinSurface (double value)				{ wallHoleMinSurface = value;		}
	inline	bool						GetWallHoleMinSurfaceFlag () const					{ return wallHoleMinSurfaceFlag;	}
	inline	void						SetWallHoleMinSurfaceFlag (bool value)				{ wallHoleMinSurfaceFlag = value;	}
	inline	double						GetWallHoleMinLength () const						{ return wallHoleMinLength;			}
	inline	void						SetWallHoleMinLength (double value)					{ wallHoleMinLength = value;		}
	inline	bool						GetWallHoleMinLengthFlag () const					{ return wallHoleMinLengthFlag;		}
	inline	void						SetWallHoleMinLengthFlag (bool value)				{ wallHoleMinLengthFlag = value;	}

	inline	double						GetCeilHoleMinVolume () const						{ return ceilHoleMinVolume;			}
	inline	void						SetCeilHoleMinVolume (double value)					{ ceilHoleMinVolume = value;		}
	inline	bool						GetCeilHoleMinVolumeFlag () const					{ return ceilHoleMinVolumeFlag;		}
	inline	void						SetCeilHoleMinVolumeFlag (bool value)				{ ceilHoleMinVolumeFlag = value;	}
	inline	double						GetCeilHoleMinSurface () const						{ return ceilHoleMinSurface;		}
	inline	void						SetCeilHoleMinSurface (double value)				{ ceilHoleMinSurface = value;		}
	inline	bool						GetCeilHoleMinSurfaceFlag () const					{ return ceilHoleMinSurfaceFlag;	}
	inline	void						SetCeilHoleMinSurfaceFlag (bool value)				{ ceilHoleMinSurfaceFlag = value;	}

	inline	double						GetRoofHoleMinVolume () const						{ return roofHoleMinVolume;			}
	inline	void						SetRoofHoleMinVolume (double value)					{ roofHoleMinVolume = value;		}
	inline	bool						GetRoofHoleMinVolumeFlag () const					{ return roofHoleMinVolumeFlag;		}
	inline	void						SetRoofHoleMinVolumeFlag (bool value)				{ roofHoleMinVolumeFlag = value;	}
	inline	double						GetRoofHoleMinSurface () const						{ return roofHoleMinSurface;		}
	inline	void						SetRoofHoleMinSurface (double value)				{ roofHoleMinSurface = value;		}
	inline	bool						GetRoofHoleMinSurfaceFlag () const					{ return roofHoleMinSurfaceFlag;	}
	inline	void						SetRoofHoleMinSurfaceFlag (bool value)				{ roofHoleMinSurfaceFlag = value;	}

	inline	double						GetShellHoleMinVolume () const						{ return shellHoleMinVolume;		}
	inline	void						SetShellHoleMinVolume (double value)				{ shellHoleMinVolume = value;		}
	inline	bool						GetShellHoleMinVolumeFlag () const					{ return shellHoleMinVolumeFlag;	}
	inline	void						SetShellHoleMinVolumeFlag (bool value)				{ shellHoleMinVolumeFlag = value;	}
	inline	double						GetShellHoleMinSurface () const						{ return shellHoleMinSurface;		}
	inline	void						SetShellHoleMinSurface (double value)				{ shellHoleMinSurface = value;		}
	inline	bool						GetShellHoleMinSurfaceFlag () const					{ return shellHoleMinSurfaceFlag;	}
	inline	void						SetShellHoleMinSurfaceFlag (bool value)				{ shellHoleMinSurfaceFlag = value;	}

	inline	double						GetBeamHoleMinVolume () const						{ return beamHoleMinVolume;			}
	inline	void						SetBeamHoleMinVolume (double value)					{ beamHoleMinVolume = value;		}
	inline	bool						GetBeamHoleMinVolumeFlag () const					{ return beamHoleMinVolumeFlag;		}
	inline	void						SetBeamHoleMinVolumeFlag (bool value)				{ beamHoleMinVolumeFlag = value;	}

	inline	const GS::HashSet<ADB::AttributeIndex>&		GetWallInsulationFills () const									{ return wallInsulationFills;		}
	inline	GS::HashSet<ADB::AttributeIndex>&			GetWallInsulationFills ()										{ return wallInsulationFills;		}
	inline	void										SetWallInsulationFills (GS::HashSet<ADB::AttributeIndex>& data)	{ wallInsulationFills = data;		}

	inline	const GS::HashSet<ADB::AttributeIndex>&		GetWallAirFills () const										{ return wallAirFills;				}
	inline	GS::HashSet<ADB::AttributeIndex>&			GetWallAirFills ()												{ return wallAirFills;				}
	inline	void										SetWallAirFills (GS::HashSet<ADB::AttributeIndex>& data)		{ wallAirFills = data;				}

	inline	const GS::HashSet<ADB::AttributeIndex>&		GetRoofInsulationFills () const									{ return roofInsulationFills;		}
	inline	GS::HashSet<ADB::AttributeIndex>&			GetRoofInsulationFills ()										{ return roofInsulationFills;		}
	inline	void										SetRoofInsulationFills (GS::HashSet<ADB::AttributeIndex>& data)	{ roofInsulationFills = data;		}

	inline	const GS::HashSet<ADB::AttributeIndex>&		GetShellInsulationFills () const									{ return shellInsulationFills;		}
	inline	GS::HashSet<ADB::AttributeIndex>&			GetShellInsulationFills ()											{ return shellInsulationFills;		}
	inline	void										SetShellInsulationFills (GS::HashSet<ADB::AttributeIndex>& data)	{ shellInsulationFills = data;		}
};


#endif
