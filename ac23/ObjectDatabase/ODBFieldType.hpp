
// *****************************************************************************
//
//                              Class FieldType
//
// Module:		   ObjectDatabase
// Namespace:	   ODB
// Contact person: MB
//
// SG compatible
//
// *****************************************************************************


#if !defined ODBFIELDTYPE_HPP
#define ODBFIELDTYPE_HPP

#pragma once


// --------------------------------- Includes ----------------------------------

#include "ODBBase.hpp"

#include <functional>

namespace GS { class String; }
namespace GS { class TypeIndex; }
namespace GS { class ObjectState; }
namespace ODB { class DynamicObject; }
namespace GS { class RandomGenerator; }


// ============================== Class FieldType ==============================

namespace ODB {

typedef std::function<void (const DynamicObject&)> GetFunction;

class ODB_DLL_EXPORT FieldType {
protected:
	template <class Type>
	static void		GetValueImp (const DynamicObject& object, const GetFunction& getFunction, const std::function<void (const char*)>& processor);

public:
	virtual ~FieldType ();

	virtual UInt32	GetSize (void) const = 0;

	virtual void	Construct		  (char* target) const = 0;
	virtual void	ConstructWithCopy (char* target, const char* sourceField) const = 0;
	virtual void	ConstructWithMove (char* target, char* sourceField) const = 0;

	virtual void	AssignWithCopy (char* targetField, const char* sourceField) const = 0;
	virtual void	AssignWithMove (char* targetField, char* sourceField) const = 0;

	virtual void	Destroy (char* field) const = 0;

	virtual void	GetValue (const DynamicObject& object, const GetFunction& getFunction, const std::function<void (const char*)>& processor) const = 0;

	virtual GSErrCode	Read  (char* field, GS::IChannel& ic) const = 0;
	virtual GSErrCode	Write (const char* field, GS::OChannel& oc) const = 0;

		// ObjectState storage

	virtual GSErrCode	Store	(const GS::String& fieldName, const char* field, GS::ObjectState& os) const;
	virtual GSErrCode	Restore	(const GS::String& fieldName, char* field, const GS::ObjectState& os) const;

		// compare

	virtual bool	ImplementsEquality () const;
	virtual bool	IsEqual			 (const char* field1, const char* field2) const;	// base method
	virtual bool	IsNotEqual		 (const char* field1, const char* field2) const;	// derived from IsEqual

	virtual bool	ImplementsOrdering () const;
	virtual bool	IsGreater		 (const char* field1, const char* field2) const;	// base method
	virtual bool	IsGreaterOrEqual (const char* field1, const char* field2) const;	// derived from IsGreater and IsEqual
	virtual bool	IsLess			 (const char* field1, const char* field2) const;	// derived from IsGreater and IsEqual
	virtual bool	IsLessOrEqual	 (const char* field1, const char* field2) const;	// derived from IsGreater and IsEqual

		// conversion

	virtual bool		ImplementsToText () const;
	virtual GS::String	ToText (const char* field) const;

	virtual bool		ImplementsFromText () const;
	virtual GSErrCode	FromText (const GS::String& text, char* field) const;

		// debug and test

	virtual void	Assign (char* field, GS::RandomGenerator& randomGenerator) const;
};


template <class Type>
void	FieldType::GetValueImp (const DynamicObject& object, const GetFunction& getFunction, const std::function<void (const char*)>& processor)
{
	Type result = reinterpret_cast<const std::function<Type (const DynamicObject&)>&> (getFunction) (object);

	processor (reinterpret_cast<const char*> (&result));
}


ODB_DLL_EXPORT FieldType*		RegisterFieldType (FieldType* fieldType, const GS::TypeIndex& typeIndex);

template <typename Type>
class FieldTypeFactory {
public:
	static const FieldType&		Get (void);
};


template <typename Type>
const FieldType&	GetFieldType (void)               // should return function static FieldType instance for the given Type
{
	return 	FieldTypeFactory<Type>::Get ();
}


template <typename Type>
const FieldType&	CallGetFieldType (void)
{
	return GetFieldType<Type> ();
}


template <typename Type>
bool	IsFieldType (const FieldType* fieldType)
{
	return &GetFieldType<Type> () == fieldType;
}


ODB_DLL_EXPORT void		TypeMismatchError (void);


template <typename Type>
void	CheckFieldType (const FieldType* fieldType)
{
	if (!IsFieldType<Type> (fieldType))
		TypeMismatchError ();
}

}	// namespace ODB


#endif
