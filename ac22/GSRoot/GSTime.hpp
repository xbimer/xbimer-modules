// *********************************************************************************************************************
// Description:		GRAPHISOFT Time Manager
//
// Namespace:		-
// Contact person:	MM
//
// [SG compatible]
// *********************************************************************************************************************

#ifndef	GSTIME_HPP
#define	GSTIME_HPP

#pragma once

// --- Includes	--------------------------------------------------------------------------------------------------------

// from GRoot
#include	"Definitions.hpp"
#include	"GSRootExport.hpp"
#include	"IV.hpp"
#include	"GSDebug.hpp"

// --- Predeclarations	------------------------------------------------------------------------------------------------

namespace GS {
	class UniString;
}


// =====================================================================================================================
// Manager Identifiers
// =====================================================================================================================

#define	IDClockManager			'CK01'
#define	IDTimer01Manager		'TP01'
#define	IDTimerManager			'TP02'
#define	IDGlobalTimerManager	'TI01'
#define	IDDateTimeManager		'DT01'


// =====================================================================================================================
// Type definitions
// =====================================================================================================================

typedef	UInt32	GSTicks;
typedef	Int32	GSTime;
typedef	Int64	GSFineTicks;

struct GSTimerData {
	char	data [64];
};

struct GSTimeRecord {
	unsigned short	year;
	unsigned short	month;
	unsigned short	dayOfWeek;		// 0 = Sunday
	unsigned short	day;
	unsigned short	hour;
	unsigned short	minute;
	unsigned short	second;
	unsigned short	milliSecond;

	GSTimeRecord () : 
		year (0),
		month (0),
		dayOfWeek (0),
		day (0),
		hour (0),
		minute (0),
		second (0),
		milliSecond (0)
	{
	}

	GSTimeRecord (unsigned short y,
				  unsigned short m,
				  unsigned short dow,
				  unsigned short d,
				  unsigned short h,
				  unsigned short mi,
				  unsigned short s,
				  unsigned short ms) :
		year (y),
		month (m),
		dayOfWeek (dow),
		day (d),
		hour (h),
		minute (mi),
		second (s),
		milliSecond (ms)
	{
	}

	bool	operator == (const GSTimeRecord& rOp) const
	{
		return	year == rOp.year &&
				month == rOp.month &&
				day == rOp.day &&
				DBVERIFY (dayOfWeek == rOp.dayOfWeek) &&
				hour == rOp.hour &&
				minute == rOp.minute &&
				second == rOp.second &&
				milliSecond == rOp.milliSecond;
	}

	bool	operator != (const GSTimeRecord& rOp) const
	{
		return !(*this == rOp);
	}
};


struct GSDateRecord {
	unsigned short	year;
	unsigned short	month;
	unsigned short	day;

	GSDateRecord () :
		year (0),
		month (0),
		day (0)
	{
	}

	GSDateRecord (unsigned short y,
				  unsigned short m,
				  unsigned short d) :
		year (y),
		month (m),
		day (d)
	{
	}

	bool	operator == (const GSDateRecord& rOp) const
	{
		return	year == rOp.year &&
			month == rOp.month &&
			day == rOp.day;
	}

	bool	operator != (const GSDateRecord& rOp) const
	{
		return !(*this == rOp);
	}
};


// =====================================================================================================================
// Byte-order conversion of the defined types
// =====================================================================================================================

#define	IVGSTicks	IVULong
#define	IVGSTime	IVLong


// =====================================================================================================================
// CLOCK MANAGER
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
// Interface definition
// ---------------------------------------------------------------------------------------------------------------------

struct IClockManager {
	GSTicks			(STDCALL* getTicks) (void);
	GSTicks			(STDCALL* getTicksPerSec) (void);
	GSTicks			(STDCALL* getDoubleClickTicks) (void);
	GSFineTicks		(STDCALL* getFineTicks) (void);
	GSFineTicks		(STDCALL* getFineTicksPerSec) (void);
	void			(STDCALL* wait) (double delaySeconds);
};

EXTERNC GSROOT_DLL_EXPORT IClockManager&	clockManager;


// ---------------------------------------------------------------------------------------------------------------------
// Inline helper funtions
// ---------------------------------------------------------------------------------------------------------------------

inline	GSTicks			TIGetTicks (void)
							{	return  clockManager.getTicks ();				}

inline	GSTicks			TIGetTicksPerSec (void)
							{	return  clockManager.getTicksPerSec ();		}

inline	GSTicks			TIGetDoubleClickTicks (void)
							{	return  clockManager.getDoubleClickTicks ();	}

inline	GSFineTicks		TIGetFineTicks (void)
							{	return  clockManager.getFineTicks ();			}

inline	GSFineTicks		TIGetFineTicksPerSec (void)
							{	return  clockManager.getFineTicksPerSec ();	}

inline	void			TIWait (double delaySeconds = 1.0)
							{	clockManager.wait (delaySeconds);				}

#define		TicksPerSecond			(clockManager.getTicksPerSec ())
#define		FineTicksPerSecond		(clockManager.getFineTicksPerSec ())


// =====================================================================================================================
// TIMER MANAGER
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
// Interface definition
// ---------------------------------------------------------------------------------------------------------------------

struct ITimerManager {
	void		(STDCALL* reset) (GSTimerData* pTimerData, const char *name, bool startTimer);
	void		(STDCALL* start) (GSTimerData* pTimerData);
	double		(STDCALL* stop) (GSTimerData* pTimerData);
	bool		(STDCALL* run) (const GSTimerData* pTimerData);
	const char*	(STDCALL* getName) (const GSTimerData* pTimerData);
	double		(STDCALL* getValue) (const GSTimerData* pTimerData);
	void		(STDCALL* print) (const GSTimerData* pTimerData);
	Int32		(STDCALL* getCounter) (const GSTimerData* pTimerData);
	void		(STDCALL* printRel) (const GSTimerData* pTimerData, const GSTimerData* pRef);
};

EXTERNC GSROOT_DLL_EXPORT ITimerManager&	timerManager;

// ---------------------------------------------------------------------------------------------------------------------
// Inline helper functions
// ---------------------------------------------------------------------------------------------------------------------

class GSTimer {			//lint -sem(GSTimer::reset,initializer)
	GSTimerData	timerData;

public:
	GSTimer (void)									{	reset ();												}
	explicit GSTimer (const char* pName)			{	timerManager.reset (&timerData, pName, false);			}
	void		reset (void)						{	timerManager.reset (&timerData, name (), false);		}
	void		start (void)						{	timerManager.start (&timerData);						}
	double		stop (void)							{	return  timerManager.stop (&timerData);					}
	bool		run (void) const					{	return  timerManager.run (&timerData);					}
	const char*	name (void)	const					{	return  timerManager.getName (&timerData);				}
	double		value (void) const					{	return  timerManager.getValue (&timerData);				}
	void		print (void) const					{	timerManager.print (&timerData);						}
	Int32		counter (void) const				{	return  timerManager.getCounter (&timerData);			}
	void		printRel (const GSTimer* pRef = nullptr) const {
														timerManager.printRel (&timerData, pRef == nullptr ?
															nullptr : &pRef->timerData);
													}
};

/* The following inline functions exist only for compatibility with C source	*/


#if defined (DEBUVERS) || defined (USE_TIMERS)
	#define	TPRESET(t)				(t)->reset ()
	#define	TPSTART(t)				(t)->start ()
	#define	TPSTOP(t)				(t)->stop ()
	#define	TPRUN(t)				(t)->run ()
	#define	TPGETNAME(t)			(t)->name ()
	#define	TPGETVALUE(t)			(t)->value ()
	#define	TPGETCOUNTER(t)			(t)->counter ()
	#define	TPPRINTVALUE(t)			(t)->print ()
	#define	TPPRINTRELVALUES(t,r)	(t)->printRel (r)
#else
	#define	TPRESET(t)
	#define	TPSTART(t)
	#define	TPSTOP(t)				0
	#define	TPRUN(t)				false
	#define	TPGETNAME(t)			""
	#define	TPGETVALUE(t)			0
	#define	TPGETCOUNTER(t)
	#define	TPPRINTVALUE(t)
	#define	TPPRINTRELVALUES(t,r)
#endif

// =====================================================================================================================
// GLOBAL TIMER MANAGER
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
// Interface definition
// ---------------------------------------------------------------------------------------------------------------------

struct IGlobalTimerManager {
	void		(STDCALL* init) (void);
	void		(STDCALL* exit) (void);
	void		(STDCALL* reset) (Int32 timerID, const char *name);
	void		(STDCALL* start) (Int32 timerID);
	double		(STDCALL* stop) (Int32 timerID);
	double		(STDCALL* getValue) (Int32 timerID);
	void		(STDCALL* printTimers) (void);
};

EXTERNC GSROOT_DLL_EXPORT IGlobalTimerManager&	globalTimerManager;

// ---------------------------------------------------------------------------------------------------------------------
// Inline helper funtions
// ---------------------------------------------------------------------------------------------------------------------

inline	void	TIInit (void)
					{	globalTimerManager.init ();						}

inline	void	TIExit (void)
					{	globalTimerManager.exit ();						}

inline	void	TIReset (Int32 timerID, const char *name)
					{	globalTimerManager.reset (timerID, name);			}

inline	void	TIStart (Int32 timerID)
					{	globalTimerManager.start (timerID);				}

inline	double	TIStop (Int32 timerID)
					{	return  globalTimerManager.stop (timerID);		}

inline	double	TIGetValue (Int32 timerID)
					{	return  globalTimerManager.getValue (timerID);	}

inline	void	TIPrintTimers (void)
					{	globalTimerManager.printTimers ();				}

#if defined (DEBUVERS) || defined (USE_TIMERS)
	#define	TIINIT			TIInit()
	#define	TIEXIT			TIExit()
	#define	TIRESET(i, s)	TIReset((i), (s))
	#define	TISTART(i)		TIStart((i))
	#define	TISTOP(i)		TIStop((i))
	#define	TIGETVALUE(i)	TIGetValue((i))
	#define	TIPRINTTIMERS	TIPrintTimers()
#else
	#define	TIINIT
	#define	TIEXIT
	#define	TIRESET(i, s)
	#define	TISTART(i)
	#define	TISTOP(i)
	#define	TIGETVALUE(i)
	#define	TIPRINTTIMERS
#endif


// =====================================================================================================================
// DATE & TIME MANAGER
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
// Interface definition
// ---------------------------------------------------------------------------------------------------------------------

#define	TI_LOCAL_TIME			0x0000	/* Local time							*/
#define	TI_UTC_TIME				0x0001	/* UTC time								*/
#define	TI_CURRENT_TIME			0x0002	/* Current time (UTC or local)			*/

#define	TI_DEFAULT_DATE_FORMAT	0x0000	/* System default date format			*/
#define	TI_SHORT_DATE_FORMAT	0x0010	/* Short date format					*/
#define	TI_LONG_DATE_FORMAT		0x0020	/* Long date format						*/
#define	TI_NO_DATE				0x0080	/* Date is supressed					*/

#define	TI_DEFAULT_TIME_FORMAT	0x0000	/* System default time format			*/
#define	TI_SHORT_TIME_FORMAT	0x0100	/* Short time format					*/
#define	TI_LONG_TIME_FORMAT		0x0200	/* Long time format						*/
#define	TI_NO_TIME				0x0800	/* Time is supressed					*/

// ---------------------------------------------------------------------------------------------------------------------
// Inline helper funtions
// ---------------------------------------------------------------------------------------------------------------------

GSROOT_DLL_EXPORT GSTime					GSCALL TIGetTime (void);
GSROOT_DLL_EXPORT UInt32				GSCALL TIGetMacTime (void);
GSROOT_DLL_EXPORT Int32						GSCALL TIGetWinTime (void);
GSROOT_DLL_EXPORT GSErr						GSCALL TIMac2GSTime (UInt32 macTime, GSTime* pGSTime);
GSROOT_DLL_EXPORT GSErr						GSCALL TIGS2MacTime (GSTime gsTime, UInt32* pMacTime);
GSROOT_DLL_EXPORT GSErr						GSCALL TIWin2GSTime (Int32 winTime, GSTime* pGSTime);
GSROOT_DLL_EXPORT GSErr						GSCALL TIGS2WinTime (GSTime gsTime, Int32* pWinTime);
GSROOT_DLL_EXPORT GSErr						GSCALL TIGetGSTime (const GSTimeRecord* pTimeRecord, GSTime* pGSTime, GSFlags formatFlags = 0);
GSROOT_DLL_EXPORT GSErr						GSCALL TIGetTimeRecord (GSTime uct, GSTimeRecord* pTimeRecord, GSFlags formatFlags = 0);
GSROOT_DLL_EXPORT GS::UniString				GSCALL TIGetTimeString (GSTime uct, GSFlags formatFlags = 0);
GSROOT_DLL_EXPORT Int32						GSCALL TIGetTimeString (GSTime uct, char* pTimeString, GSFlags formatFlags = 0);
GSROOT_DLL_EXPORT GS::UniString				GSCALL TIGetISO8601TimeString (GSTime gsTime, bool UTCTime = false);
GSROOT_DLL_EXPORT Int32						GSCALL TIGetISO8601TimeString (GSTime gsTime, char* pTimeString, bool UTCTime = false);

#if defined(macintosh)
GSROOT_DLL_EXPORT double					GSCALL TIGS2CFAbsoluteTime (GSTime gsTime);
GSROOT_DLL_EXPORT GSTime					GSCALL TICFAbsolute2GSTime (double cfTime);
#endif

#if defined (macintosh)
#define		GSPlatformTime			UInt32
#define		TIGetPlatformTime		TIGetMacTime
#define		TIGS2PlatformTime		TIGS2MacTime
#define		TIPlatform2GSTime		TIMac2GSTime


#else

#define		GSPlatformTime			Int32
#define		TIGetPlatformTime		TIGetWinTime
#define		TIGS2PlatformTime		TIGS2WinTime
#define		TIPlatform2GSTime		TIWin2GSTime

#endif

GSROOT_DLL_EXPORT void	GSCALL IVPlatformTime (GS::PlatformSign platform, GSPlatformTime* pPlatformTime);

/*------------------------------------------------------------------------------*/
/*								Function table initialization					*/
/*------------------------------------------------------------------------------*/

void	InitTimeManager (void);

#endif	/* GSTIME_H */
