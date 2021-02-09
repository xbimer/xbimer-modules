
// *****************************************************************************
//
//                               Class FastLogger
//
// Module:			GSRoot
// Namespace:		GS
// Contact person:	MB
//
// SG compatible
//
// *****************************************************************************


#if !defined FASTLOGGER_HPP
#define FASTLOGGER_HPP

#pragma once

// --------------------------------- Includes ----------------------------------

#include "GSTime.hpp"
#include "GSGuid.hpp"
#include "Array.hpp"
#include "ApplicationTermination.hpp"
#include "SpinLock.hpp"



// ------------------------------ Predeclarations ------------------------------

namespace GS { template <class IndexType, class ItemType> class HashTable; }
namespace GS { template <class Type> class HashSet; }



// ============================= Class FastLogger ==============================

namespace GS {

class LogLine;
class EventLogLine;
class DataLogLine;

class GSROOT_DLL_EXPORT FastLogger: private ApplicationTerminationObserver {
public:
	class Control;

private:
	Control*		control;		// controls behavior of the logger
	OChannel*		output;			// output of the logger

	static bool		logDataLines;	// true if data lines should be logged

	HashSet<UInt16>&			logLineCompressionTable;	// stores whether was the log line used in the current block
	HashTable<Guid, UInt32>&	idCompressionTable;			// stores { Guid -> UInt32 } compression codes
	HashTable<UInt64, UInt32>&	stringCompressionTable;		// stores { (String -> UInt64) -> UInt32 } compression codes
	UInt32						compressionCodeGenerator;	// counter to be used at the next compression

	UInt8							nextLogLineIdCircularIndex;		// circular index to be used at the next log line identifier compression
	GS::Array<UInt16>				circularIndexToLogLineId;		// maps circular indices to log line identifiers
	GS::HashTable<UInt16, UInt8>&	logLineIdToCircularIndex;		// maps log line identifiers to circular indices

	UInt8							nextCompressionCodeCircularIndex;	// circular index to be used at the next compression code compression
	GS::Array<UInt32>				circularIndexToCompressionCode;		// maps circular indices to compression codes
	GS::HashTable<UInt32, UInt8>&	compressionCodeToCircularIndex;		// maps compression codes to circular indices

	UInt8							nextThreadIdCircularIndex;		// circular index to be used at the next thread identifier compression
	GS::Array<UInt64>				circularIndexToThreadId;		// maps circular indices to thread identifiers
	GS::HashTable<UInt64, UInt8>&	threadIdToCircularIndex;		// maps thread identifiers to circular indices

	SpinLock	outputBlockLock;		// synchronizes output generation
	IntPtr		threadInOutputBlock;	// thread in the critical section guarded by outputLock
	IntPtr		lastLogThread;			// stores the thread of the last (previous) log

	GSTime		lastTime;	// stores the value of the last (previous) time

	FastLogger (const FastLogger& source);				// disabled
	FastLogger&	operator= (const FastLogger& source);	// disabled

	static void		InitLoggingParameters (void);

	void	StartNewBlockImp (void);

	void	AppendLoggingStarted (void);
	void	AppendLoggingStopped (void);
	void	AppendNewLine (void);

	void	WriteTimeChange   (GSTime time);
	void	WriteThreadChange (UInt64 threadId);

	void	WriteEventLogLine (const char* content, UInt32& idx);
	void	WriteDataLogLine  (const char* content, UInt32& idx);
	void	WriteParameters   (const LogLine* logLine, const char* content, UInt32& idx);

	void	WriteDate (GSTime date);
	void	WriteTime (GSTime time);

	void	CompressLogLine			(UInt16 logLineId, UInt8* circularIndex);
	void	CompressLogLineId		(UInt16 logLineId, UInt8* circularIndex);
	void	CompressId				(const Guid& id, UInt32* compressionCode, UInt8* circularIndex);
	void	CompressStringHashValue (UInt64 stringHashValue, UInt32* compressionCode, UInt8* circularIndex);
	void	CompressCompressionCode	(UInt32 compresssionCode, UInt8* circularIndex);
	void	CompressThreadId		(UInt64 threadId, UInt8* circularIndex);

	static UInt64	GenerateStringHashValue (const char* string, UInt32 length);

	virtual void	SeriousErrorEncountered (const GS::Utf8Char* errorCategoryGuid, const GS::Utf8Char* errorDescriptionGuid, const GS::Utf8Char* errorDescription) override;
	virtual void	ApplicationTerminated	(const GS::Utf8Char* errorCategoryGuid, const GS::Utf8Char* terminateReasonGuid, const GS::Utf8Char* terminateReason, bool applicationError) override;

public:
	class LogComposer;

	FastLogger ();
	virtual ~FastLogger ();

	void					Start	  (Control& control, OChannel& output);
	void					Stop	  (void);
	GS_FORCE_INLINE bool	IsStarted (void) const;

	GSErrCode				Flush (void);
	void					StartNewBlock (void);

	void	Append (EventLogLine& eventLogLine);
	void	Append (EventLogLine& eventLogLine, LogComposer* logComposer);

	template <class P1>
	void	Append (EventLogLine& eventLogLine, const P1& p1);
	template <class P1>
	void	Append (EventLogLine& eventLogLine, const P1& p1, LogComposer* logComposer);

	template <class P1, class P2>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2);
	template <class P1, class P2>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, LogComposer* logComposer);

	template <class P1, class P2, class P3>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3);
	template <class P1, class P2, class P3>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, LogComposer* logComposer);

	template <class P1, class P2, class P3, class P4>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4);
	template <class P1, class P2, class P3, class P4>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, LogComposer* logComposer);

	template <class P1, class P2, class P3, class P4, class P5>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5);
	template <class P1, class P2, class P3, class P4, class P5>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, LogComposer* logComposer);

	template <class P1, class P2, class P3, class P4, class P5, class P6>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6);
	template <class P1, class P2, class P3, class P4, class P5, class P6>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, LogComposer* logComposer);

	template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7);
	template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, LogComposer* logComposer);

	template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8);
	template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	void	Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, LogComposer* logComposer);

		// Types

	class GSROOT_DLL_EXPORT Control {
	public:
		virtual ~Control ();

		virtual void	LoggingStarted (void);
		virtual bool	IsNewBlockNeeded (void) const = 0;
		virtual void	StartNewBlock (void) = 0;

		virtual void	SeriousErrorEncountered (void) = 0;
		virtual void	ApplicationTerminated	(void) = 0;
	};

		// Private methods called back by composers -> must be synchronized!

	void	LogComposed	(char* content, UInt32 size);
};


class GSROOT_DLL_EXPORT FastLogger::LogComposer {
private:
	friend class FastLogger;

	FastLogger*	parent;					// parent of the composer

	LogLine*	logLine;				// current log line under logging
	UInt32		currentParameterIndex;	// index of the current parameter to be appended

	UInt32		dataLogLineCount;		// number of data log lines composed

	char*		logBuffer;				// points to the buffer used to compose the log
	UInt32		logSize;				// size of the log
	UInt32		logBufferCapacity;		// capacity of the log buffer
	enum { FixLogBufferCapacity = 1024 };
	char		FixLogBuffer[FixLogBufferCapacity];	// log buffer of fixed size to avoid frequent memory allocations

	LogComposer (const LogComposer& source);				// disabled
	LogComposer&	operator= (const LogComposer& source);	// disabled

	void	Init (FastLogger* newParent, EventLogLine& newEventLogLine);

	GS_FORCE_INLINE void	Append (const char* string, UInt32 length, UInt64 stringHashValue);

	void	AppendDefault (void);

	GS_FORCE_INLINE void	ComposeLog (const char* content, UInt32 size);
	template <class Type>
	GS_FORCE_INLINE void	ComposeLog (const Type& content);

	void	ReallocateLogBuffer (UInt32 minCapacity);

	void	FinishLog (void);

public:
	LogComposer ();
   ~LogComposer ();

	void	Append (DataLogLine& dataLogLine);

	void	Append (bool   value);
	void	Append (Int16  value);
	void	Append (UInt16 value);
	void	Append (Int32  value);
	void	Append (UInt32 value);
	void	Append (Int64  value);
	void	Append (UInt64 value);

	void	Append (float  value);
	void	Append (double value);

	void	Append (char value);

	void	Append (const char* string);
	void	Append (const char* string, UInt32 length);
	void	AppendStringWithConstMemoryAddress (const char* string);

	void	Append (const Guid& id);

	void	AppendDate (GSTime date);
	void	AppendTime (GSTime time);

	template <class P1>
	void	Append (DataLogLine& dataLogLine, const P1& p1);

	template <class P1, class P2>
	void	Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2);

	template <class P1, class P2, class P3>
	void	Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3);

	template <class P1, class P2, class P3, class P4>
	void	Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4);

	template <class P1, class P2, class P3, class P4, class P5>
	void	Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5);

	template <class P1, class P2, class P3, class P4, class P5, class P6>
	void	Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6);

	template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	void	Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7);

	template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	void	Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8);
};


GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, bool	 value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, Int16  value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, UInt16 value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, Int32  value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, UInt32 value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, Int64  value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, UInt64 value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, float  value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, double value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, char value);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, const char* string);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, const Guid& id);
GS_FORCE_INLINE void	Append (FastLogger::LogComposer& logComposer, const ClassInfo* classInfo);

template <class P1>
GS_FORCE_INLINE void	Append1 (FastLogger::LogComposer& logComposer, const P1& p1);

template <class P1, class P2>
GS_FORCE_INLINE void	Append2 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2);

template <class P1, class P2, class P3>
GS_FORCE_INLINE void	Append3 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3);

template <class P1, class P2, class P3, class P4>
GS_FORCE_INLINE void	Append4 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4);

template <class P1, class P2, class P3, class P4, class P5>
GS_FORCE_INLINE void	Append5 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5);

template <class P1, class P2, class P3, class P4, class P5, class P6>
GS_FORCE_INLINE void	Append6 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6);

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
GS_FORCE_INLINE void	Append7 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7);

template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
GS_FORCE_INLINE void	Append8 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8);


GSROOT_DLL_EXPORT extern FastLogger	logger;


class GSROOT_DLL_EXPORT LogLine {
public:
	struct Parameter {
		enum Type { Int16, UInt16, Int32, UInt32, Int64, UInt64, Float, Double, Char, String, Id, Date, Time, Unknown };

		Type	type;			// type of the parameter
		bool	isCompressed;	// true if the parameter is compressed

		static const char*		GetTypeText (Type type);
	};

	enum Level {
		Trace	 = 1000,	// control path dependent information or event
		Info	 = 2000,	// control path independent information or event
		Warning	 = 3000,	// something can be wrong but it can't be determined in the current context
		Error	 = 4000,	// something is wrong but the application can continue to run
		Fatal	 = 5000		// something is wrong and the application can't continue to run -> it will be terminated
	};

private:
	UInt16					id;					// run time unique identifier of the log line
	ArrayFB<Parameter, 4>	parameters;			// parameters of the log line
	const char*				formatString;		// format string of the log line

	static volatile Int32	idGenerator;		// counter used to generate run time identifiers for log lines

	static void			SkipWhiteSpace	 (const char* parameterString, UInt32 length, UInt32& index);
	static GSErrCode	ParseType		 (const char* parameterString, UInt32 length, UInt32& index, Parameter::Type* type);
	static GSErrCode	ParseCompression (const char* parameterString, UInt32 length, UInt32& index, char* compression);
	static bool			ParseString		 (const char* parameterString, UInt32 length, UInt32& index, const char* string);

	static UInt32	CountParameters (const char* formatString);

protected:
	LogLine (const char* formatString);
	LogLine (const char* formatString, const char* parameterString);
   ~LogLine ();

public:
	inline UInt16			GetId (void) const;

	inline const Parameter&	GetParamater	  (UInt32 index) const;
	inline UInt32			GetParamaterCount (void) const;

	inline bool				CheckParameterType   (UInt32 index, Parameter::Type type) const;
	inline bool				CollateParameterType (UInt32 index, Parameter::Type type);

	inline const char*		GetFormatString (void) const;

	struct ParameterTypes {
		const char* parameterString;

		explicit ParameterTypes (const char* parameterString): parameterString (parameterString) {}
	};
};


class GSROOT_DLL_EXPORT EventLogLine: public LogLine {
private:
	Level			level;		// level of the log line
	const char*		owner;		// owner of the log line
	const char*		metaData;	// meta data attached to the log line

public:
	EventLogLine (const char* formatString, Level level, const char* owner = nullptr, const char* metaData = nullptr);
	EventLogLine (const char* formatString, const ParameterTypes& parameterTypes, Level level, const char* owner = nullptr, const char* metaData = nullptr);
   ~EventLogLine ();

	inline Level		GetLevel  (void) const;

	inline const char*	GetOwner  (void) const;
	inline bool			HasOwner  (void) const;

	inline const char*	GetMetaData  (void) const;
	inline bool			HasMetaData  (void) const;
};


class GSROOT_DLL_EXPORT TraceLogLine: public EventLogLine {
public:
	TraceLogLine (const char* formatString, const char* owner = nullptr, const char* metaData = nullptr);
	TraceLogLine (const char* formatString, const ParameterTypes& parameterTypes, const char* owner = nullptr, const char* metaData = nullptr);
};


class GSROOT_DLL_EXPORT InfoLogLine: public EventLogLine {
public:
	InfoLogLine (const char* formatString, const char* owner = nullptr, const char* metaData = nullptr);
	InfoLogLine (const char* formatString, const ParameterTypes& parameterTypes, const char* owner = nullptr, const char* metaData = nullptr);
};


class GSROOT_DLL_EXPORT WarningLogLine: public EventLogLine {
public:
	WarningLogLine (const char* formatString, const char* owner = nullptr, const char* metaData = nullptr);
	WarningLogLine (const char* formatString, const ParameterTypes& parameterTypes, const char* owner = nullptr, const char* metaData = nullptr);
};


class GSROOT_DLL_EXPORT ErrorLogLine: public EventLogLine {
public:
	ErrorLogLine (const char* formatString, const char* owner = nullptr, const char* metaData = nullptr);
	ErrorLogLine (const char* formatString, const ParameterTypes& parameterTypes, const char* owner = nullptr, const char* metaData = nullptr);
};


class GSROOT_DLL_EXPORT FatalLogLine: public EventLogLine {
public:
	FatalLogLine (const char* formatString, const char* owner = nullptr, const char* metaData = nullptr);
	FatalLogLine (const char* formatString, const ParameterTypes& parameterTypes, const char* owner = nullptr, const char* metaData = nullptr);
};


class GSROOT_DLL_EXPORT DataLogLine: public LogLine {
public:
	DataLogLine (const char* formatString);
	DataLogLine (const char* formatString, const ParameterTypes& parameterTypes);
};


class GSROOT_DLL_EXPORT ScopeLogger {
private:
	FastLogger*	  logger;			// logger to be used for the scope enter-exit logging
	TraceLogLine* scopeExitedLog;	// log definition to be used when the scope is exited

public:
	 ScopeLogger (FastLogger& logger, TraceLogLine& scopeEnteredLog, TraceLogLine& scopeExitedLog);
	~ScopeLogger ();
};

}	// namespace GS



GS_FORCE_INLINE bool	GS::FastLogger::IsStarted (void) const
{
	return (control != nullptr);
}


template <class P1>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1)
{
	LogComposer logComposer;
	Append (eventLogLine, &logComposer);

	GS::Append1 (logComposer, p1);
}


template <class P1>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, LogComposer* logComposer)
{
	Append (eventLogLine, logComposer);

	GS::Append1 (*logComposer, p1);
}


template <class P1, class P2>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2)
{
	LogComposer logComposer;
	Append (eventLogLine, &logComposer);

	GS::Append2 (logComposer, p1, p2);
}


template <class P1, class P2>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, LogComposer* logComposer)
{
	Append (eventLogLine, logComposer);

	GS::Append2 (*logComposer, p1, p2);
}


template <class P1, class P2, class P3>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3)
{
	LogComposer logComposer;
	Append (eventLogLine, &logComposer);

	GS::Append3 (logComposer, p1, p2, p3);
}


template <class P1, class P2, class P3>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, LogComposer* logComposer)
{
	Append (eventLogLine, logComposer);

	GS::Append3 (*logComposer, p1, p2, p3);
}


template <class P1, class P2, class P3, class P4>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	LogComposer logComposer;
	Append (eventLogLine, &logComposer);

	GS::Append4 (logComposer, p1, p2, p3, p4);
}


template <class P1, class P2, class P3, class P4>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, LogComposer* logComposer)
{
	Append (eventLogLine, logComposer);

	GS::Append4 (*logComposer, p1, p2, p3, p4);
}


template <class P1, class P2, class P3, class P4, class P5>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
	LogComposer logComposer;
	Append (eventLogLine, &logComposer);

	GS::Append5 (logComposer, p1, p2, p3, p4, p5);
}


template <class P1, class P2, class P3, class P4, class P5>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, LogComposer* logComposer)
{
	Append (eventLogLine, logComposer);

	GS::Append5 (*logComposer, p1, p2, p3, p4, p5);
}


template <class P1, class P2, class P3, class P4, class P5, class P6>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
{
	LogComposer logComposer;
	Append (eventLogLine, &logComposer);

	GS::Append6 (logComposer, p1, p2, p3, p4, p5, p6);
}


template <class P1, class P2, class P3, class P4, class P5, class P6>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, LogComposer* logComposer)
{
	Append (eventLogLine, logComposer);

	GS::Append6 (*logComposer, p1, p2, p3, p4, p5, p6);
}


template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
{
	LogComposer logComposer;
	Append (eventLogLine, &logComposer);

	GS::Append7 (logComposer, p1, p2, p3, p4, p5, p6, p7);
}


template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, LogComposer* logComposer)
{
	Append (eventLogLine, logComposer);

	GS::Append7 (*logComposer, p1, p2, p3, p4, p5, p6, p7);
}


template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8)
{
	LogComposer logComposer;
	Append (eventLogLine, &logComposer);

	GS::Append8 (logComposer, p1, p2, p3, p4, p5, p6, p7, p8);
}


template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
void	GS::FastLogger::Append (EventLogLine& eventLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, LogComposer* logComposer)
{
	Append (eventLogLine, logComposer);

	GS::Append8 (*logComposer, p1, p2, p3, p4, p5, p6, p7, p8);
}


template <class P1>
void	GS::FastLogger::LogComposer::Append (DataLogLine& dataLogLine, const P1& p1)
{
	Append (dataLogLine);

	GS::Append1 (*this, p1);
}


template <class P1, class P2>
void	GS::FastLogger::LogComposer::Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2)
{
	Append (dataLogLine);

	GS::Append2 (*this, p1, p2);
}


template <class P1, class P2, class P3>
void	GS::FastLogger::LogComposer::Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3)
{
	Append (dataLogLine);

	GS::Append3 (*this, p1, p2, p3);
}


template <class P1, class P2, class P3, class P4>
void	GS::FastLogger::LogComposer::Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	Append (dataLogLine);

	GS::Append4 (*this, p1, p2, p3, p4);
}


template <class P1, class P2, class P3, class P4, class P5>
void	GS::FastLogger::LogComposer::Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
	Append (dataLogLine);

	GS::Append5 (*this, p1, p2, p3, p4, p5);
}


template <class P1, class P2, class P3, class P4, class P5, class P6>
void	GS::FastLogger::LogComposer::Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
{
	Append (dataLogLine);

	GS::Append6 (*this, p1, p2, p3, p4, p5, p6);
}


template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
void	GS::FastLogger::LogComposer::Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
{
	Append (dataLogLine);

	GS::Append7 (*this, p1, p2, p3, p4, p5, p6, p7);
}


template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
void	GS::FastLogger::LogComposer::Append (DataLogLine& dataLogLine, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8)
{
	Append (dataLogLine);

	GS::Append8 (*this, p1, p2, p3, p4, p5, p6, p7, p8);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, bool	 value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, Int16  value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, UInt16 value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, Int32  value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, UInt32 value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, Int64  value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, UInt64 value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, float  value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, double value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, char value)
{
	logComposer.Append (value);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, const char* string)
{
	logComposer.Append (string);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, const Guid& id)
{
	logComposer.Append (id);
}


GS_FORCE_INLINE void	GS::Append (FastLogger::LogComposer& logComposer, const GS::ClassInfo* classInfo)
{
	if (classInfo != nullptr)
		logComposer.AppendStringWithConstMemoryAddress (classInfo->GetName ());
	else
		logComposer.AppendStringWithConstMemoryAddress ("UnknownClass");
}


template <class P1>
GS_FORCE_INLINE void	GS::Append1 (FastLogger::LogComposer& logComposer, const P1& p1)
{
	Append (logComposer, p1);
}


template <class P1, class P2>
GS_FORCE_INLINE void	GS::Append2 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2)
{
	Append (logComposer, p1);
	Append (logComposer, p2);
}


template <class P1, class P2, class P3>
GS_FORCE_INLINE void	GS::Append3 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3)
{
	Append (logComposer, p1);
	Append (logComposer, p2);
	Append (logComposer, p3);
}


template <class P1, class P2, class P3, class P4>
GS_FORCE_INLINE void	GS::Append4 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	Append (logComposer, p1);
	Append (logComposer, p2);
	Append (logComposer, p3);
	Append (logComposer, p4);
}


template <class P1, class P2, class P3, class P4, class P5>
GS_FORCE_INLINE void	GS::Append5 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
{
	Append (logComposer, p1);
	Append (logComposer, p2);
	Append (logComposer, p3);
	Append (logComposer, p4);
	Append (logComposer, p5);
}


template <class P1, class P2, class P3, class P4, class P5, class P6>
GS_FORCE_INLINE void	GS::Append6 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
{
	Append (logComposer, p1);
	Append (logComposer, p2);
	Append (logComposer, p3);
	Append (logComposer, p4);
	Append (logComposer, p5);
	Append (logComposer, p6);
}


template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
GS_FORCE_INLINE void	GS::Append7 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
{
	Append (logComposer, p1);
	Append (logComposer, p2);
	Append (logComposer, p3);
	Append (logComposer, p4);
	Append (logComposer, p5);
	Append (logComposer, p6);
	Append (logComposer, p7);
}


template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
GS_FORCE_INLINE void	GS::Append8 (FastLogger::LogComposer& logComposer, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8)
{
	Append (logComposer, p1);
	Append (logComposer, p2);
	Append (logComposer, p3);
	Append (logComposer, p4);
	Append (logComposer, p5);
	Append (logComposer, p6);
	Append (logComposer, p7);
	Append (logComposer, p8);
}


inline UInt16	GS::LogLine::GetId (void) const
{
	return id;
}


inline const GS::LogLine::Parameter&	GS::LogLine::GetParamater (UInt32 index) const
{
	return parameters[index];
}


inline UInt32	GS::LogLine::GetParamaterCount (void) const
{
	return parameters.GetSize ();
}


inline bool		GS::LogLine::CheckParameterType (UInt32 index, Parameter::Type type) const
{
	if (index >= parameters.GetSize ())
		return false;

	return (parameters[index].type == type);
}


inline bool		GS::LogLine::CollateParameterType (UInt32 index, Parameter::Type type)
{
	if (index >= parameters.GetSize ())
		return false;

	Parameter& parameter = parameters[index];
	if (parameter.type == Parameter::Unknown) {
		parameter.type = type;
		if (type == Parameter::Id || type == Parameter::String)
			parameter.isCompressed = true;
	}

	return (parameter.type == type);
}


inline const char*	GS::LogLine::GetFormatString (void) const
{
	return formatString;
}


inline GS::LogLine::Level	GS::EventLogLine::GetLevel (void) const
{
	return level;
}


inline const char*	GS::EventLogLine::GetOwner (void) const
{
	return owner;
}


inline bool		GS::EventLogLine::HasOwner (void) const
{
	return (owner != nullptr);
}


inline const char*	GS::EventLogLine::GetMetaData (void) const
{
	return metaData;
}


inline bool		GS::EventLogLine::HasMetaData (void) const
{
	return (metaData != nullptr);
}
// _____________________________ Class FastLogger ______________________________


#endif
