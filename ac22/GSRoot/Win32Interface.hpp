// *****************************************************************************
// File:			Win32Interface.hpp
//
// Description:		Includes Windows.h and covers Windows macros
//
// Namespace:		-
//
// Contact person:	BM, MB
//
// SG compatible
// *****************************************************************************

#if !defined (WIN32INTERFACE_HPP)
#define	WIN32INTERFACE_HPP

#pragma once


// -- Includes -----------------------------------------------------------------

#pragma warning (push)
#pragma warning (disable: 4115 4201 4214 4514 4995)

#pragma warning (disable: 6509)	// Error in the Windows 10.0.12040.0 SDK; fixed in later SDKs, can be removed if we upgrade

#if !defined (WIN32_LEAN_AND_MEAN)
	#define	WIN32_LEAN_AND_MEAN
#endif

//Done in the compiler options:
//#if defined (_MSC_VER) && !defined (_WIN32_WINNT)
//#define _WIN32_WINNT 0x0501							// Support Windows Vista or higher
//#endif

#if defined (WINDOWS)
#	include <windows.h>
#endif

#undef WIN32_LEAN_AND_MEAN


// -- Definitions --------------------------------------------------------------

// --- Inline functions covering Windows macros --------------------------------

// ... CreateFile ..............................................................

#if defined (CreateFile)

inline HANDLE	Win_CreateFile (LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
								DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return CreateFile (lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
					   dwFlagsAndAttributes, hTemplateFile);
}

#undef CreateFile

inline HANDLE	CreateFile (LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
							LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
							DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return Win_CreateFile (lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
						   dwFlagsAndAttributes, hTemplateFile);
}

#endif


// ... DeleteFile ..............................................................

#if defined (DeleteFile)

inline BOOL	Win_DeleteFile (LPCTSTR lpFileName)
{
	return DeleteFile (lpFileName);
}

#undef DeleteFile

inline BOOL	DeleteFile (LPCTSTR lpFileName)
{
	return Win_DeleteFile (lpFileName);
}

#endif


// ... CreateWindow ............................................................

#if defined (CreateWindow)

inline HWND		Win_CreateWindow (LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth,
								  int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	return CreateWindow (lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance,
						 lpParam);
}

#undef CreateWindow

inline HWND		CreateWindow (LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth,
							  int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	return Win_CreateWindow (lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance,
							 lpParam);
}

#endif


// ... PostMessage .............................................................

#if defined (PostMessage)

inline BOOL		Win_PostMessage (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return PostMessage (hWnd, msg, wParam, lParam);
}

#undef PostMessage

inline BOOL		PostMessage (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return Win_PostMessage (hWnd, msg, wParam, lParam);
}

#endif


// ... InsertMenu ..............................................................

#if defined (InsertMenu)

inline BOOL		Win_InsertMenu (HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
{
	return InsertMenu (hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
}

#undef InsertMenu

inline BOOL		InsertMenu (HMENU hMenu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCTSTR lpNewItem)
{
	return Win_InsertMenu (hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
}

#endif


// ... InsertMenuItem ..........................................................

#if defined (InsertMenuItem)

inline BOOL		Win_InsertMenuItem (HMENU hMenu, UINT uItem, BOOL fByPosition, LPCMENUITEMINFO lpmii)
{
	return InsertMenuItem (hMenu, uItem, fByPosition, lpmii);
}

#undef InsertMenuItem

inline BOOL		InsertMenuItem (HMENU hMenu, UINT uItem, BOOL fByPosition, LPCMENUITEMINFO lpmii)
{
	return Win_InsertMenuItem (hMenu, uItem, fByPosition, lpmii);
}

#endif


// ... ChangeMenu ..............................................................

#if defined (ChangeMenu)

inline BOOL		Win_ChangeMenu (HMENU hMenu, UINT cmd, LPCTSTR lpszNewItem, UINT cmdInsert, UINT flags)
{
	return ChangeMenu (hMenu, cmd, lpszNewItem, cmdInsert, flags);
}

#undef ChangeMenu

inline BOOL		ChangeMenu (HMENU hMenu, UINT cmd, LPCTSTR lpszNewItem, UINT cmdInsert, UINT flags)
{
	return Win_ChangeMenu (hMenu, cmd, lpszNewItem, cmdInsert, flags);
}

#endif


// ... GetFreeSpace ............................................................

#if defined (GetFreeSpace)
	#undef GetFreeSpace
#endif

#if defined (max)
	#undef max
	template <class Type> inline Type max (Type a, Type b) { return (a > b ? a : b); }
#endif

#if defined (min)
	#undef min
	template <class Type> inline Type min (Type a, Type b) { return (a < b ? a : b); }
#endif


// ... GetMessage ..............................................................

#if defined (GetMessage)

inline BOOL		Win_GetMessage (LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	return  GetMessage (lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

#undef	GetMessage

inline BOOL		GetMessage (LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax)
{
	return  Win_GetMessage (lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

#endif


// ... GetClassInfo ............................................................

#if defined (GetClassInfo)

inline BOOL		Win_GetClassInfo (HINSTANCE hInstance, LPCTSTR lpClassName, LPWNDCLASS lpWndClass)
{
	return  GetClassInfo (hInstance, lpClassName, lpWndClass);
}

#undef	GetClassInfo

inline BOOL		GetClassInfo (HINSTANCE hInstance, LPCTSTR lpClassName, LPWNDCLASS lpWndClass)
{
	return  Win_GetClassInfo (hInstance, lpClassName, lpWndClass);
}

#endif


// ... GetClassName ............................................................

#if defined (GetClassName)

inline BOOL		Win_GetClassName (HWND hWnd, LPTSTR lpClassName, int nMaxCount)
{
	return  GetClassName (hWnd, lpClassName, nMaxCount);
}

#undef	GetClassName

inline BOOL		GetClassName (HWND hWnd, LPTSTR lpClassName, int nMaxCount)
{
	return  Win_GetClassName (hWnd, lpClassName, nMaxCount);
}

#endif


// ... GetUserName ............................................................

#if defined (GetUserName)

inline BOOL		Win_GetUserName (LPTSTR lpBuffer, LPDWORD pcbBuffer)
{
	return GetUserName (lpBuffer, pcbBuffer);
}

#undef	GetUserName

inline BOOL		GetUserName (LPTSTR lpBuffer, LPDWORD pcbBuffer)
{
	return Win_GetUserName (lpBuffer, pcbBuffer);
}

#endif


// ... SetPort ............................................................

#if defined (SetPort)

inline BOOL		Win_SetPort (LPTSTR pName, LPTSTR pPortName, DWORD dwLevel, LPBYTE pPortInfo)
{
	return SetPort (pName, pPortName, dwLevel, pPortInfo);
}

#undef	SetPort

inline BOOL		SetPort (LPTSTR pName, LPTSTR pPortName, DWORD dwLevel, LPBYTE pPortInfo)
{
	return Win_SetPort (pName, pPortName, dwLevel, pPortInfo);
}

#endif


// ... GetObject ............................................................

#if defined (GetObject)


inline int		Win_GetObject (HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject)
{
	return GetObject (hgdiobj, cbBuffer, lpvObject);
}

#undef	GetObject

inline int		GetObject (HGDIOBJ hgdiobj, int cbBuffer, LPVOID lpvObject)
{
	return Win_GetObject (hgdiobj, cbBuffer, lpvObject);
}

#endif

// ... StartService ............................................................

#if defined (StartService)


inline BOOL	Win_StartService (SC_HANDLE hService, DWORD dwNumServiceArgs, LPCTSTR *lpServiceArgVectors)
{
	return StartService (hService, dwNumServiceArgs, lpServiceArgVectors);
}

#undef	StartService

inline BOOL	StartService (SC_HANDLE hService, DWORD dwNumServiceArgs, LPCTSTR *lpServiceArgVectors)
{
	return Win_StartService (hService, dwNumServiceArgs, lpServiceArgVectors);
}

#endif

// ... GetServiceDisplayName ............................................................

#if defined (GetServiceDisplayName)


inline BOOL	Win_GetServiceDisplayName (SC_HANDLE hSCManager, LPCTSTR lpServiceName, LPTSTR lpDisplayName, LPDWORD lpcchBuffer)
{
	return GetServiceDisplayName (hSCManager, lpServiceName, lpDisplayName, lpcchBuffer);
}

#undef	GetServiceDisplayName

inline BOOL	GetServiceDisplayName (SC_HANDLE hSCManager, LPCTSTR lpServiceName, LPTSTR lpDisplayName, LPDWORD lpcchBuffer)
{
	return Win_GetServiceDisplayName (hSCManager, lpServiceName, lpDisplayName, lpcchBuffer);
}

#endif

// ... MoveFile ............................................................

#if defined (MoveFile)


inline BOOL	Win_MoveFile (LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
{
	return MoveFile (lpExistingFileName, lpNewFileName);
}

#undef	MoveFile

inline BOOL	MoveFile (LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName)
{
	return Win_MoveFile (lpExistingFileName, lpNewFileName);
}

#endif

// ... GetFreeSpace ............................................................

#if defined (GetFreeSpace)

// Not supported in Windows
#undef	GetFreeSpace

#endif

// ... DrawText ................................................................

#if defined (DrawText)

inline BOOL		Win_DrawTextA (HDC hdc, LPCSTR lpchText, int cchText, LPRECT lprc, UINT format)
{
	return DrawTextA (hdc, lpchText, cchText, lprc, format);
}

inline BOOL		Win_DrawTextW (HDC hdc, LPCWSTR lpchText, int cchText, LPRECT lprc, UINT format)
{
	return DrawTextW (hdc, lpchText, cchText, lprc, format);
}

#undef	DrawText

inline BOOL		DrawText (HDC hdc, LPCSTR lpchText, int cchText, LPRECT lprc, UINT format)
{
	return Win_DrawTextA (hdc, lpchText, cchText, lprc, format);
}

inline BOOL		DrawText (HDC hdc, LPCWSTR lpchText, int cchText, LPRECT lprc, UINT format)
{
	return Win_DrawTextW (hdc, lpchText, cchText, lprc, format);
}

#endif

#pragma warning (pop)


// Simply we don't need this macro
// http://stackoverflow.com/questions/18909719/for-what-is-the-function-like-macro-yield-in-winbase-h-line-97
#if defined (Yield)
#undef Yield
#endif

#endif
