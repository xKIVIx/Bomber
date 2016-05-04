#pragma once
#ifdef mCONTROL_DLL
#define mCONTROL_API _declspec(dllexport)
#else
#define mCONTROL_API _declspec(dllimport)
#endif
#include <Main\includs.h>
class mCONTROL_API mCONTROL
{
public:
	mCONTROL(HWND in_h_wnd,std::function <void> * import, int * key_flags, int size);
	//~mCONTROL();
	void MouseMove(float f_x, float f_y);
	void MouseRightUp(float f_x, float f_y);
	void MouseRightDown(float f_x, float f_y);
	void MouseLeftUp(float f_x, float f_y);
	void MouseLeftDown(float f_x, float f_y);
	void KeyDown(int flag);
	void KeyUp(int flag);
private:
	int now_state = STATE_LOADING;
	class FIELD;
	FIELD * fields;
	int count_fields = 0;
	HWND h_wnd;
	float f_window_width, f_window_height;
};