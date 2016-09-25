#define gNET_DLL
#define LOG_ON
#include "gNET.h"
#include <loger\log_error.h>

gNET::gNET(TYPE_NET type, char * server_name)
{
	// init sock interface
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data_)!=0)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Can`t init socket API");
		return;
	}
	// init output socket
	output_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (output_sock_ == INVALID_SOCKET)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Can`t init output socket");
		WSACleanup();
		return;
	}
	// init input socket
	input_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (output_sock_ == INVALID_SOCKET)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Can`t init input socket");
		WSACleanup();
		return;
	}
	sucsses_init_ = 1;
	switch (type)
	{
	case SERVER:
	{
		InitServerState();
		break;
	}
	case CLIENT:
	{
		InitClientState(server_name);
		break;
	}
	default:
	{
		break;
	}
	}
	output_thread_ = std::thread(&gNET::SendMessegeThread, this);
	input_thread_ = std::thread(&gNET::GetMessegeThread, this);
}

gNET::~gNET()
{
	Stop();
	lock_input_buffer_.lock();
	lock_output_buffer_.lock();
	shutdown(output_sock_, NULL);
	shutdown(input_sock_, NULL);
	output_thread_.detach();
	input_thread_.detach();
	lock_input_buffer_.unlock();
	lock_output_buffer_.unlock();
	closesocket(output_sock_);
	closesocket(input_sock_);
	WSACleanup();
}

void gNET::SendCommand(char com)
{
	std::lock_guard <std::mutex> lock(lock_output_buffer_);
	size_output_buffer_++;
	char * tmp_buffer = new char[size_output_buffer_];
	if (output_buffer_ != NULL)
	{
		memcpy_s(tmp_buffer, size_output_buffer_, output_buffer_, size_output_buffer_ - 1);
		delete[] output_buffer_;
	}
	tmp_buffer[size_output_buffer_ - 1] = com;
	output_buffer_ = tmp_buffer;

}

bool gNET::GetCommand(char ** comands, unsigned int * count_com)
{
	std::lock_guard <std::mutex> lock(lock_input_buffer_);
	*comands = input_buffer_;
	*count_com = size_input_buffer_;
	input_buffer_ = NULL;
	size_input_buffer_ = 0;
	return CheckStopState();
}

void gNET::InitClientState(char * server_name)
{
	LogSend(LOG_INFO, "gNET", "Init client state");
	sockaddr_in adress_out, adress_in;
	hostent* host;
	char * ip;
	host = gethostbyname(server_name);
	ip = inet_ntoa(*(struct in_addr *)*host->h_addr_list);
	adress_out.sin_addr.S_un.S_addr = inet_addr(ip);
	adress_out.sin_family = AF_INET;
	adress_out.sin_port = htons(3300);
	adress_in = adress_out;
	adress_in.sin_port = htons(3301);
	if (connect(output_sock_, (SOCKADDR*)&adress_out, sizeof(adress_out)) == SOCKET_ERROR)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Error connect output line");
		sucsses_init_ = 0;
		return;
	}
	if (connect(input_sock_, (SOCKADDR*)&adress_in, sizeof(adress_in)) == SOCKET_ERROR)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Error connect input line");
		sucsses_init_ = 0;
		return;
	}
}

void gNET::InitServerState()
{
	LogSend(LOG_INFO, "gNET", "Init server state");
	sockaddr_in adress_out, adress_in;
	hostent* host;
	char * ip;
	host = gethostbyname("localhost");
	ip = inet_ntoa(*(struct in_addr *)*host->h_addr_list);
	adress_out.sin_addr.S_un.S_addr = inet_addr(ip);
	adress_out.sin_family = AF_INET;
	adress_out.sin_port = htons(3301);
	adress_in = adress_out;
	adress_in.sin_port = htons(3300);
	if ((bind(output_sock_, (SOCKADDR*)&adress_out, sizeof(adress_out)) == SOCKET_ERROR)||
		(bind(input_sock_, (SOCKADDR*)&adress_in, sizeof(adress_in)) == SOCKET_ERROR))
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Error binding sockets");
		sucsses_init_ = 0;
		return;
	}
	if (listen(input_sock_, 1) == SOCKET_ERROR)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Error listen input line");
		sucsses_init_ = 0;
		return;
	}
	SOCKET tmp_sock = input_sock_;
	input_sock_ = accept(input_sock_, NULL, NULL);
	closesocket(tmp_sock);
	if (input_sock_ == INVALID_SOCKET)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Error input line");
		sucsses_init_ = 0;
		return;
	}
	if (listen(output_sock_, 1) == SOCKET_ERROR)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Error listen output line");
		sucsses_init_ = 0;
		return;
	}
	tmp_sock = output_sock_;
	output_sock_ = accept(output_sock_, NULL, NULL);
	closesocket(tmp_sock);
	if (output_sock_ == INVALID_SOCKET)
	{
		LogSend(LOG_CRITICAL_ERROR, "gNET", "Error output line");
		sucsses_init_ = 0;
		return;
	}
}

void gNET::GetMessegeThread()
{
	char * tmp_buffer = NULL, * tmp_input_buf = NULL;
	int count_com_sent = 0;
	while (!CheckStopState())
	{
		tmp_input_buf = new char[250];
		count_com_sent = recv(input_sock_, tmp_input_buf, 249, NULL);
		if (count_com_sent < 0)
		{
			LogSend(LOG_ERROR, "gNET", "Input error");
			return;
		}
		unsigned int new_size;
		std::lock_guard <std::mutex> lock(lock_input_buffer_);
		new_size = size_input_buffer_ + unsigned int(count_com_sent);
		tmp_buffer = new char[new_size];
		if (input_buffer_ != NULL)
		{
			memcpy_s(tmp_buffer, new_size, input_buffer_, size_input_buffer_);
			delete[] input_buffer_;
		}
		for (unsigned int i = 0; i < count_com_sent; i++)
		{
			tmp_buffer[size_input_buffer_ + i] = tmp_input_buf[i];
		}
		delete[] tmp_input_buf;
		size_input_buffer_ = new_size;
		input_buffer_ = tmp_buffer;
	}
	if (input_buffer_ != NULL)
	{
		size_input_buffer_ = 0;
		delete[] input_buffer_;
	}
}

void gNET::SendMessegeThread()
{
	while (!CheckStopState())
	{
		std::lock_guard <std::mutex> lock (lock_output_buffer_);
		if (output_buffer_ != NULL)
		{
			int k = send(output_sock_, output_buffer_, size_output_buffer_, NULL);
			if (k < 0)
			{
				Stop();
				return;
			}
			unsigned int count_send = unsigned int(k);
			if (count_send != size_input_buffer_)
			{
				char * tmp_buffer = new char[size_output_buffer_ - count_send];
				for (unsigned int i = count_send; i < size_output_buffer_; i++)
					tmp_buffer[i - count_send] = output_buffer_[i];
				delete[]output_buffer_;
				output_buffer_ = tmp_buffer;
				size_output_buffer_ -= count_send;
			}
			else
			{
				if (output_buffer_ != NULL)
				{
					delete[] output_buffer_;
					output_buffer_ = NULL;
				}
			}
		}
	}
	if (output_buffer_ != NULL)
	{
		size_output_buffer_ = 0;
		delete[] output_buffer_;
	}
}

bool gNET::CheckStopState()
{
	std::lock_guard <std::mutex> lock(lock_stop_state_);
	return stop_;
}

void gNET::Stop()
{
	LogSend(LOG_INFO, "gNET", "Stop net");
	std::lock_guard <std::mutex> lock(lock_stop_state_);
	stop_ = 1;
}
