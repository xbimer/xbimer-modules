#ifndef GDL_PROPERTYDICTCONVERSIONS_HPP
#define	GDL_PROPERTYDICTCONVERSIONS_HPP

#pragma once

#include "GDLExport.h"
#include "GDLDictionary.hpp"
#include "GDLDefs.h"

namespace GDL {


class GDL_DLL_EXPORT ComponentIdsRequestInput
{
public:
	GDL_ComponentCollectMode		GetCollectMode () const;

	static GS::Optional<ComponentIdsRequestInput> ParseDict (const GDL::Dictionary&);

private:
	ComponentIdsRequestInput (GDL_ComponentCollectMode);

	GDL_ComponentCollectMode	collectMode;
};

	

class GDL_DLL_EXPORT GDLRequestComponentId
{
public:

	static	GDLRequestComponentId	CreateEmptyComponentId ();
			bool					IsEmptyComponentId () const;

	static	GDLRequestComponentId	CreateFromSkinIndex (short id);

			short					GetSkinIndex () const;
			

	static	GS::Optional<GDLRequestComponentId>	ParseDict (const GDL::Dictionary&);
			GDL::Dictionary						ToDict () const;

private:
	GDLRequestComponentId (Int32 id);

	static const Int32 ID_MULTIPLY = 7;
	static const Int32 ID_OFFSET_BEFORE = 42;
	static const Int32 ID_OFFSET_AFTER = 37;

	static	Int32	FromInternalToGDL (Int32 internalId);
	static	Int32	FromGDLToInternal (Int32 gdlId);
	static	bool	IsValidGDLId (Int32 gdlId);

	Int32	id;
};



class GDL_DLL_EXPORT GDLRequestPropertyIdList
{
public:
	bool							IsEmpty () const;
	const GS::Array<GS::UniString>&	GetPropertyIds () const;

	static  GS::Optional<GDLRequestPropertyIdList> ParsePartialDict (const GDL::Dictionary&);

private:
	GDLRequestPropertyIdList (const GS::Array<GS::UniString> propertyIds);

	GS::Array<GS::UniString>	propertyIds;
};



class GDL_DLL_EXPORT PropertyValuesOfParentRequestInput
{
public:
	const GDLRequestPropertyIdList&			GetPropertyIdList () const;

	static GS::Optional<PropertyValuesOfParentRequestInput> ParseDict (const GDL::Dictionary&);

private:
	PropertyValuesOfParentRequestInput (const GDLRequestPropertyIdList&);

	GDLRequestPropertyIdList				propertyIdList;
};



class GDL_DLL_EXPORT ComponentPropertyValuesOfParentRequestInput
{
public:
	const GDLRequestComponentId&			GetComponentId () const;
	const GDLRequestPropertyIdList&			GetPropertyIdList () const;

	static GS::Optional<ComponentPropertyValuesOfParentRequestInput> ParseDict (const GDL::Dictionary&);

private:
	ComponentPropertyValuesOfParentRequestInput (const GDLRequestComponentId&, const GDLRequestPropertyIdList&);

	GDLRequestComponentId		componentId;
	GDLRequestPropertyIdList	propertyIdList;
};


GDL_DLL_EXPORT Dictionary CreateComponentIdsRequestResult (const GS::Array<GDLRequestComponentId>& componentIds);

GDL_DLL_EXPORT Dictionary CreateNotAvailablePropertyValueDict ();
GDL_DLL_EXPORT Dictionary CreateNotEvaluablePropertyValueDict ();
GDL_DLL_EXPORT Dictionary CreateUndefinedPropertyValueDict (GDL_PropertyValueType gdlType);
GDL_DLL_EXPORT Dictionary CreateNormalPropertyValueDict (GDL_PropertyValueType gdlType, const GDLRequestResult& propValues);

GDL_DLL_EXPORT Dictionary CreatePropertyValuesRequestResult (const GS::Array<GDL::Dictionary>&);

}

#endif	//GDL_PROPERTYDICTCONVERSIONS_HPP
