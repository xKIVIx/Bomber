#define LOG_ON
#define vCONTROL_DLL
#include "vCONTROL.h"
#include <vMENUS\vMENUS.h>
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
void vCONTROL_API vCONTROL::ResizeWindow(int rect_x, int rect_y)
{
	glViewport(0, 0, rect_x, rect_y);
}
void vCONTROL_API vCONTROL::Rend(int new_state)
{
	if (new_state)
	{
		LogSend(LOG_INFO, "vCONTROL", "get new state");
		now_state = new_state;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (MENU_API::GetMenu(now_state))
		LogSend(LOG_ERROR, "vCONTROL", "can`t define state");
	SwapBuffers(h_dc);
	SendMessage(h_wnd, WM_PAINT, NULL, NULL);
}
//set class function
vCONTROL_API vCONTROL::vCONTROL (HWND in_h_wnd)
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
	glClearColor(0, 0, 0, 255);
}
vCONTROL_API vCONTROL::~vCONTROL()
{
	if (h_rc)
	{
		wglMakeCurrent(h_dc, NULL);
		wglDeleteContext(h_rc);
		h_rc = 0;
	}
}
