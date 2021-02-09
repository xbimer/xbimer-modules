#if !defined (VBELEMOPERATIONS_GDLREQUESTPROPERTYCOLLECTOR_HPP)
#define VBELEMOPERATIONS_GDLREQUESTPROPERTYCOLLECTOR_HPP

#pragma once

// from GSRoot
#include	"Definitions.hpp"
#include	"HashSet.hpp"

#include	"GDLDefs.h"
#include	"LibrarySet.hpp"

// from VBElemOperations
#include	"VBElemOperationsExport.hpp"

// from Property
#include	"PropertyTypes.hpp"
#include	"IValueDescriptor.hpp"



namespace GDLOperations
{


class VB_ELEM_OPERATIONS_DLL_EXPORT GDLRequestPropertyCollector
{
public: 
	virtual ~GDLRequestPropertyCollector ();
			bool					IsSearchingFinished () const;

			bool					IsSearchingPropertyById () const;
			bool					IsSearchingPropertyValueById () const;
			bool					IsSearchingAllAvailableProperties () const;

			const GS::UniString		GetRequestedPropertyId () const;

	virtual void	AvailablePropertyFound (const GS::UniString& propId, const GS::UniString& propTypeName, const GS::UniString& propGrpName, const GS::UniString& propName);

	virtual void	NotAvailablePropertyValueFound ();
	virtual void	NotEvaluablePropertyValueFound ();
	virtual void	UndefinedPropertyValueFound (Property::ValueType valType, const Property::Measure& measure);
			void	UndefinedPropertyValueFoundAsString (); 
	virtual void	NormalPropertyValueFound (Property::ValueType valType, const Property::Measure& measure, Int32 dim1, Int32 dim2, const GDLRequestResult& propValues); 
			void	NormalPropertyValueFoundAsString (const GS::UniString& propValue); 

protected:
	GDL_PropertyValueType	PropertyValueType2GDLParType (Property::ValueType valType, const Property::Measure& measure);
	void					SetSearchingIsFinished ();

	enum class CollectMode
	{
		PropertyById,
		PropertyValueById,
		AllAvailableProperties
	};
	GDLRequestPropertyCollector (CollectMode collectMode, GS::UniString requestedPropertyId);

	CollectMode		collectMode;
	GS::UniString	requestedPropertyId;
	bool			isSearchingFinished;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT FindAllAvailablePropertiesCollector : public GDLRequestPropertyCollector
{
public:
	FindAllAvailablePropertiesCollector (const LM::LibrarySet* librarySet_init, Int32* retValue_init, GDLRequestResult& values_init);
	virtual ~FindAllAvailablePropertiesCollector ();
	
	virtual void	AvailablePropertyFound (const GS::UniString& propId, const GS::UniString& propTypeName, const GS::UniString& propGrpName, const GS::UniString& propName) override;

private:
	const LM::LibrarySet*		librarySet;
	Int32*						retValue;
	GDLRequestResult&			values;
	GS::HashSet<GS::UniString>	propertyIds;
};



class VB_ELEM_OPERATIONS_DLL_EXPORT FindPropertyByIdCollector : public GDLRequestPropertyCollector
{
public:
	FindPropertyByIdCollector (const LM::LibrarySet* librarySet_init, const GS::UniString& propId_key_init,
						   Int32* retValue_init, GS::UniString& propertyType_init, GS::UniString& propertyGroupName_init, GS::UniString& propertyName_init);

	virtual ~FindPropertyByIdCollector ();

	virtual void	AvailablePropertyFound (const GS::UniString& /*propId*/, const GS::UniString& propTypeName_in, const GS::UniString& propGrpName_in, const GS::UniString& propName_in) override;

private:
	const LM::LibrarySet*	librarySet;
	
	Int32*					retValue;
	GS::UniString&			propertyType;
	GS::UniString&			propertyGroupName;
	GS::UniString&			propertyName;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT FindPropertyValueByIdCollector : public GDLRequestPropertyCollector
{
public:
	FindPropertyValueByIdCollector (const LM::LibrarySet* librarySet_init, const GS::UniString& propId_key_init,
									Int32* retValue_init, GDLRequestResult& values_init);

	virtual ~FindPropertyValueByIdCollector ();
	
	virtual void	NotAvailablePropertyValueFound () override;
	virtual void	NotEvaluablePropertyValueFound () override;
	virtual void	UndefinedPropertyValueFound (Property::ValueType /*valType*/, const Property::Measure& /*measure*/) override;
	virtual void	NormalPropertyValueFound (Property::ValueType valType, const Property::Measure& measure, Int32 dim1, Int32 dim2, const GDLRequestResult& propValues) override;

private:
	const LM::LibrarySet*	librarySet;

	Int32*						retValue;
	GDLRequestResult&			values;
};


class VB_ELEM_OPERATIONS_DLL_EXPORT FindPropertyValueAsDictionaryByIdCollector : public GDLRequestPropertyCollector
{
public:
	FindPropertyValueAsDictionaryByIdCollector (const GS::UniString& inPropertyId, GDL::Dictionary& resultOut);

	virtual ~FindPropertyValueAsDictionaryByIdCollector ();

	virtual void	NotAvailablePropertyValueFound () override;
	virtual void	NotEvaluablePropertyValueFound () override;
	virtual void	UndefinedPropertyValueFound (Property::ValueType valType, const Property::Measure& measure) override;
	virtual void	NormalPropertyValueFound (Property::ValueType valType, const Property::Measure& measure, Int32 /*dim1*/, Int32 /*dim2*/, const GDLRequestResult& propValues) override;

private:
	const GS::UniString		propertyId;
	Int32*					retValue;
	GDL::Dictionary&		result;
};


}

#endif	// GDLREQUESTS_HPP
