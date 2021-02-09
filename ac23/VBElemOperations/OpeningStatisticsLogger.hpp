#ifndef OPENING_STATISTICS_LOGGER_HPP
#define OPENING_STATISTICS_LOGGER_HPP

#pragma once

#include "VBElemOperationsExport.hpp"
#include "OpeningStatisticsStorage.hpp"
#include "GSGuid.hpp"
#include "UniString.hpp"
#include "UsageLogger.hpp"


namespace VBEO {
namespace Opening {


class VB_ELEM_OPERATIONS_DLL_EXPORT OpeningStatisticsLogger
{
private:
	static const GS::Guid OpeningCountLogId;
	static const GS::Guid NumberOfOpeningsByElementsCutLogId;
	static const GS::Guid OpeningsByCutElementsLogId;
	static const GS::Guid ElementsCutByOpeningLogId;
	static const GS::Guid SuccessfulOpeningInputCountLogId;
	static const GS::Guid FailedOpeningInputCountLogId;

	const ODB::Database* mFileDatabase;
	const GS::UniString mProjectId;
	OpeningStatisticsStorage::OpeningStatisticsResult mOpeningStatisticsResult;

public:
	OpeningStatisticsLogger (const ODB::Database* fileDatabase, GS::UniString projectId);
	virtual ~OpeningStatisticsLogger ();

	void LogStatistics ();

private:
	void LogOpeningCount ();
	void LogCutOpeningCount ();
	void LogLinkedElementsCount ();
	void LogCutElementsCount ();
	void LogSuccessfulInputCount ();
	void LogFailedInputCount ();
	
	template <typename Key>
	void LogCounterTable (const GS::HashTable<Key, UInt32>& table, USL::UsageRecord& usageRecord);

	const GS::HashTable<UInt32, UInt32> ConvertCutNumberTypeToUInt32 (const GS::HashTable<OpeningStatisticsStorage::CutNumberType, UInt32>& table);
};


}
}

#endif