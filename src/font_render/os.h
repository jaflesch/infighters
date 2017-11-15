#pragma once

#include "../common.h"

#ifdef _WIN64
#include <windows.h>
typedef void* Thread_Handle;
typedef u32 Thread_Proc_Return;
#define STDCALL __stdcall
#elif __linux__
#define STDCALL
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <string.h>
#include <pthread.h>
typedef void* Thread_Proc_Return;
typedef pthread_t Thread_Handle;
#endif

struct Window_Info
{
	s32 width;
	s32 height;
#ifdef _WIN64
	HWND window_handle;
	WINDOWPLACEMENT window_placement;
	HDC device_context;
	HGLRC rendering_context;
	LRESULT(CALLBACK* window_proc)(HWND, UINT, WPARAM, LPARAM);
	HINSTANCE hinstance;
	s32 cmd_show;
#elif __linux__
	Display* display;
	Window win;
	GLXContext ctx;
	Colormap cmap;
#endif
};

s32 init_opengl(Window_Info* window_info);
s32 create_window(Window_Info* window_info);
void swap_buffers(Window_Info* window_info);
Thread_Handle create_thread(Thread_Proc_Return(STDCALL *proc)(void*), void* argument, u32* tid);
