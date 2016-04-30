#pragma once
#ifdef MENU_GET_DLL
#define MENU_GET_API _declspec(dllexport)
#else
#define MENU_GET_API _declspec(dllimport)
#endif

#include <Main\includs.h>
namespace MENU_API
{
	bool MENU_GET_API GetMenu(int type);
}