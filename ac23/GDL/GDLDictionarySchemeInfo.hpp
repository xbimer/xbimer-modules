#ifndef GDL_GDLDICTIONARYSCHEMES_HPP
#define GDL_GDLDICTIONARYSCHEMES_HPP

#include "GDLDictionaryValidation.hpp"

namespace GDL
{
	class Dictionary;


	enum class DictionarySchemeIndex
	{
		NotFilledGdlGlobal,
		InvalidRequestResult,
		BoundingBox,
		OpeningRelativeHeight,
		Polygon2D,
		OpeningPolygon,
		ComponentId,
		PropertyId,
		ComponentIdsOfParentRequestInput,
		ComponentIdsOfParentRequestResult,
		PropertyValuesOfParentRequestInput,
		ComponentPropertyValuesOfParentRequestInput,
		PropertyValuesRequestResult,
		
		LastDictionarySchemeIndex = PropertyValuesRequestResult
	};

	class GDL_DLL_EXPORT IGdlDictionarySchemeInfo
	{
	public:
		IGdlDictionarySchemeInfo ();
		virtual ~IGdlDictionarySchemeInfo ();
		
		IGdlDictionarySchemeInfo (const IGdlDictionarySchemeInfo&) = delete;
		void operator= (const IGdlDictionarySchemeInfo&) = delete;

		virtual void	Registered (DictionarySchemeIndex index) = 0;

		virtual const GS::UniString&		GetName () const = 0;
		virtual bool						IsValid (const GDL::Dictionary& dict) const = 0;
		virtual GS::Optional<GS::UniString>	GetErrorMessage (const GDL::Dictionary& dict) const = 0;

		virtual void						DumpTechnicalInfo (ReportDisplayTextBuilder& display) const = 0;
	};

	GDL_DLL_EXPORT const IGdlDictionarySchemeInfo*	FindGdlDictionarySchemeInfoByIndex (DictionarySchemeIndex index);
	GDL_DLL_EXPORT void								DumpTechnicalInfoOfGdlDictionarySchemes (ReportDisplayTextBuilder&);

	void											InitGdlDictionarySchemeInfos ();
	void											ExitGdlDictionarySchemeInfos ();
}

#endif