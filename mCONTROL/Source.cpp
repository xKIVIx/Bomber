#define mCONTROL_DLL
#define LOG_ON
#include "mCONTROL.h"
void mCONTROL_API mCONTROL::MouseRightDown(int x, int y)
{
	LogSend(LOG_INFO, "mCONTROL", "right click");
	SendMessage(h_wnd, WM_PAINT, STATE_MAIN_MENU, NULL);
}
void mCONTROL_API mCONTROL::MouseLeftDown(int x, int y)
{
	LogSend(LOG_INFO, "mCONTROL", "left click");
	SendMessage(h_wnd, WM_PAINT, STATE_LOADING, NULL);
}
mCONTROL_API mCONTROL::mCONTROL(HWND in_h_wnd, std::function <void> * import, int * key_flags, int size)
{
	h_wnd = in_h_wnd;
}