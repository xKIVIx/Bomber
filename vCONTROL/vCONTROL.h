#pragma once
#ifndef _vCONTROL_HEADER
#define _vCONTROL_HEADER
#include <vector>
#include <Windows.h>
#include <gl\GL.h>
#include "glext.h"
#include <loger\log_error.h>
#include "OPENGL_BUFFER.h"
#include "OpenGL_math.h"
#include "RenderObject.h"
#ifdef vCONTROL_DLL
#define vCONTROL_API _declspec(dllexport)
#else
#define vCONTROL_API _declspec(dllimport)
#endif
class vCONTROL_API vCONTROL
{
public:
	vCONTROL(HWND in_h_wnd);
	~vCONTROL();
	void Rend();
	void ResizeWindow(int rect_x, int rect_y);
	void SwapRenderObjects(std::vector <OBJECT_FOR_REND> new_render_objects);
private:
	HWND h_wnd;
	HDC h_dc;
	HGLRC h_rc;
	bool stop = 0;
	std::vector <OBJECT_FOR_REND> render_objects;
	std::mutex lock_swap;
};
#endif // !_vCONTROL_HEADER

