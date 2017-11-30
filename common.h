#pragma once
#define internal static
#define null 0
#define FLAG(X) (1 << X)

typedef int 		s32;
typedef long long 	s64;
typedef short 		s16;
typedef char		s8;
typedef unsigned int 		u32;
typedef unsigned long long 	u64;
typedef unsigned short 		u16;
typedef unsigned char		u8;
typedef float r32;
typedef double r64;

#define USE_CRT 1

#if USE_CRT
#include <stdio.h>
#include <malloc.h>
#endif

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <windowsx.h>

#elif defined(__linux__)
#define __stdcall
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <string.h>
#include <pthread.h>
typedef void* Thread_Proc_Return;
typedef pthread_t Thread_Handle;

#endif

typedef struct {
	int win_width, win_height;
#if defined(_WIN32) || defined(_WIN64)
	HWND window_handle;
	WINDOWPLACEMENT g_wpPrev;
	HDC device_context;
	HGLRC rendering_context;
#elif __linux__
	Display* display;
	Window win;
#endif

	bool move_camera = true;
	bool do_input = true;
} Window_State;

