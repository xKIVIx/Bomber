#define WIN32_LEAN_AND_MEAN
#pragma once
#ifndef _gNET_HEADER
#define _gNET_HEADER
#ifdef gNET_DLL
#define gNET_API _declspec(dllexport)
#else
#define gNET_API _declspec(dllimport)
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mutex>
#include <thread>
#include <vector>

enum TYPE_NET
{
	SERVER,
	CLIENT
};
class gNET_API gNET
{
public:
	gNET(TYPE_NET type,char * server_name);
	~gNET();
	void SendCommand(char com);
	bool GetCommand(char ** comands , unsigned int * count_com);
private:
	WSADATA wsa_data_ = { 0 };
	SOCKET output_sock_ = INVALID_SOCKET, input_sock_ = INVALID_SOCKET;
	char * output_buffer_ = NULL, * input_buffer_= NULL;
	unsigned int size_output_buffer_ = 0, size_input_buffer_ = 0;
	bool stop_ = 0;
	std::mutex lock_output_buffer_, lock_input_buffer_,lock_stop_state_;
	std::thread output_thread_, input_thread_;
	bool sucsses_init_ = 0;
	void InitClientState(char * server_name);
	void InitServerState();
	void GetMessegeThread();
	void SendMessegeThread();
	bool CheckStopState();
	void Stop();
};

#endif // !_gNET_HEADER