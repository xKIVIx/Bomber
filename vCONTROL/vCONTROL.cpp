#define LOG_ON
#define vCONTROL_DLL
#include "vCONTROL.h"
//function init pixel format
bool  InitPixelFormat(HDC h_dc)
{
	PIXELFORMATDESCRIPTOR pixel_setting = { 0 };
	pixel_setting.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel_setting.nVersion = 1;
	pixel_setting.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
	pixel_setting.iPixelType = PFD_TYPE_RGBA;
	pixel_setting.cColorBits = 32;
	pixel_setting.cDepthBits = 24;

	int pixel_format = ChoosePixelFormat(h_dc, &pixel_setting);
	if (!pixel_format)
	{
		return 1;
	}
	if (!SetPixelFormat(h_dc, pixel_format, &pixel_setting))
	{
		return 1;
	}
	return 0;
}
//rendering finction
void vCONTROL::ResizeWindow(int rect_x, int rect_y)
{
	glViewport(0, 0, rect_x, rect_y);
}
void vCONTROL::SwapRenderObjects(std::vector<OBJECT_FOR_REND> new_render_objects)
{
	lock_swap.lock();
	render_objects.swap(new_render_objects);
	lock_swap.unlock();
}
void vCONTROL::Rend()
{
	byte erro = NULL;
	OPENGL_BUFFER opengl_buffer(&erro);
	if (!erro)
	{
		LogSend(LOG_INFO, "vCONTROL", "Buffer work");
		while (!stop)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			lock_swap.lock();
			for (UINT i = 0; i < render_objects.size(); i++)
				render_objects[i].Draw(&opengl_buffer);
			lock_swap.unlock();
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			SwapBuffers(h_dc);
		}
	}
	else
	{
		MessageBox(NULL, L"Buffer don`t work", L"Error buffer", NULL);
		LogSend(LOG_ERROR, "vCONTROL", "Buffer don`t work");
	}
}
//set class function
vCONTROL::vCONTROL (HWND in_h_wnd)
{
	h_wnd = in_h_wnd;
	h_dc = GetDC(h_wnd);
	InitPixelFormat(h_dc);
	h_rc = wglCreateContext(h_dc);
	if (!h_rc)
	{
		MessageBox(NULL, L"Error init OpenGL!", L"Error create context", NULL);
		LogSend(LOG_CRITICAL_ERROR,"vCONTROL","Error create context");
		SendMessage(h_wnd, WM_DESTROY,NULL,NULL);
	}
	if (!wglMakeCurrent(h_dc, h_rc))
	{
		MessageBox(NULL, L"Error init OpenGL!", L"Error meke current context", NULL);
		LogSend(LOG_CRITICAL_ERROR, "vCONTROL", "Error meke current context");
		SendMessage(h_wnd, WM_DESTROY, NULL, NULL);
		return;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glAlphaFunc(GL_GREATER, 0.6f);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
}
vCONTROL::~vCONTROL()
{
	if (h_rc)
	{
		wglMakeCurrent(h_dc, NULL);
		wglDeleteContext(h_rc);
		h_rc = 0;
	}
}
