#pragma once
#ifndef _vCONTROL_HEADER
#define _vCONTROL_HEADER

#define MAX_SPRITES 10

#include <vector>
#include <unordered_map>
#include <functional>

#include "vOBJECT.h"
#ifdef vCONTROL_DLL
#define vCONTROL_API _declspec(dllexport)
#else
#define vCONTROL_API _declspec(dllimport)
#endif
class vCONTROL_API vCONTROL
{
public:
	vCONTROL(HWND h_wnd);
	~vCONTROL();
	void ResizeWindow(int rect_x, int rect_y);
	void Rend();
	void SetSourceObjects(std::function <std::vector <vOBJECT>()> source_objects);
private:
	HWND h_wnd_;
	HDC h_dc_;
	HGLRC h_rc_;
	// opengl buffer context
	void * opengl_buffer_;
	std::unordered_map <unsigned int, void *> resource_;
	std::vector <vOBJECT> object_for_rend_;
	std::function <std::vector <vOBJECT>()> source_objects_;
	void LoadGameResource();
};
#endif // !_vCONTROL_HEADER

