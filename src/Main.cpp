#include "../common.h"
#include "input.h"
#define HOGL_IMPLEMENT
#include "ho_gl.h"
#define HO_ARENA_IMPLEMENT
#include <memory_arena.h>
#include "util.h"
#include <hmath.h>
#include "application.h"
#include "chat.h"
#include <stdlib.h>
#include <time.h>
#include "font_render/os.h"
#include "font_render/render_engine.h"
#include <vector>
#include "WindowApi/Window.h"

#if defined(_WIN32) || defined(_WIN64)
struct Timer
{
	double g_freq;
	Timer()
	{
		LARGE_INTEGER li = {};
		QueryPerformanceFrequency(&li);
		g_freq = double(li.QuadPart);
	}

	double GetTime()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart) / this->g_freq;
	}
};
#elif defined(__linux__)
struct Timer{
	Timer(){}
	double GetTime()
	{
		clockid_t clockid;
		struct timespec t_spec;
		int r = clock_gettime(clockid, &t_spec);
		double res = t_spec.tv_sec + t_spec.tv_nsec;
		return res;
	}
};
#endif

Window_Info window_info;
Keyboard_State keyboard_state = { 0 };
Mouse_State mouse_state = { 0 };
extern Chat* g_chat;

extern Font_ID fonts[32];

#define DEFAULT_FONT "../../../res/fonts/Oswald-Regular.ttf"
#define OSWALD_LIGHT "../../../res/fonts/Oswald-Light.ttf"

void application_state_init()
{

	linked::Window::linkedWindowInit();

	engine::render_engine_init();
	font_rendering_init();
	fonts[FONT_OSWALD_REGULAR_12] = load_font(DEFAULT_FONT, 12, false);
	fonts[FONT_OSWALD_REGULAR_14] = load_font(DEFAULT_FONT, 14, false);
	fonts[FONT_OSWALD_REGULAR_16] = load_font(DEFAULT_FONT, 16, false);
	fonts[FONT_OSWALD_REGULAR_18] = load_font(DEFAULT_FONT, 18, false);
	fonts[FONT_OSWALD_REGULAR_24] = load_font(DEFAULT_FONT, 24, false);
	fonts[FONT_OSWALD_REGULAR_32] = load_font(DEFAULT_FONT, 32, false);
	fonts[FONT_OSWALD_REGULAR_38] = load_font(DEFAULT_FONT, 38, false);

	fonts[FONT_OSWALD_REGULAR_12] = load_font(OSWALD_LIGHT, 12, false);
	fonts[FONT_OSWALD_REGULAR_14] = load_font(OSWALD_LIGHT, 14, false);
	fonts[FONT_OSWALD_REGULAR_16] = load_font(OSWALD_LIGHT, 16, false);
	fonts[FONT_OSWALD_REGULAR_16] = load_font(OSWALD_LIGHT, 18, false);
	fonts[FONT_OSWALD_REGULAR_24] = load_font(OSWALD_LIGHT, 24, false);
	fonts[FONT_OSWALD_REGULAR_32] = load_font(OSWALD_LIGHT, 32, false);
	fonts[FONT_OSWALD_REGULAR_38] = load_font(OSWALD_LIGHT, 38, false);

	init_application();

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glDisable(GL_CULL_FACE);
}

void application_state_update(double frametime)
{
	update_and_render(frametime);

	linked::Window::updateWindows();
	linked::Window::renderWindows();

	//font_rendering_flush();
}

#ifdef _WIN64
s32 WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
{
#if _DEBUG
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
#endif

	bool is_running = true;

	window_info.width = 1600;
	window_info.height = 900;
	window_info.hinstance = instance;
	window_info.cmd_show = cmd_show;

	if (create_window(&window_info))
	{
		print("Error creating window.");
		return -1;
	}

	glewExperimental = true;
	init_opengl(&window_info);
	if (glewInit() != GLEW_OK)
	{
		print("Error loading Glew\n");
		return -1;
	}

	// Track mouse events
	TRACKMOUSEEVENT mouse_event = { 0 };
	mouse_event.cbSize = sizeof(mouse_event);
	mouse_event.dwFlags = TME_LEAVE;
	mouse_event.dwHoverTime = HOVER_DEFAULT;
	mouse_event.hwndTrack = window_info.window_handle;

	application_state_init();

	Timer timer;
	srand(time(0));

	double laststart = timer.GetTime();
	double totaltime = 0.0;
	MSG msg;

	while (is_running)
	{
		double endframe = timer.GetTime();

		double frametime = (endframe - laststart);
		totaltime += frametime;
		laststart = timer.GetTime();

		if (totaltime >= 1.0) {
			totaltime = 0.0;
			//printf("1 second\n");
		}

		TrackMouseEvent(&mouse_event);
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
			if (msg.message == WM_QUIT) {
				is_running = false;
				continue;
			}
			switch (msg.message) {
			case WM_KEYDOWN: {
				size_t key = msg.wParam;
				size_t mod = msg.lParam;
				keyboard_state.key[key] = true;
			} break;
			case WM_KEYUP: {
				size_t key = msg.wParam;
				printf("%d\n", key);
				keyboard_state.key[key] = false;
				keyboard_state.key_event[key] = true;
				if (keyboard_state.key_event[VK_UP] && g_chat->m_enabled) {
					keyboard_state.key_event[VK_UP] = false;
					g_chat->next_history();
				}
				if (keyboard_state.key_event[VK_DOWN] && g_chat->m_enabled) {
					keyboard_state.key_event[VK_DOWN] = false;
					g_chat->previous_history();
				}
			} break;
			case WM_MOUSEMOVE: {
				mouse_state.x = GET_X_LPARAM(msg.lParam);
				mouse_state.y = GET_Y_LPARAM(msg.lParam);

			} break;
			case WM_LBUTTONDOWN: {
				int x = GET_X_LPARAM(msg.lParam);
				int y = GET_Y_LPARAM(msg.lParam);
				mouse_state.is_captured = true;
				mouse_state.x_left = x;
				mouse_state.y_left = y;
				SetCapture(GetActiveWindow());
			} break;
			case WM_LBUTTONUP: {
				mouse_state.is_captured = false;
				ReleaseCapture();
			} break;
			case WM_CHAR: {
				size_t key = msg.wParam;
			} break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		application_state_update(frametime);
		swap_buffers(&window_info);
	}
	linked::Window::linkedWindowDestroy();

	return 0;
}
#elif defined(__linux__)
#include <unistd.h>
s32 main(int argc, char** argv)
{
	window_info.width  = 1600;
	window_info.height = 900;
	create_window(&window_info);
	XEvent xev;

	glewExperimental = true;
	if(glewInit() != GLEW_OK){
		print("Error trying to initialize glew\n");
		return -1;
	}
	
	application_state_init();
	
	Window root;
	int x, y;
	u32 width_return, height_return, border_width_return, depth_return;
	XGetGeometry(window_info.display, window_info.win, &root, &x, &y, &width_return, &height_return, &border_width_return, &depth_return);
	print("got window info\n");
	print("x: %d\ny: %d\nwidth: %d\nheight: %d\nborder_width: %d\ndepth: %d\n", x, y, width_return, height_return, border_width_return, depth_return);
	Screen*  s = DefaultScreenOfDisplay(window_info.display);
	print("Width of Screen: %d\n", WidthOfScreen(s));
	print("Height of Screen: %d\n", HeightOfScreen(s));
//	void(*glXSwapIntervalEXT)(Display*, GLXDrawable, int) = (void(*)(Display*, GLXDrawable, int))glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalEXT");
//	GLXDrawable(*teste)() = (GLXDrawable(*)())glXGetProcAddressARB((const GLubyte*)"glXGetCurrentDrawable");
//	print("address of glXSwapIntervalEXT: %p\n", glXSwapIntervalEXT);
//	print("address of glXGetCurrentDrawable: %p\n", teste);
	
//	GLXDrawable glxdrawable = teste();
//	glXSwapIntervalEXT(window_info.display, glxdrawable, 1);

	int fps = 0;
	XGrabPointer(window_info.display, window_info.win, 1, PointerMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	XAllowEvents(window_info.display, AsyncBoth, CurrentTime);
	while(true)
	{
		//XNextEvent(window_info.display, &xev);

		// On this website it lists all the event masks
		// tronche.com/gui/x/xlib/events/mask.html/
		//
		while(XCheckWindowEvent(window_info.display, window_info.win, ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | FocusChangeMask | ResizeRedirectMask | PointerMotionMask | ButtonMotionMask, &xev))
		{
			// /usr/include/X11/X.h line 181 has #define of all types of events
			if(xev.type == KeyPress){
				XKeyEvent* kev = (XKeyEvent*)&xev;
				int key = XLookupKeysym(kev, 0);
				print("Key Press %d\n", key);
			}
			if(xev.type == FocusIn)
				print("Focus In\n");
			if(xev.type == FocusOut)
				print("Focus Out\n");
			if(xev.type == Expose) {
				//print("Resize\n");
				XExposeEvent* xexp = (XExposeEvent*)&xev;
				//print("%d %d\n", xexp->width, xexp->height);
				static int c = 3;
				if(c == 0){
					//print("Event: %d %d\n", xexp->width, xexp->height);
					window_info.width = xexp->width;
					window_info.height = xexp->height;
					engine::update_window_size();
				} else {
					c-=1;
				}
			}
			if(xev.type == MotionNotify){
				XMotionEvent* xmotev = (XMotionEvent*)&xev;
				mouse_state.x = xmotev->x;
				mouse_state.y = xmotev->y;
				//printf("Motion %d %d\n", xmotev->x, xmotev->y);
			}
			if(xev.type == ButtonPress){
				XButtonEvent* xbuttev = (XButtonEvent*)&xev;
				printf("Press %d %d\n", xbuttev->x, xbuttev->y);
				mouse_state.is_captured = true;
				mouse_state.x_left = xbuttev->x;
				mouse_state.y_left = xbuttev->y;
				for(u32 i = 0; i < linked::Window::openedWindows.size(); ++i)
					linked::Window::openedWindows[i]->mouseCallback(0,1,0);
				linked::Button::mouseCallback(0,1,0);
			}
			if(xev.type == ButtonRelease){
				XButtonEvent* xbuttev = (XButtonEvent*)&xev;
				printf("UnPress %d %d\n", xbuttev->x, xbuttev->y);
				mouse_state.is_captured = false;
				for(u32 i = 0; i < linked::Window::openedWindows.size(); ++i)
					linked::Window::openedWindows[i]->mouseCallback(0,0,0);
				linked::Button::mouseCallback(0,0,0);
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		if(fps >= 60){
			fps = 0;
			//print("fps\n");
		}
		usleep(1500);

		fps+=1;
		application_state_update(1.0/60.0);
		swap_buffers(&window_info);
	}
	return 0;
}

#endif
