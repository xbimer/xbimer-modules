#ifndef _GDLOPERATIONS_PARTIALGDLREQUESTS_HPP_
#define _GDLOPERATIONS_PARTIALGDLREQUESTS_HPP_

#include "GDLRequests.hpp"

namespace GDLOperations
{

class VB_ELEM_OPERATIONS_DLL_EXPORT PartialGDLRequests : public GDLRequests
{
public:
	PartialGDLRequests (const GDLRequestCallbacks& callbacks, 
					  const GDLRequestData& gdlRequestData, 
					  const EDB::DefaultElem* defaultElem, 
					  const GDL_ScriptType& scriptType,
					  EDB::GeneralElemConstRef elemRef,
					  const IPropertyBasedReaderFactory& elemReaderFactory,
					  const ViewType viewType,
					  const short floorPlanDDDStory,
					  UInt64 attributesModiStamp = 0);

	virtual ~PartialGDLRequests ();

	virtual GSErr		PropertiesOfParent (	Int32 reqIndex,
												Int32* retValue,
												GDLRequestResult& values) override;

	virtual GSErr		PropertyValueOfParent (	const GS::UniString& propertyId,
												Int32* retValue,
												GDLRequestResult& values) override;

	virtual GSErr		PropertyValuesOfParent (const GDL::Dictionary&		dictValue,
												Int32*						retValue,
												GDLRequestResult&			values) override;

	virtual GSErr		PropertyNameOfParent (	const GS::UniString& propertyId,
												Int32* retValue,
												GS::UniString& propertyType,
												GS::UniString& propertyGroupName,
												GS::UniString& propertyName) override;

	virtual GSErr		ComponentPropertyValuesOfParent (const GDL::Dictionary&	dictValue, Int32* retValue, GDLRequestResult& result) override;
	virtual GSErr		ComponentPropertiesOfParent (Int32 reqIndex, Int32* retValue, GDLRequestResult& values) override;

	virtual GSErr		AutoTextList (Int32* retValue,
									  GDLRequestResult& values) override;
};

}

#endif