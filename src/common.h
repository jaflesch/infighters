#pragma once

typedef int 		s32;
typedef long long 	s64;
typedef short 		s16;
typedef char		s8;
typedef unsigned int 		u32;
typedef unsigned long long 	u64;
typedef unsigned short 		u16;
typedef unsigned char		u8;

#define null 0

#define USE_CRT 1

#if USE_CRT
#include <stdio.h>
#include <malloc.h>
#endif

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <windowsx.h>

typedef struct {
	HWND window_handle;
	LONG win_width, win_height;
	WINDOWPLACEMENT g_wpPrev;
	HDC device_context;
	HGLRC rendering_context;

	bool move_camera = true;
	bool do_input = true;
} Window_State;

#else
#error OS not yet supported
#endif
