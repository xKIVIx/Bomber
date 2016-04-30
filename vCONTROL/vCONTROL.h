#pragma once
#ifdef vCONTROL_DLL
#define vCONTROL_API _declspec(dllexport)
#else
#define vCONTROL_API _declspec(dllimport)
#endif
#include <Main\includs.h>

class vCONTROL_API vCONTROL
{
public:
	vCONTROL(HWND in_h_wnd);
	~vCONTROL();
	void Rend(int new_state);
	void ResizeWindow(int rect_x, int rect_y);
private:
	HWND h_wnd;
	HDC h_dc;
	HGLRC h_rc;
	int now_state = STATE_LOADING;
};
