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
	vCONTROL(HWND h_wnd,bool * error);
	~vCONTROL();
	void StartRendering();
	void StopRendering();
	void ChangeState(int state);
private:
	HDC h_dc;
	HGLRC h_rc;
	bool end_render = 0;
	int state = STATE_LOADING;
};
