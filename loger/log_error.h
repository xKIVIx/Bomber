#define LOG_ERROR 1000
#define LOG_CRITICAL_ERROR 1001
#define LOG_WARNING 1002
#define LOG_DOING 1003
#define LOG_INFO 1004
#pragma once
//определяем типо заголовка
#ifdef __LOG_DLL
#define _LOG_API _declspec(dllexport)
#else
#define	_LOG_API _declspec(dllimport)
#endif
//включение/выключение логирования
#ifdef LOG_ON
#define LogSend(type_message, block, message) LogSendMessage(type_message,block, message)
#else
#define	LogSend(type_message, block, message)
#endif
#include <string>
_LOG_API void LogSendMessage(int type_message, std::string block, std::string message);

