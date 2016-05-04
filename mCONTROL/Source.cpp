#define mCONTROL_DLL
#define LOG_ON
#include "mCONTROL.h"
//change coord to [-1;1]
void ChangeCord(float * x, float * y,float f_window_width,float f_window_height)
{
	*x = *x / f_window_width * 2.0f - 1.0f;
	*y = (*y / f_window_height * 2.0f - 1.0f)*(-1.0f);
}
class mCONTROL::FIELD
{
public:
	FIELD(float in_left, float in_right, float in_top, float in_bot, std::function <void(void)> in_deal)
	{
		left = in_left;
		right = in_right;
		top = in_top;
		bot = in_bot;
		deal = in_deal;
	}
	bool Doing(float x, float y)
	{
		if ((x > left) && (x<right) && (y>bot) && (y < top))
		{
			deal();
			return 1;
		}
		return 0;
	}
private:
	float top, bot, right, left;
	std::function <void(void)> deal;
};
void mCONTROL_API mCONTROL::MouseRightDown(float f_x, float f_y)
{
}
void mCONTROL_API mCONTROL::MouseLeftDown(float f_x, float f_y)
{
	ChangeCord(&f_x, &f_y, f_window_width, f_window_height);
	char * tmp = new char[2000];
	fields->Doing(f_x,f_y);
}
mCONTROL_API mCONTROL::mCONTROL(HWND in_h_wnd, std::function <void> * import, int * key_flags, int size)
{
	h_wnd = in_h_wnd;
	RECT window_size;
	GetClientRect(h_wnd, &window_size);
	f_window_width = float(window_size.right - window_size.left);
	f_window_height = float(window_size.bottom - window_size.top);
	fields = new FIELD(0.8f, 1.0f, 1.0f, 0.9f, [in_h_wnd]() {SendMessage(in_h_wnd, WM_DESTROY, NULL, NULL); });
}