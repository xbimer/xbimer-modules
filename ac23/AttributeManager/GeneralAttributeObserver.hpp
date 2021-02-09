// *****************************************************************************
// General attribute
// AttributeManager, platform-independent
//
// Namespaces:					Contact person:
//	GeneralAttributeObserver		  JG
//
// [SG compatible]
// *****************************************************************************

#if !defined (GENERALATTRIBUTEOBSERVER_HPP)
#define GENERALATTRIBUTEOBSERVER_HPP

#pragma once

//============================== Includes ==================================

#include "ADBGeneralDefinitions.hpp"
#include "AttributeManagerEnv.hpp"
#include "EventObserver.hpp"

class FieldMask;

//======================= Begin of the ADB namespace =======================

namespace ADB {

//================ Class GeneralAttributeObserver ==========================
//
// Observer class for GeneralAttribute.
//
//==========================================================================

class ATTRIBUTE_DLL_EXPORT GeneralAttributeObserver : public GS::EventObserver {
protected:
	// destructor

	virtual ~GeneralAttributeObserver ()
	{
	}

public:
	// changed methods

	virtual void	NameChanged (GeneralAttributeConstRef);
	virtual void	IndexChanged (GeneralAttributeConstRef);
	virtual void	FlagsChanged (GeneralAttributeConstRef);
	virtual void	ModificationTimeChanged (GeneralAttributeConstRef);
	virtual void	UserDataChanged (GSType owner1, GSType owner2);
	virtual void	ReadOnlyChanged (GeneralAttributeConstRef);
	virtual void	GroupIdChanged (GeneralAttributeConstRef);
	virtual void	BIMDataChanged (GeneralAttributeConstRef);

	virtual void	AttributeChanged (GeneralAttributeConstRef, const FieldMask&);

	// name changed method specifying the old name too
	virtual void	NameChanged (GeneralAttributeRef, const GS::UniString&);
};


//======================== End of the ADB namespace ========================

}

#endif
