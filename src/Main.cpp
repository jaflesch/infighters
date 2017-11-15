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

Window_Info window_info;
Keyboard_State keyboard_state = { 0 };
Mouse_State mouse_state = { 0 };
extern Chat* g_chat;

extern Font_ID fonts[32];

#define DEFAULT_FONT "./res/fonts/Oswald-Regular.ttf"
#define OSWALD_LIGHT "./res/fonts/Oswald-Light.ttf"

void application_state_init()
{
	linked::Window::linkedWindowInit();

	engine::render_engine_init();
	font_rendering_init();
	fonts[0] = load_font(DEFAULT_FONT, 16, false);
	fonts[1] = load_font(DEFAULT_FONT, 14, false);
	fonts[2] = load_font(DEFAULT_FONT, 12, false);
	fonts[3] = load_font(DEFAULT_FONT, 24, false);
	fonts[4] = load_font(DEFAULT_FONT, 32, false);
	fonts[5] = load_font(DEFAULT_FONT, 38, false);

	fonts[6]  = load_font(OSWALD_LIGHT, 16, false);
	fonts[7]  = load_font(OSWALD_LIGHT, 14, false);
	fonts[8]  = load_font(OSWALD_LIGHT, 12, false);
	fonts[9]  = load_font(OSWALD_LIGHT, 24, false);
	fonts[10] = load_font(OSWALD_LIGHT, 32, false);
	fonts[11] = load_font(OSWALD_LIGHT, 38, false);

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
			printf("1 second\n");
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
#endif