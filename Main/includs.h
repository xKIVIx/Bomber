#pragma once
#include <Windows.h>
#include <tchar.h>
#include <loger\log_error.h>
#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <map>

#include <gl\GL.h>  

extern "C"
{
#include <Lua\lua.h>
#include <Lua\lauxlib.h>
#include <Lua\lualib.h>
}
#include "names.h"
