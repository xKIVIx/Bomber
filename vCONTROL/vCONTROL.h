#pragma once
#ifdef vCONTROL_DLL
#define vCONTROL_API _declspec(dllexport)
#else
#define vCONTROL_API _declspec(dllimport)
#endif
#include<Windows.h>

//states
#define STATE_LOADING	1000 
#define STATE_MAIN_MENU 1001
#define STATE_GAME		1002
#define STATE_MENU		1003

class vCONTROL_API vCONTROL
{
public:
	vCONTROL(HWND h_wnd);
	void ChangeState(int state);
	~vCONTROL();
	void Rend();
	void ResizeWindow(int rect_x, int rect_y);
private:
	HDC h_dc;
	HGLRC h_rc;
	int now_state = STATE_LOADING;
};
