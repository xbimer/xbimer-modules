#ifndef	PROPERTY_COPIER_HPP
#define	PROPERTY_COPIER_HPP

#pragma once

#include "PropertyOperationsExport.hpp"
#include "EDBGeneralDefinitions.hpp"
#include "PropertyOperationsTypes.hpp"
#include "PropertyAccessor.hpp"
#include "HashSet.hpp"

namespace PropertyOperations
{

	class PROPERTY_OPERATIONS_DLL_EXPORT PropertyCopier
	{
	public:
		PropertyCopier (const Property::PropertyOwnerConstPtr& sourceElemPropertyOwner, const Property::PropertyOwnerConstPtr& targetElemPropertyOwner, const Property::PropertyAccessor& propertyAccessor);
		virtual ~PropertyCopier ();

		void CopyBuiltInPropertiesByGuid (const GS::HashSet<GS::Guid>& propertyGuids);
		void CopyAllValidUserDefinedProperites ();

	private:
		const Property::PropertyOwnerConstPtr sourceElemPropertyOwner;
		const Property::PropertyOwnerConstPtr targetElemPropertyOwner;
		const Property::PropertyAccessor propertyAccessor;

		void ProcessCopy (const Property::PropertyDefinitionExtensionConstPtr& propExt);
	};

}


#endif