#pragma once
#ifdef mCONTROL_DLL
#define mCONTROL_API _declspec(dllexport)
#else
#define mCONTROL_API _declspec(dllimport)
#endif
#include <Windows.h>
#include <functional>

class mCONTROL_API mCONTROL
{
public:
	mCONTROL(HWND h_wnd,std::function <void> * import, int * key_flags, int size);
	~mCONTROL();
	void MouseMove(int x, int y);
	void MouseRightUp(int x, int y);
	void MouseRightDown(int x, int y);
	void MouseLeftUp(int x, int y);
	void MouseLeftDown(int x, int y);
	void KeyDown(int flag);
	void KeyUp(int flag);
private:
	class FIELD;
	FIELD * fields;
	HWND h_wnd;
};