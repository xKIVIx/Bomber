#pragma once

#include <functional>
#include <mutex>

struct gCONVEER_ELEM
{
	std::function <void()> func_;
	gCONVEER_ELEM * next_ = NULL;
};
class gCONVEER
{
public:
	gCONVEER();
	~gCONVEER();
	void Do();
	void AddFunc(std::function <void()> func);
private:
	gCONVEER_ELEM * GetNextPointer();
	gCONVEER_ELEM * now;
	gCONVEER_ELEM * end;
	std::mutex lock_;
};