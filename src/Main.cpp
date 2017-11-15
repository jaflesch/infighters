#include "common.h"
#include "input.h"
#define HOGL_IMPLEMENT
#include "ho_gl.h"
#include "util.h"
#include <hmath.h>
#include "application.h"
#include "chat.h"
#include <stdlib.h>
#include <time.h>
#include "debug_table.h"

DebugTable debug_table;

Window_State win_state;

Keyboard_State keyboard_state = { 0 };
Mouse_State mouse_state = { 0 };
extern Chat* g_chat;

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

LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE: {
		int width = win_state.win_width;
		int height = win_state.win_height;
		int x = GET_X_LPARAM(lparam);
		int y = GET_Y_LPARAM(lparam);

		const float aspect = (float)width / height;
		float screenX = ((float)x * 2 / width - 1.0f);
		float screenY = -((float)y * 2 / height - 1.0f);

		mouse_state.x = x;
		mouse_state.y = y;
	}break;
	case WM_LBUTTONDOWN: {
		int width = win_state.win_width;
		int height = win_state.win_height;

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
			g_chat->handle_keystroke(wparam, lparam);
		}
		break;
	case WM_SIZE: {
		RECT r;
		GetClientRect(window, &r);
		win_state.win_width = r.right - r.left;
		win_state.win_height = r.bottom - r.top;
		glViewport(0, 0, win_state.win_width, win_state.win_height);
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


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
{
	WNDCLASSEX window_class;
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = 0;
	window_class.lpfnWndProc = WndProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance;
	window_class.hIcon = LoadIcon(instance, MAKEINTRESOURCE(NULL));
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = 0;
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = "HoEngineClass";
	window_class.hIconSm = NULL;

	if (!RegisterClassEx(&window_class)) error_fatal("Error creating window class.\n", 0);

	// alloc console
#if _DEBUG
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
#endif

	u32 window_style_exflags = WS_EX_ACCEPTFILES | WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	u32 window_style_flags = WS_OVERLAPPEDWINDOW;
	// Note: Client area must be correct, so windows needs to get the WindowRect
	// area depending on the style of the window
	RECT window_rect = { 0 };
	window_rect.right = 1600;
	window_rect.bottom = 900;
	AdjustWindowRectEx(&window_rect, window_style_flags, FALSE, window_style_exflags);

	win_state.g_wpPrev.length = sizeof(WINDOWPLACEMENT);
	win_state.win_width = window_rect.right - window_rect.left;
	win_state.win_height = window_rect.bottom - window_rect.top;

	win_state.window_handle = CreateWindowExA(
		window_style_exflags,
		window_class.lpszClassName,
		"HoEngine",
		window_style_flags,
		50, 50,
		win_state.win_width, win_state.win_height, NULL, NULL, instance, NULL
	);
	if (!win_state.window_handle) error_fatal("Error criating window context.\n", 0);

	ShowWindow(win_state.window_handle, cmd_show);
	UpdateWindow(win_state.window_handle);

	init_opengl(win_state.window_handle, &win_state.device_context, &win_state.rendering_context, 3, 1);

	wglSwapIntervalEXT_(1);		// Enable Vsync

	bool running = true;
	MSG msg;

	// Track mouse events
	TRACKMOUSEEVENT mouse_event = { 0 };
	mouse_event.cbSize = sizeof(mouse_event);
	mouse_event.dwFlags = TME_LEAVE;
	mouse_event.dwHoverTime = HOVER_DEFAULT;
	mouse_event.hwndTrack = win_state.window_handle;

	linked::Window::linkedWindowInit();

	init_application();

	Timer timer;

	srand(time(0));

	double laststart = timer.GetTime();
	double totaltime = 0.0;

	while (running) {
		double endframe = timer.GetTime();

		double frametime = (endframe - laststart);
		totaltime += frametime;
		laststart = timer.GetTime();

		//printf("%f\n", totaltime);
		if (totaltime >= 1.0) {
			totaltime = 0.0;
			printf("1 second\n");
		}

		TrackMouseEvent(&mouse_event);
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
			if (msg.message == WM_QUIT) {
				running = false;
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

		update_and_render(frametime);

		linked::Window::updateWindows();
		linked::Window::renderWindows();

		SwapBuffers(win_state.device_context);
	}
	linked::Window::linkedWindowDestroy();
#ifdef _DEBUG
	FreeConsole();
#endif
	return 0;
}