
#define __LOG_DLL


#include "log_error.h"
#include <fstream>
#include <time.h>



_LOG_API void LogSendMessage(int type_message,std::string block,std::string message)
{
	// path to log file
	time_t t;
	tm tk;
	time(&t);
	localtime_s(&tk, &t);
	char * tmp;
	tmp = new char[5];
	_itoa_s(tk.tm_mday, tmp, 5, 10);
	std::string name = tmp;
	tmp = new char[5];
	_itoa_s(tk.tm_mon + 1, tmp, 5, 10);
	name = name + "_" + tmp;
	tmp = new char[5];
	_itoa_s(tk.tm_year + 1900, tmp, 5, 10);
	name = name + "_" + tmp;
	std::string path = "log\\" + block;

	//open or create log file
	std::fstream file;
	CreateDirectoryA("log", NULL);
	CreateDirectoryA(path.c_str(), NULL);
	path = path + "\\" + name + ".log";
	file.open(path, std::ios::app);
	tmp = new char[5];
	_itoa_s(tk.tm_hour, tmp, 5, 10);
	name = tmp;
	tmp = new char[5];
	_itoa_s(tk.tm_min, tmp, 5, 10);
	name = name + ":" + tmp;
	tmp = new char[5];
	_itoa_s(tk.tm_sec, tmp, 5, 10);
	name = name + ":" + tmp;
	delete (tmp);

	switch (type_message)
	{
	case LOG_CRITICAL_ERROR: message = "[CRITICAL_ERROR]" + name + "	" + message + "\n"; break;
	case LOG_ERROR: message = "[ERROR]" + name + "	" + message + "\n"; break;
	case LOG_WARNING: message = "[WARNING]" + name + "	" + message + "\n"; break;
	case LOG_INFO: message = "[INFO] " + name + "	" + message + "\n"; break;
	case LOG_DOING: message = "[DOING]" + name + "	" + message + "\n"; break;
	default:message = "[UNDEF]" + name + "	" + message + "\n"; break;
	}
	file << message.c_str();
	file.close();
}