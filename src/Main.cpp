#define STB_IMAGE_IMPLEMENTATION
#define HO_SYSTEM_IMPLEMENT
#define HO_ARENA_IMPLEMENT
#include "common.h"

#include <ho_system.h>
#include <memory_arena.h>
#include <GL/glew.h>

#include "util.h"
#include "os.h"
#include "font_rendering.h"
#include "render_engine.h"
#include "GUI/gui.h"
#include "input.h"

Window_Info window_info;
engine::Grid_3D grid;

Keyboard_State keyboard_state = { 0 };
Mouse_State mouse_state = { 0 };

void application_state_init()
{
	engine::render_engine_init();
	font_rendering_init();
	//gui::gui_init();

	engine::grid_create(&grid, 100, 100, 20.0f, 20.0f);
	grid.position = { -1000.0f, 0.0f, 0.0f };

	glClearColor(0.03f, 0.05f, 0.08f, 1.0f);
	glDisable(GL_CULL_FACE);
}

void application_state_update()
{
	static float speed = 0.01f;
	engine::grid_prepare_render();
	engine::grid_render(&grid);
	grid.position.z += speed;

	//gui::div_render_all();
}

#ifdef _WIN64
#include <windowsx.h>

s32 WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show)
{
#if _DEBUG
	AllocConsole();
	FILE* pCout;
	freopen_s(&pCout, "CONOUT$", "w", stdout);
#endif

	bool is_running = true;

	window_info.width = 1024;
	window_info.height = 768;
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

	application_state_init();

	while (is_running)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
			if (msg.message == WM_QUIT) {
				is_running = false;
				continue;
			}
			switch (msg.message) {
				case WM_KEYDOWN: {
					u32 key = (u32)msg.wParam;
					u32 mod = (u32)msg.lParam;
					keyboard_state.key[key] = true;
				} break;
				case WM_KEYUP: {
					int key = (u32)msg.wParam;
					keyboard_state.key[key] = false;
				} break;
				case WM_MOUSEMOVE: {
					mouse_state.x = (u32)GET_X_LPARAM(msg.lParam);
					mouse_state.y = (u32)GET_Y_LPARAM(msg.lParam);
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
					u32 key = (u32)msg.wParam;
				} break;

			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT);

		application_state_update();
		swap_buffers(&window_info);
	}

	return 0;
}

#elif __linux__
#include <GL/gl.h>
#include <GL/glx.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	window_info.width  = 1024;
	window_info.height = 768;
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

	void(*glXSwapInterval)(int) = (void(*)(int))glXGetProcAddressARB("glXSwapIntervalSGI");
	print("glXSwapInterval = %p\n", glXSwapInterval);
	glXSwapInterval(1);

	int fps = 0;
	while(true)
	{
		//XNextEvent(window_info.display, &xev);

		// On this website it lists all the event masks
		// tronche.com/gui/x/xlib/events/mask.html/
		//
		if(XCheckWindowEvent(window_info.display, window_info.win, ExposureMask | ButtonPressMask | KeyPressMask | FocusChangeMask | ResizeRedirectMask , &xev))
		{
			// /usr/include/X11/X.h line 181 has #define of all types of events
			if(xev.type == KeyPress){
				XKeyEvent* kev = (XKeyEvent*)&xev;
				int key = XLookupKeysym(kev, 0);
				print("Key Press %d\n", key);
				if(key == 'r'){
					gui::gui_release();
				} else if(key == 't') {
					gui::gui_init();
				}
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
		}
		glClear(GL_COLOR_BUFFER_BIT);
		if(fps >= 60){
			fps = 0;
			//print("fps\n");
		}
		usleep(1500);
		fps+=1;
		application_state_update();
		swap_buffers(&window_info);
	}
	return 0;
}
#endif
