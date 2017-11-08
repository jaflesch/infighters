#include "common.h"
#include "input.h"
#define HOGL_IMPLEMENT
#include "ho_gl.h"
#include "util.h"
#include <hmath.h>
#include "application.h"
#include "WindowApi\Window.h"
#include "chat.h"
#include <stdlib.h>
#include <time.h>
#include "debug_table.h"

DebugTable debug_table;

Window_State win_state;

Keyboard_State keyboard_state = { 0 };
Mouse_State mouse_state = { 0 };
Chat* g_chat = 0;

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
		create_object(buffer);
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
#if DEBUG
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
		CW_USEDEFAULT, CW_USEDEFAULT,
		win_state.win_width, win_state.win_height, NULL, NULL, instance, NULL
	);
	if (!win_state.window_handle) error_fatal("Error criating window context.\n", 0);

	ShowWindow(win_state.window_handle, cmd_show);
	UpdateWindow(win_state.window_handle);

	init_opengl(win_state.window_handle, &win_state.device_context, &win_state.rendering_context, 3, 1);

	init_application();

	wglSwapIntervalEXT_(1);		// Enable Vsync

	bool running = true;
	MSG msg;

	// Track mouse events
	TRACKMOUSEEVENT mouse_event = { 0 };
	mouse_event.cbSize = sizeof(mouse_event);
	mouse_event.dwFlags = TME_LEAVE;
	mouse_event.dwHoverTime = HOVER_DEFAULT;
	mouse_event.hwndTrack = win_state.window_handle;

	using namespace linked;

	linked::Window::linkedWindowInit();

	linked::Window* bgwindow = new linked::Window(win_state.win_width, win_state.win_height, hm::vec2(0, 0), hm::vec4(0, 0, 0, 0), (u8*)"", 0, W_UNFOCUSABLE);
	linked::WindowDiv* bgdiv = new linked::WindowDiv(*bgwindow, win_state.win_width, win_state.win_height, 0, 0, hm::vec2(0, 0), hm::vec4(0, 1, 0, 0.5f), DIV_ANCHOR_LEFT | DIV_ANCHOR_TOP);
	bgwindow->divs.push_back(bgdiv);
	Texture* bgtexture = new Texture("res/textures/bg2.jpg");
	bgdiv->setBackgroundTexture(bgtexture);

	linked::Window* window = new linked::Window(400, 840, hm::vec2(200, 30), hm::vec4(12.0f/255.0f, 16.0f/255.0f, 40.0f/255.0f, 1.0f), (u8*)"Hello", sizeof("Hello"), linked::W_BORDER);
	window->setBorderSizeX(10.0f);
	window->setBorderSizeY(0.0f);
	window->setBorderColor(hm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

	linked::Window* window2 = new linked::Window(760, 630, hm::vec2(620, 30), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.4f), (u8*)"Hello", sizeof("Hello"), 0);

	linked::Window* window3 = new linked::Window(760, 200, hm::vec2(620, 670), hm::vec4(12.0f / 255.0f, 16.0f / 255.0f, 40.0f / 255.0f, 0.4f), (u8*)"Hello", sizeof("Hello"), 0);

	linked::WindowDiv* s_div1 = new linked::WindowDiv(*window3, 110, 110, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 0.0f + 110.0f * 0.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div2 = new linked::WindowDiv(*window3, 110, 110, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 1.0f + 110.0f * 1.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* s_div3 = new linked::WindowDiv(*window3, 110, 110, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 2.0f + 110.0f * 2.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	window3->divs.push_back(s_div1);
	window3->divs.push_back(s_div2);
	window3->divs.push_back(s_div3);

	linked::WindowDiv* info_confirm = new linked::WindowDiv(*window3, 24, 24, 0, 0, hm::vec2(400.0f, 20.0f + 110.0f), hm::vec4(1, 0, 0, 1), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	Texture* confirm_texture = new Texture("res/textures/enter.png");
	info_confirm->setBackgroundTexture(confirm_texture);

	linked::WindowDiv* info_confirm_labels = new linked::WindowDiv(*window3, 256, 24, 0, 0, hm::vec2(400.0f, 20.0f + 110.0f), hm::vec4(1,0,0,0.1f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	window3->divs.push_back(info_confirm_labels);
	linked::Label* confirm_label = new linked::Label(*info_confirm_labels, (u8*)"Hello", sizeof("Hello"), hm::vec2(0.0f, 0.0f), 24.0f);
	info_confirm_labels->getLabels().push_back(confirm_label);

	
	linked::WindowDiv* div1 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 0.0f + 110.0f * 0.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div2 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 1.0f + 110.0f * 1.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div3 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 2.0f + 110.0f * 2.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div4 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 3.0f + 110.0f * 3.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div5 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 4.0f + 110.0f * 4.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div6 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 5.0f + 110.0f * 5.0f, 10.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	window2->divs.push_back(div1);
	window2->divs.push_back(div2);
	window2->divs.push_back(div3);
	window2->divs.push_back(div4);
	window2->divs.push_back(div5);
	window2->divs.push_back(div6);

	linked::WindowDiv* div7  = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 0.0f + 110.0f * 0.0f, 10.0f * 2.0f + 300.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div8  = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 1.0f + 110.0f * 1.0f, 10.0f * 2.0f + 300.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div9  = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 2.0f + 110.0f * 2.0f, 10.0f * 2.0f + 300.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div10 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 3.0f + 110.0f * 3.0f, 10.0f * 2.0f + 300.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div11 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 4.0f + 110.0f * 4.0f, 10.0f * 2.0f + 300.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	linked::WindowDiv* div12 = new linked::WindowDiv(*window2, 110, 300, 0.0f, 0.0f, hm::vec2(25.0f + 10.0f * 5.0f + 110.0f * 5.0f, 10.0f * 2.0f + 300.0f), hm::vec4(0, 0, 0, 1.0f), linked::DIV_ANCHOR_LEFT | linked::DIV_ANCHOR_TOP);
	window2->divs.push_back(div7);
	window2->divs.push_back(div8);
	window2->divs.push_back(div9);
	window2->divs.push_back(div10);
	window2->divs.push_back(div11);
	window2->divs.push_back(div12);

	linked::Button* but1 = new Button(*div1, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but1->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but1->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but2 = new Button(*div2, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but2->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but2->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but3 = new Button(*div3, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but3->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but3->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but4 = new Button(*div4, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but4->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but4->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but5 = new Button(*div5, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but5->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but5->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but6 = new Button(*div6, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but6->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but6->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but7 = new Button(*div7, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but7->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but7->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but8 = new Button(*div8, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but8->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but8->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but9 = new Button(*div9, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but9->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but9->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but10 = new Button(*div10, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but10->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but10->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but11 = new Button(*div11, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but11->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but11->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	linked::Button* but12 = new Button(*div12, 0, hm::vec2(0, 0), 110, 300, hm::vec4(0, 1, 0, 1));
	but12->setHoveredBGColor(hm::vec4(1, 1, 1, 1));
	but12->setNormalBGColor(hm::vec4(0, 0, 0, 1));
	div1->getButtons().push_back(but1);
	div2->getButtons().push_back(but2);
	div3->getButtons().push_back(but3);
	div4->getButtons().push_back(but4);
	div5->getButtons().push_back(but5);
	div6->getButtons().push_back(but6);
	div7->getButtons().push_back(but7);
	div8->getButtons().push_back(but8);
	div9->getButtons().push_back(but9);
	div10->getButtons().push_back(but10);
	div11->getButtons().push_back(but11);
	div12->getButtons().push_back(but12);



	Chat chat;
	linked::Window* chat_window = chat.init_chat();
	chat_window->setActive(false);
	chat.m_active = false;
	g_chat = &chat;

	srand(time(0));

	while (running) {
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

		if (keyboard_state.key_event[VK_F1]) {
			keyboard_state.key_event[VK_F1] = false;
			chat_window->setActive(!chat_window->getActive());
			g_chat->m_active = !g_chat->m_active;
		}

		win_state.move_camera = !chat_window->isAttached();
		win_state.do_input = !chat.m_enabled;

		update_and_render();

		if(chat.m_active)
			chat.update();

		linked::Window::updateWindows();
		linked::Window::renderWindows();

		SwapBuffers(win_state.device_context);
	}
	linked::Window::linkedWindowDestroy();
#ifdef DEBUG
	FreeConsole();
#endif
	return 0;
}