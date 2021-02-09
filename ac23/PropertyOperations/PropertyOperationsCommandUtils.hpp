// *********************************************************************************************************************
// File:			PropertyOperationsCommandUtils.hpp
//
// Module:		    PropertyOperations
// Namespace:		Property
// Contact person:	BIM
//
// *********************************************************************************************************************


#ifndef PROPERTYOPERATIONSCOMMANDUTILS_HPP
#define PROPERTYOPERATIONSCOMMANDUTILS_HPP

#pragma once


#include "ObjectState.hpp"
#include "ProjectTypes.hpp"
#include "PropertyOperationsExport.hpp"
#include "PropertyOperationsTypes.hpp"


namespace PropertyOperations {
namespace Commands {

PROPERTY_OPERATIONS_DLL_EXPORT void	Init ();

}	// namespace Commands
}	// namespace PropertyOperations


namespace Property {

PropertyAccessor	GetPropertyAccessorForCommands (const EDB::ProjectConstRef& project);

}	// namespace Property


#endif
