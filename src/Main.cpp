#include "common.h"
#define STB_IMAGE_IMPLEMENTATION
#define HO_SYSTEM_IMPLEMENT
#include "ho_system.h"
#include "util.h"
#include <GL/glew.h>
#include "os.h"
#include "font_rendering.h"
#include "render_engine.h"
#include "GUI/gui.h"

Window_Info window_info;

Texture DEBUG_texture;
size_t div_index = -1;
size_t div_index2 = -1;

void application_state_init()
{
	engine::render_engine_init();
	font_rendering_init();
	gui::gui_init();

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glDisable(GL_CULL_FACE);
}

void application_state_update()
{
	gui::div_render_all();
	//gui::div_render(div_index);
	//gui::div_render(div_index2);
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
			} break;
			case WM_KEYUP: {
			} break;
			case WM_MOUSEMOVE: {
			} break;
			case WM_LBUTTONDOWN: {
			} break;
			case WM_LBUTTONUP: {
			} break;
			case WM_CHAR: {
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
			if(xev.type == KeyPress)
				print("Key Press\n");
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
