#ifndef UDITEMINFOTAGSETTINGS_HPP
#define UDITEMINFOTAGSETTINGS_HPP
#pragma once

#include "UDDefinitions.hpp"

namespace UD {
namespace ItemInfoTag {

/************************************************************************/
/* SettingsOptions                                                      */
/************************************************************************/

class UD_DLL_EXPORT SettingsOptions {
public:
	enum class Alignment {
		ToCenterOfItem,
		ToRightOfItem,
		HorizontallyToMouse
	};

	SettingsOptions ();
	virtual ~SettingsOptions ();

	bool						GetUseOwnTooltipDelayForHoverInWaitTime () const;
	Int32						GetHoverInWaitMillisecs () const;
	Int32						GetHoverInMillisecs () const;
	Int32						GetHoverOutWaitMillisecs () const;
	Int32						GetHoverOutMillisecs () const;
	short						GetVerticalOffset () const;
	Alignment					GetAlignment () const;

	SettingsOptions&			SetUseOwnTooltipDelayForHoverInWaitTime (bool useOwnTooltipDelayForHoverInWaitTime);
	SettingsOptions&			SetHoverInWaitMillisecs (Int32 hoverInWaitMillisecs);
	SettingsOptions&			SetHoverInMillisecs (Int32 hoverInMillisecs);
	SettingsOptions&			SetHoverOutWaitMillisecs (Int32 hoverOutWaitMillisecs);
	SettingsOptions&			SetHoverOutMillisecs (Int32 hoverOutMillisecs);
	SettingsOptions&			SetVerticalOffset (short verticalOffset);
	SettingsOptions&			SetAlignment (Alignment alignment);

protected:
	bool						useOwnTooltipDelayForHoverInWaitTime;
	Int32						hoverInWaitMillisecs;
	Int32						hoverInMillisecs;
	Int32						hoverOutWaitMillisecs;
	Int32						hoverOutMillisecs;
	short						verticalOffset;
	Alignment					alignment;
};


/************************************************************************/
/* RegistryBasedSettingsOptions                                         */
/************************************************************************/

class UD_DLL_EXPORT RegistryBasedSettingsOptions : public SettingsOptions
{
public:
	RegistryBasedSettingsOptions ();
	virtual ~RegistryBasedSettingsOptions ();
};


/************************************************************************/
/* ISettingsProvider                                                    */
/************************************************************************/

class UD_DLL_EXPORT SettingsProvider {
private:
	SettingsOptions settingsOptions;
public:
	SettingsProvider (SettingsOptions settingsOptions);
	virtual ~SettingsProvider ();

	const SettingsOptions& GetSettingsOptions () const;

	void ApplyOnInfoTagProperty (SettingsOptions& settingsOptions) const;
};


typedef GS::Ref<SettingsProvider> SettingsProviderRef;

}
}

#endif
