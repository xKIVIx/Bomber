#pragma once

#ifndef LOG_HEAD
#define LOG_HEAD
#ifdef LOG_DLL
#define LOG_API _declspec(dllexport)
#else
#define	LOG_API _declspec(dllimport)
#endif
//on/off loging
#ifdef LOG_ON
#define LogSend(type_message, block, message) LogSendMessage(type_message,block, message)
#else
#define	LogSend(type_message, block, message)
#endif
#include <string>
#include <Main\names.h>
LOG_API void LogSendMessage(int type_message, std::string block, std::string message);
#endif // !LOG_HEAD


