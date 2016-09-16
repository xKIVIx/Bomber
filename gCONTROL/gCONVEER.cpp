#include "gCONVEER.h"

gCONVEER::gCONVEER()
{
	now = new gCONVEER_ELEM;
	end = now;
}

gCONVEER::~gCONVEER()
{
	gCONVEER_ELEM * tmp ;
	while (now->next_ != NULL)
	{
		tmp = now->next_;
		delete now;
		now = tmp;
	}
	delete now;
}

void gCONVEER::Do()
{
	while (GetNextPointer() != NULL)
	{
		gCONVEER_ELEM * tmp;
		tmp = now->next_;
		delete now;
		now = tmp;
		now->func_();
	}
}

void gCONVEER::AddFunc(std::function<void()> func)
{
	gCONVEER_ELEM * tmp = new gCONVEER_ELEM;
	tmp->func_ = func;
	std::lock_guard <std::mutex> lock(lock_);
	end->next_ = tmp;
	end = tmp;
}

gCONVEER_ELEM * gCONVEER::GetNextPointer()
{
	std::lock_guard <std::mutex> lock(lock_);
	return now->next_;
}
