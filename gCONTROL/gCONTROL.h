#pragma once
#ifndef _gCONTROL_HEADER
#define _gCONTROL_HEADER

#include <mutex>
#include <vector>
#include <Windows.h>

#include "gCELL.h"

#ifdef gCONTROL_DLL
#define gCONTROL_API _declspec(dllexport)
#else
#define gCONTROL_API _declspec(dllimport)
#endif

class gCONTROL_API gCONTROL
{
public:
	std::vector <vOBJECT> GetObjectsForRend();
	gCONTROL(HWND h_wnd);
	~gCONTROL();
	void InitMap(unsigned int map_width,unsigned int map_height);
private:
	HWND h_wnd_;
	std::mutex lock_objects_, lock_stop_stat_;
	std::vector <gOBJECT *> objects_;
};

#endif // !_gCONTROL_HEADER