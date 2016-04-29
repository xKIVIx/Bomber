#define LOG_ON
#define vCONTROL_DLL
#include "vCONTROL.h"
#include <thread>
#include <log_error.h>
#include <mutex>
#include <thread>
#include <gl\GL.h>
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
void vCONTROL_API vCONTROL::Rend()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glEnd();
	SwapBuffers(h_dc);
}
//set class function
vCONTROL_API vCONTROL::vCONTROL (HWND h_wnd)
{
	h_dc = GetDC(h_wnd);
	InitPixelFormat(h_dc);
	h_rc = wglCreateContext(h_dc);
	if (!h_rc)
	{
		MessageBox(NULL, L"Ошибка создания окна!", L"init1", NULL);
		SendMessage(h_wnd, WM_DESTROY,NULL,NULL);
	}
	if (!wglMakeCurrent(h_dc, h_rc))
	{
		MessageBox(NULL, L"Ошибка создания окна!", L"init2", NULL);
		SendMessage(h_wnd, WM_DESTROY, NULL, NULL);
		return;
	}
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
void vCONTROL_API vCONTROL::ChangeState(int state)
{
	now_state = state;
}
