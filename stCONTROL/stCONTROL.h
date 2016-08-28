#pragma once
#ifndef stCONTROL_HEAD
#define stCONTROL_HEAD
#include <mutex>
#include <stMENU\stMENU.h>
#ifdef stCONTROL_DLL
#define stCONTROL_API _declspec(dllexport)
#else
#define stCONTROL_API _declspec(dllimport)
#endif
class stCONTROL_API stCONTROL
{
public:
private:
};
#endif // !stCONTROL_HEAD
