#ifndef OPENING_INPUT_STORAGE_HPP
#define OPENING_INPUT_STORAGE_HPP
#pragma once

#include "VBElemOperationsExport.hpp"
#include "Singleton.hpp"
#include "HashTable.hpp"
#include "ODBDatabase.hpp"
#include "VBElementsBase/ModelElementTypes.hpp"
#include "Opening.hpp"

namespace VBEO {
namespace Opening {


	class VB_ELEM_OPERATIONS_DLL_EXPORT OpeningStatisticsStorage : public GS::Singleton<OpeningStatisticsStorage>
	{
		friend class GS::LazyInstantiationPolicy;

	public:
		enum class CutNumberType : UInt32
		{
			SingleCut = 1,
			MultipleCut = 2
		};

		struct OpeningStatisticsResult
		{
			UInt32 openingCount;
			GS::HashTable<CutNumberType, UInt32> numberOfOpeningsByElementsCut;
			GS::HashTable<GS::Guid, UInt32> elementsCutByOpening;
			GS::HashTable<GS::Guid, UInt32> openingsByCutElements;
			UInt32 successfulInputCount;
			GS::HashTable<GS::Guid, UInt32> failedInputCount;
		};

	private:
		enum CutCounterType
		{
			OpeningCutCounter,
			ModelElemsCutCounter,
			FailedInputCounter
		};

		OpeningStatisticsResult mResult;

		OpeningStatisticsStorage ();

	public:
		virtual ~OpeningStatisticsStorage ();

		void ClearResult ();

		void AddSuccessfulInput ();
		void AddFailedInput (GS::Guid classId);

		OpeningStatisticsResult GetOpeningStatisticsResult (const ODB::Database* fileDatabase);

	private:
		void CountNumberOfElemsCutByOpening (const VBElem::ModelElementConstRefList& cutElems);
		void CountNumberOfOpeningsThatCutElem (CutCounterType cutCounterType, const VBElem::ModelElementConstRef& modelElementconst);
		void AddToCounterTable (CutCounterType cutCounterType, const GS::Guid& key);
		void ProcessOpening (const VBElem::OpeningConstRef& opening);

		GS::HashTable<GS::Guid, UInt32>& GetCounterTable (CutCounterType cutCounterType);
	};


}
}


#endif
