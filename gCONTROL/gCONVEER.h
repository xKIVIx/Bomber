#pragma once

#include <functional>
#include <mutex>

struct gCONVEER_ELEM
{
	std::function <void()> func_;
	gCONVEER_ELEM * next_ = NULL;
	unsigned int dif_time_ = 0, init_time_ = 0;
};
class gCONVEER
{
public:
	gCONVEER();
	~gCONVEER();
	void Do();
	void AddFunc(std::function <void()> func);
	void AddFunc(unsigned int time_def , std::function <void()> func);
private:
	gCONVEER_ELEM * GetNextPointer();
	gCONVEER_ELEM * now;
	gCONVEER_ELEM * end;
	std::mutex lock_;
	void AddFunc(gCONVEER_ELEM in);
};