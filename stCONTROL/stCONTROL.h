#pragma once
#ifdef stCONTROL_DLL
#define stCONTROL_API _declspec(dllexport)
#else
#define stCONTROL_API _declspec(dllimport)
#endif
class stCONTROL_API stCONTROL
{
private:

public:
};