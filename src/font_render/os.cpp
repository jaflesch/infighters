#include "render_engine.h"
#include "os.h"
#include "../util.h"

// @temporary
#include "../input.h"
#include "../WindowApi/Window.h"
#include "../chat.h"

#define OPENGL_MAJOR_VERSION 3
#define OPENGL_MINOR_VERSION 3

extern Window_Info window_info;
extern Mouse_State mouse_state;
extern Keyboard_State keyboard_state;
extern Chat* g_chat;

#ifdef _WIN64

LRESULT CALLBACK window_proc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE: {
		int width = window_info.width;
		int height = window_info.height;
		int x = GET_X_LPARAM(lparam);
		int y = GET_Y_LPARAM(lparam);

		const float aspect = (float)width / height;
		float screenX = ((float)x * 2 / width - 1.0f);
		float screenY = -((float)y * 2 / height - 1.0f);

		mouse_state.x = x;
		mouse_state.y = y;
	}break;
	case WM_LBUTTONDOWN: {
		int width = window_info.width;
		int height = window_info.height;

		const float aspect = (float)width / height;
		float screenX = ((float)mouse_state.x * 2 / width - 1.0f);
		float screenY = -((float)mouse_state.x * 2 / height - 1.0f);

		for (unsigned int i = 0; i < linked::Window::openedWindows.size(); i++)
			linked::Window::openedWindows[i]->mouseCallback(0, 1, 0);
		linked::Button::mouseCallback(0, 1, 0);
	}break;
	case WM_LBUTTONUP: {
		for (unsigned int i = 0; i < linked::Window::openedWindows.size(); i++)
			linked::Window::openedWindows[i]->mouseCallback(0, 0, 0);
		linked::Button::mouseCallback(0, 0, 0);
	}break;
	case WM_KILLFOCUS: {
		ZeroMemory(keyboard_state.key, MAX_KEYS);
	}break;
	case WM_CREATE: break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SYSKEYDOWN:
		break;
	case WM_SYSKEYUP:
		break;
	case WM_CHAR:
		if (g_chat && g_chat->m_active) {
			g_chat->handle_keystroke((u64)wparam, (u64)lparam);
		}
		break;
	case WM_SIZE: {
		RECT r;
		GetClientRect(window, &r);
		window_info.width = r.right - r.left;
		window_info.height = r.bottom - r.top;
		glViewport(0, 0, window_info.width, window_info.height);
	} break;
	case WM_DROPFILES: {
		char buffer[512];
		HDROP hDrop = (HDROP)wparam;
		UINT ret = DragQueryFile(hDrop, 0, buffer, 512);
		POINT mouse_loc;
		DragQueryPoint(hDrop, &mouse_loc);
		DragFinish(hDrop);
		//create_object(buffer);
	}break;
	default:
		return DefWindowProc(window, msg, wparam, lparam);
	}
	return 0;
}

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_FLAGS_ARB                   0x2094

s32 init_opengl(Window_Info* window_info)
{
	int PixelFormat;
	window_info->device_context = GetDC(window_info->window_handle);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 24;
	pfd.iPixelType = PFD_TYPE_RGBA;

	PixelFormat = ChoosePixelFormat(window_info->device_context, &pfd);
	if (!SetPixelFormat(window_info->device_context, PixelFormat, &pfd)) {
		print("Error setting pixel format.");
		return -1;
	}

	HGLRC temp_context = wglCreateContext(window_info->device_context);
	BOOL error = wglMakeCurrent(window_info->device_context, temp_context);

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, OPENGL_MAJOR_VERSION,
		WGL_CONTEXT_MINOR_VERSION_ARB, OPENGL_MINOR_VERSION,
		WGL_CONTEXT_FLAGS_ARB, 0, 0
	};
	HGLRC(WINAPI* wglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, int *attribList);
	wglCreateContextAttribsARB = (HGLRC(WINAPI*)(HDC, HGLRC, int *))wglGetProcAddress("wglCreateContextAttribsARB");
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(temp_context);
	window_info->rendering_context = wglCreateContextAttribsARB(window_info->device_context, 0, attribs);
	wglMakeCurrent(window_info->device_context, window_info->rendering_context);
	return 0;
}

s32 create_window(Window_Info* window_info)
{
	window_info->window_proc = window_proc;

	WNDCLASSEX window_class;
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = 0;
	window_class.lpfnWndProc = window_info->window_proc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = window_info->hinstance;
	window_class.hIcon = LoadIcon(window_info->hinstance, MAKEINTRESOURCE(NULL));
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = 0;
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = "SquigglyClass";
	window_class.hIconSm = NULL;

	if (!RegisterClassEx(&window_class))
	{
		print("Error creating window class.\n");
		return -1;
	}

	u32 window_style_exflags = WS_EX_ACCEPTFILES | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	u32 window_style_flags = WS_OVERLAPPEDWINDOW;
	// Note: Client area must be correct, so windows needs to get the WindowRect
	// area depending on the style of the window
	RECT window_rect = { 0 };
	window_rect.right = window_info->width;
	window_rect.bottom = window_info->height;
	AdjustWindowRectEx(&window_rect, window_style_flags, FALSE, window_style_exflags);

	window_info->window_placement.length = sizeof(WINDOWPLACEMENT);
	window_info->width = window_rect.right - window_rect.left;
	window_info->height = window_rect.bottom - window_rect.top;

	window_info->window_handle = CreateWindowExA(
		window_style_exflags,
		window_class.lpszClassName,
		"Infighters",
		window_style_flags,
		50, 50,
		window_info->width, window_info->height, NULL, NULL, window_info->hinstance, NULL
	);

	if (!window_info->window_handle)
	{
		print("Error criating window context.\n");
		return -1;
	}

	ShowWindow(window_info->window_handle, window_info->cmd_show);
	UpdateWindow(window_info->window_handle);

	return 0;
}

void swap_buffers(Window_Info* window_info)
{
	SwapBuffers(window_info->device_context);
}

Thread_Handle create_thread(Thread_Proc_Return(STDCALL *proc)(void*), void* argument, u32* tid)
{
	void* thread_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)proc, argument, 0, (LPDWORD)tid);
	return thread_handle;
}

#elif __linux__

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);


Thread_Handle create_thread(Thread_Proc_Return(STDCALL *proc)(void*), void* argument, u32* tid)
{
	pthread_t tinfo;
	int error = pthread_create(&tinfo, 0, proc, argument);
	return tinfo;
}

static bool isExtensionSupported(const char *extList, const char *extension)
{
	const char *start;
	const char *where, *terminator;

	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;

	/* It takes a bit of care to be fool-proof about parsing the
	   OpenGL extensions string. Don't be fooled by sub-strings,
	   etc. */
	for (start=extList;;) {
		where = strstr(start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if ( where == start || *(where - 1) == ' ' )
			if ( *terminator == ' ' || *terminator == '\0' )
				return true;

		start = terminator;
	}

	return false;
}

static bool ctxErrorOccurred = false;
static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
	ctxErrorOccurred = true;
	return 0;
}


s32 create_window(Window_Info* info)
{
	s8 name[] = "Squiggly";
	info->display = XOpenDisplay(NULL);
	if (!info->display)
	{
		print("Failed to open X display\n");
		return -1;
	}

	// Get a matching FB config
	static int visual_attribs[] =
	{
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		//GLX_SAMPLE_BUFFERS  , 1,
		//GLX_SAMPLES         , 4,
		None
	};

	int glx_major, glx_minor;

	// FBConfigs were added in GLX version 1.3.
	if ( !glXQueryVersion( info->display, &glx_major, &glx_minor ) || ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
	{
		print("Invalid GLX version");
		return -1;
	}

	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig(info->display, DefaultScreen(info->display), visual_attribs, &fbcount);
	if (!fbc)
	{
		print( "Failed to retrieve a framebuffer config\n" );
		return -1;
	}
	// Pick the FB config/visual with the most samples per pixel
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

	int i;
	for (i=0; i<fbcount; ++i)
	{
		XVisualInfo *vi = glXGetVisualFromFBConfig(info->display, fbc[i] );
		if ( vi )
		{
			int samp_buf, samples;
			glXGetFBConfigAttrib(info->display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
			glXGetFBConfigAttrib(info->display, fbc[i], GLX_SAMPLES       , &samples);

			if (best_fbc < 0 || samp_buf && samples > best_num_samp)
				best_fbc = i, best_num_samp = samples;
			if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
				worst_fbc = i, worst_num_samp = samples;
		}
		XFree(vi);
	}

	GLXFBConfig bestFbc = fbc[best_fbc];

	// Be sure to free the FBConfig list allocated by glXChooseFBConfig()
	XFree(fbc);

	// Get a visual
	XVisualInfo *vi = glXGetVisualFromFBConfig(info->display, bestFbc);

	XSetWindowAttributes swa;
	swa.colormap = info->cmap = XCreateColormap( info->display,
			RootWindow( info->display, vi->screen ), 
			vi->visual, AllocNone );
	swa.background_pixmap = None ;
	swa.border_pixel      = 0;
	swa.event_mask        = StructureNotifyMask;

	info->win = XCreateWindow(info->display, RootWindow(info->display, vi->screen), 0, 0, info->width, info->height, 0, vi->depth, InputOutput, vi->visual, 
			CWBorderPixel|CWColormap|CWEventMask, &swa );
	XSelectInput(info->display, info->win, ExposureMask | KeyPressMask);
	if (!info->win)
	{
		print( "Failed to create window.\n" );
		return -1;
	}

	XClassHint* class_hint = XAllocClassHint();
	u8 class_name[] = "infighters";
	u8 class_class[] = "infclass";
	class_hint->res_name = (char*)class_name;
	class_hint->res_class = (char*)class_class;
	XSetClassHint(info->display, info->win, class_hint);

	// Done with the visual info data
	XFree(vi);

	XStoreName(info->display, info->win, name);
	XMapWindow(info->display, info->win);

	// Get the default screen's GLX extension list
	const char *glxExts = glXQueryExtensionsString(info->display, DefaultScreen(info->display));
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );


	// Install an X error handler so the application won't exit if GL 3.0
	// context allocation fails.
	//
	// Note this error handler is global.  All display connections in all threads
	// of a process use the same error handler, so be sure to guard against other
	// threads issuing X commands while this code is running.
	ctxErrorOccurred = false;
	int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

	// Check for the GLX_ARB_create_context extension string and the function.
	// If either is not present, use GLX 1.3 context creation method.
	if ( !isExtensionSupported( glxExts, "GLX_ARB_create_context" ) || !glXCreateContextAttribsARB )
	{
		print("glXCreateContextAttribsARB() not found, using old-style GLX context\n");
		info->ctx = glXCreateNewContext(info->display, bestFbc, GLX_RGBA_TYPE, 0, True);
	}

	// If it does, try to get a GL 3.0 context!
	else
	{
		int context_attribs[] = 
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, OPENGL_MAJOR_VERSION,
			GLX_CONTEXT_MINOR_VERSION_ARB, OPENGL_MINOR_VERSION,
			GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			None
		};
		info->ctx = glXCreateContextAttribsARB(info->display, bestFbc, 0, True, context_attribs);

		// Sync to ensure any errors generated are processed.
		XSync(info->display, False);
		if (!ctxErrorOccurred && info->ctx)
			print("Created %d.%d OpenGL context.\n", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
		else
		{
			context_attribs[1] = 1;
			context_attribs[3] = 0;
			ctxErrorOccurred = false;

			print("Failed to create GL %d.%d context, using old-style GLX context\n", OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
			info->ctx = glXCreateContextAttribsARB(info->display, bestFbc, 0, True, context_attribs);
		}
	}

	// Sync to ensure any errors generated are processed.
	XSync(info->display, False);

	// Restore the original error handler
	XSetErrorHandler(oldHandler);

	if (ctxErrorOccurred || !info->ctx)
	{
		print("Failed to create an OpenGL context\n");
		return -1;
	}

	glXMakeCurrent(info->display, info->win, info->ctx);
}

s32 init_opengl(Window_Info* info)
{

}

void swap_buffers(Window_Info* info)
{
	glXSwapBuffers(info->display, info->win);
}
#endif
