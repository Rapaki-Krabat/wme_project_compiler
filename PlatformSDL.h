/*
This file is part of WME Lite.
http://dead-code.org/redir.php?target=wmelite

Copyright (c) 2011 Jan Nedoma

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef __WmePlatformSDL_H__
#define __WmePlatformSDL_H__


// #include "SDL.h"
#include "engine_core/wme_base/dctypes.h"


#ifdef __WIN32__
#	define WIN32_LEAN_AND_MEAN

#	ifndef _WIN32_WINNT
	#	define _WIN32_WINNT 0x0500
#	endif
#	include <windows.h>
#	include <shellapi.h>
#else
#	include "wintypes.h"
#endif // __WIN32__

#ifdef __WIN32__
#define ADD_CRASH_REPORT
#endif


class CBGame;


//////////////////////////////////////////////////////////////////////////
class CBPlatform
{
public:
#if 0	
	static int Initialize(CBGame* inGame, int argc, char* argv[]);
	static int Run(CBGame* inGame, int argc, char* argv[]);
	static int MessageLoop();
	static void HandleEvent(SDL_Event* event);

#ifdef __WIN32__
#ifdef ADD_CRASH_REPORT
	// debugging
	static LONG HandleCrashReport(CBGame* Game, EXCEPTION_POINTERS* pExPtrs);
#endif
#endif

	static AnsiString GetSystemFontPath();
	static AnsiString GetPlatformName();

	// Win32 API bindings
	static HINSTANCE ShellExecute(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
	static void OutputDebugString(LPCSTR lpOutputString);
#endif

	static DWORD GetTime();
	
#if 0	
	static void SleepMs(DWORD millis);
	static BOOL GetCursorPos(LPPOINT lpPoint);
	static BOOL SetCursorPos(int X, int Y);
	static BOOL TransformEventCoords(LPPOINT lpPoint);
	static BOOL ShowWindow(HWND hWnd, int nCmdShow);
	static bool DeleteFile(const char* lpFileName);
	static bool CopyFile(const char* from, const char* to, bool failIfExists);
#endif	
	static bool CreateDirectory(const char* path);
	static bool FlushDirectory(const char* path);
#if 0	
	static HWND SetCapture(HWND hWnd);
	static BOOL ReleaseCapture();
	static BOOL SetForegroundWindow(HWND hWnd);

	static BOOL SetRectEmpty(LPRECT lprc);
	static BOOL IsRectEmpty(CONST LPRECT lprc);
	static BOOL PtInRect(LPRECT lprc, POINT p);
	static BOOL SetRect(LPRECT lprc, int left, int top, int right, int bottom);
	static BOOL IntersectRect(LPRECT lprcDst, CONST LPRECT lprcSrc1, CONST LPRECT lprcSrc2);
	static BOOL UnionRect(LPRECT lprcDst, RECT* lprcSrc1, RECT* lprcSrc2);
	static BOOL CopyRect(LPRECT lprcDst, RECT* lprcSrc);
	static BOOL OffsetRect(LPRECT lprc, int dx, int dy);
	static BOOL EqualRect(LPRECT rect1, LPRECT rect2);
	static int  DistanceRectMiddle(LPRECT lprc, POINT p);

#endif

	// string functions
	static int stricmp(const char* str1, const char* str2);
	static int strnicmp(const char* str1, const char* str2, size_t maxCount);
	static char* strupr(char* string);
	static char* strlwr(char* string);

#if 0

	// sdl event callback
	static int SDLEventWatcher(void* userdata, SDL_Event* event);
#endif
	
private:
	static CBGame* Game;
};


#endif
