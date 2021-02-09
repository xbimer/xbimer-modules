// Contact person : KiP

#ifndef PROPERTYOPERATIONS_HPP
#define PROPERTYOPERATIONS_HPP

#pragma once

// === Includes ========================================================================================================

#include "AutoPtr.hpp"

// from VBElements
#include "Project.hpp"

// from VBElemOperations
#include "GDLRequests.hpp"

// from PropertyOperations
#include "PropertyOperationsExport.hpp"
#include "PropertyOperationsTypes.hpp"
#include "PropertyOwner.hpp"

// === Function prototypes  ============================================================================================

namespace GDLOperations {


class PROPERTY_OPERATIONS_DLL_EXPORT GDLRequestsWithPropertyHandling : public GDLRequests
{
protected:
	GDLRequestsWithPropertyHandling (const GDLRequestCallbacks& callbacks, 
									 const GDLRequestData& gdlRequestData, 
									 const EDB::DefaultElem* defaultElem, 
									 const GDL_ScriptType& scriptType,
									 EDB::GeneralElemConstRef elemRef,
									 const IPropertyBasedReaderFactory& elemReaderFactory,
									 const ViewType viewType,
									 const short floorPlanDDDStory,
									 UInt64 attributesModiStamp = 0);

	virtual ~GDLRequestsWithPropertyHandling ();

public:
	class PROPERTY_OPERATIONS_DLL_EXPORT IUserDefinedPropertyFilter
	{
	public:
		virtual ~IUserDefinedPropertyFilter ();
		
		virtual bool ShouldIncludeProperty (const Property::PropertyDefinitionExtensionConstPtr& propExt) const = 0;
	};

	static void	CollectPropertiesCommon_UserDefined (GDLOperations::GDLRequestPropertyCollector& collector,
										  const Property::UserDefinedPropertyDefinitionContainerSetConstRef& containerSet,
										  Property::PropertyEvaluationEnvironmentSourceConstPtr propEvalEnv,
										  Property::PropertyOwnerConstPtr propertyOwner,
										  const IUserDefinedPropertyFilter& propertyFilter);

	static void	CollectPropertiesCommon_DynamicBuiltIn (GDLOperations::GDLRequestPropertyCollector& collector,
									  Property::PropertyEvaluationEnvironmentSourceConstPtr propEvalEnv,
									  const Property::PropertyOwnerConstPtr& propertyOwner,
									  const GS::Guid& dynamicGroupId,
									  const GS::UniString& typeDisplayName,
									  const GS::UniString& groupDisplayName);
};



class PROPERTY_OPERATIONS_DLL_EXPORT ByOwnerAvailabilityUserDefinedPropertyFilter : public GDLRequestsWithPropertyHandling::IUserDefinedPropertyFilter
{
public:
	ByOwnerAvailabilityUserDefinedPropertyFilter (const Property::PropertyOwnerConstPtr& propertyOwner);
	virtual ~ByOwnerAvailabilityUserDefinedPropertyFilter ();
		
	virtual bool ShouldIncludeProperty (const Property::PropertyDefinitionExtensionConstPtr& propExt) const override;
	
private:
	Property::PropertyOwnerConstPtr propertyOwner;
};

}

#endif
