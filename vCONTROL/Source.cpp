#define LOG_ON
#define vCONTROL_DLL
#include "vCONTROL.h"
#include <thread>
#include <log_error.h>
#include <gl\GL.h>
//function init pixel format
bool InitPixelFormat(HDC h_dc)
{
	LogSend(LOG_INFO, "vCONTROL", "init pixel format");
	PIXELFORMATDESCRIPTOR pixel_setting = {0};
	pixel_setting.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel_setting.nVersion = 1;
	pixel_setting.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
	pixel_setting.iPixelType = PFD_TYPE_RGBA;
	pixel_setting.cColorBits = 32;
	pixel_setting.cDepthBits = 24;

	int pixel_format = ChoosePixelFormat(h_dc, &pixel_setting);
	if (!pixel_format)
	{
		LogSend(LOG_CRITICAL_ERROR, "vCONTROL", "couldn`t chouse pixel format");
		return 1;
	}
	if (!SetPixelFormat(h_dc, pixel_format, &pixel_setting))
	{
		LogSend(LOG_CRITICAL_ERROR, "vCONTROL", "couldn`t init pixel format");
		return 1;
	}
	return 0;
}

vCONTROL::vCONTROL(HWND h_wnd, bool * error)
{
	h_dc = GetDC(h_wnd);
	*error = InitPixelFormat(h_dc);
	if (!*error)
		return;
	LogSend(LOG_INFO, "vCONTROL", "Create context OpenGl");
	h_rc = wglCreateContext(h_dc);
	if (!h_rc)
	{
		LogSend(LOG_CRITICAL_ERROR, "vCONTROL", "Couldn`t create context OpenGl");
		*error = 1;
		return;
	}
	if (!wglMakeCurrent(h_dc, h_rc))
	{
		LogSend(LOG_CRITICAL_ERROR, "vCONTROL", "Couldn`t make current context OpenGl");
		*error = 1;
		return;
	}
}
vCONTROL::~vCONTROL()
{
	wglMakeCurrent(h_dc, NULL);
	wglDeleteContext(h_rc);
	h_rc = 0;
}
void RendScen(HDC h_dc)
{
	LogSend(LOG_INFO, "vCONTROL", "Rend");
	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(h_dc);
}
void vCONTROL::StartRendering()
{
	RendScen(h_dc);
}