/********************************************************************************/
/*																				*/
/*						 ARCHICAD Memory Manager extensions						*/
/*																				*/
/********************************************************************************/

#ifndef	BMX_HPP
#define	BMX_HPP

#pragma once

#include	"GSException.hpp"
#include	"GSDebug.hpp"
#include	"BM.hpp"

namespace GS {

DECLARE_EXCEPTION_CLASS (BMException,			RootException, Error, GSROOT_DLL_EXPORT)

DECLARE_EXCEPTION_CLASS2 (BMLogicErrorException,		BMException, LogicErrorException,		Error, GSROOT_DLL_EXPORT)
DECLARE_EXCEPTION_CLASS2 (BMIllegalArgumentException,	BMException, IllegalArgumentException,	ErrParam, GSROOT_DLL_EXPORT)
	DECLARE_EXCEPTION_CLASS2 (BMNullPointerException,	BMException, NullPointerException,		ErrNilPtr, GSROOT_DLL_EXPORT)
	DECLARE_EXCEPTION_CLASS2 (BMNullHandleException,	BMException, NullHandleException,		ErrNilHandle, GSROOT_DLL_EXPORT)
	DECLARE_EXCEPTION_CLASS2 (BMEmptyHandleException,	BMException, EmptyHandleException,		ErrEmptyHandle, GSROOT_DLL_EXPORT)
DECLARE_EXCEPTION_CLASS2 (BMOutOfMemoryException,		BMException, RunTimeErrorException,	ErrMemoryFull, GSROOT_DLL_EXPORT)
DECLARE_EXCEPTION_CLASS2 (BMMemoryLockedException,		BMException, RunTimeErrorException,	ErrMemLocked, GSROOT_DLL_EXPORT)

#if defined (_MSC_VER)
#pragma warning (disable: 4673)		// the following types will not be considered at the catch site
									// it seems to be a compiler-error to send this warning here...
#endif

inline void RaiseExceptionFromBMError (GSErr err) throw (BMException)
{
	switch (err) {
		case NoError:
			/* why we are here? */
			break;

		case ErrParam:
			throw BMIllegalArgumentException ("BM: illegal argument!");
//			break;

		case ErrMemoryFull:
			throw BMOutOfMemoryException ("BM: cannot allocate the needed memory!");
//			break;

		case ErrNilHandle:
			throw BMNullHandleException ("BM: nullptr handle!");
//			break;

		case ErrMemLocked:
			throw BMMemoryLockedException ("BM: memory locked!");
//			break;

		case ErrNilPtr:
			throw BMNullPointerException ("BM: Null pointer!");
//			break;

		case ErrEmptyHandle:
			throw BMEmptyHandleException ("BM: empty handle!");
//			break;

		default:
			throw BMLogicErrorException ("BMX internal error!");
//			break;
	}
}

inline Int32 BMXCheck (void) throw (BMException)
{
	Int32 ret = BMCheckHeap ();
	GSErr bmError = BMError ();
	if (bmError != NoError) {
		RaiseExceptionFromBMError (bmError);
	}
	return ret;
}


inline GSHandle BMX_hAll (GSSize nBytes, GSFlags flags) throw (BMException)
{
	GSHandle ret = BMAllocateHandle (nBytes, flags, 0);
	if (ret == nullptr) {
		RaiseExceptionFromBMError (BMError ());
	}
	return ret;
}

inline void BMXhFree (GSHandle handle) throw (BMException)
{
	BMKillHandle (&handle);
	GSErr bmError = BMError ();
	if (bmError != NoError) {
		RaiseExceptionFromBMError (bmError);
	}
}

inline void BMXhKill (GSHandle* handle) throw (BMException)
{
	BMKillHandle (handle);
	GSErr bmError = BMError ();
	if (bmError != NoError) {
		RaiseExceptionFromBMError (bmError);
	}
}

inline GSSize BMXhGetSize (GSHandle handle) throw (BMException)
{
	GSSize ret = BMGetHandleSize (handle);
	GSErr bmError = BMError ();
	if (bmError != NoError) {
		RaiseExceptionFromBMError (bmError);
	}	return ret;
}

inline GSHandle BMX_hSetSize (GSHandle handle, GSSize nBytes, GSFlags reallocFlags, short actionCode) throw (BMException)
{
	GSHandle ret = BMReallocHandle (handle, nBytes, reallocFlags, actionCode);
	if (ret == nullptr) {
		RaiseExceptionFromBMError (BMError ());
	}
	return ret;
}

inline GSSize BMXhClear (GSHandle handle) throw (BMException)
{
	GSSize ret = BMClearHandle (handle);
	if (ret == 0) {
		RaiseExceptionFromBMError (BMError ());
	}
	return ret;
}


inline GSPtr BMX_pAll (GSSize nBytes, GSFlags flags) throw (BMException)
{
	GSPtr ret = BMAllocatePtr (nBytes, flags, 0);
	if (ret == nullptr) {
		RaiseExceptionFromBMError (BMError ());
	}
	return ret;
}

inline void BMXpFree (GSPtr ptr) throw (BMException)
{
	BMKillPtr (&ptr);
	GSErr bmError = BMError ();
	if (bmError != NoError) {
		RaiseExceptionFromBMError (bmError);
	}
}

inline void BMXpKill (GSPtr* ptr) throw (BMException)
{
	BMKillPtr (ptr);
	GSErr bmError = BMError ();
	if (bmError != NoError) {
		RaiseExceptionFromBMError (bmError);
	}
}

inline GSSize BMXpGetSize (GSPtr ptr) throw (BMException)
{
	GSSize ret = BMGetPtrSize (ptr);
	if (ret == 0) {
		RaiseExceptionFromBMError (BMError ());
	}
	return ret;
}

inline void BMX_hToHandle (GSHandle ihandle, GSHandle* ohandle) throw (BMException)
{
	GSErr ret = BMHandleToHandle (ihandle, ohandle);
	if (ret != NoError) {
		RaiseExceptionFromBMError (BMError ());
	}
}

inline void BMX_hAndHandle (GSHandle ihandle, GSHandle ohandle) throw (BMException)
{
	GSErr ret = BMHandleAndHandle (ihandle, ohandle);
	if (ret != NoError) {
		RaiseExceptionFromBMError (BMError ());
	}
}

inline void BMX_pToHandle (const GSConstPtr ptr, GSHandle* handle, GSSize nBytes) throw (BMException)
{
	GSErr ret = BMPtrToHandle (ptr, handle, nBytes);
	if (ret != NoError) {
		RaiseExceptionFromBMError (BMError ());
	}
}

inline void BMX_pAndHandle (const GSConstPtr ptr, GSHandle handle, GSSize nBytes) throw (BMException)
{
	GSErr ret = BMPtrAndHandle (ptr, handle, nBytes);
	if (ret != NoError) {
		RaiseExceptionFromBMError (BMError ());
	}
}


#define		BMXhAll(n)					(GS::BMX_hAll (n, 0))
#define		BMXhAllClear(n)				(GS::BMX_hAll (n, ALLOCATE_CLEAR))
#define		BMXhSetSize(h, n)			(GS::BMX_hSetSize (h, n, 0, 0))
#define		BMXhSetSizeClear(h, n)		(GS::BMX_hSetSize (h, n, REALLOC_CLEAR, 0))
#define		BMXhSetSizeFullClear(h, n)	(GS::BMX_hSetSize (h, n, REALLOC_FULLCLEAR, 0))
#define		BMXhReal(h, n)				(GS::BMX_hSetSize (h, n, REALLOC_DISPOSE, 0))
#define		BMXhReal1(h, n)				(GS::BMX_hSetSize (h, n, REALLOC_SHRINK, 0))
#define		BMXhAllSetSizeClear(h, n)	(GS::BMX_hSetSize (h, n, REALLOC_CLEAR, 0))


#define		BMXpAll(n)					(GS::BMX_pAll (n, 0))
#define		BMXpAllClear(n)				(GS::BMX_pAll (n, ALLOCATE_CLEAR))

#define		BMXhToHandle(ih, oh)		(GS::BMX_hToHandle (ih, oh))
#define		BMXpToHandle(p, h, n)		(GS::BMX_pToHandle (p, h, n))
#define		BMXhAndHandle(ih, oh)		(GS::BMX_hAndHandle (ih, oh))
#define		BMXpAndHandle(p, h, n)		(GS::BMX_pAndHandle (p, h, n))



/*------------------------------------------------------------------------------*/
/*					Pointer and handle guard utility classes					*/
/*------------------------------------------------------------------------------*/

class GuardedHandle {
private:
	GSHandle	handle;

	GuardedHandle (const GuardedHandle& source);				// disabled
	GuardedHandle& operator= (const GuardedHandle& source);	// disabled

public:
	inline GuardedHandle (void);
	inline explicit GuardedHandle (GSHandle src);
	inline explicit GuardedHandle (GSSize bytesNum) throw (BMException);
	inline ~GuardedHandle ();

	inline void		Allocate (GSSize bytesNum) throw (BMException);
	inline GSHandle	GetHandle (void);
};


inline GuardedHandle::GuardedHandle (void) : handle (nullptr)
{
}

inline GuardedHandle::GuardedHandle (GSHandle src) : handle (src)
{
}

inline GuardedHandle::GuardedHandle (GSSize bytesNum) throw (BMException) : handle (BMXhAll (bytesNum))
{
}


inline GuardedHandle::~GuardedHandle ()
{
	try {
		BMXhKill (&handle);
	} catch (...) {
	}
}

inline void GuardedHandle::Allocate (GSSize bytesNum) throw (BMException)
{
	if (handle == nullptr) {
		handle = BMXhAll (bytesNum);
	} else {
		DBBREAK ();
	}
}

inline GSHandle GuardedHandle::GetHandle (void)
{
	return handle;
}

}	// namespace GS

#endif
