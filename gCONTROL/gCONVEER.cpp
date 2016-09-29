#include "gCONVEER.h"
#include <time.h>
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
	if (GetNextPointer() != NULL)
	{
		gCONVEER_ELEM * tmp;
		tmp = now->next_;
		delete now;
		now = tmp;
		if (now->dif_time_ == 0)
			now->func_();
		else
		{
			if (now->dif_time_ <= (clock() - now->init_time_))
				now->func_();
			else
				AddFunc(*now);
		}
	}
}

void gCONVEER::AddFunc(std::function<void()> func)
{
	AddFunc(0, func);
}

void gCONVEER::AddFunc(unsigned int time_dif, std::function<void()> func)
{
	gCONVEER_ELEM * tmp = new gCONVEER_ELEM;
	tmp->func_ = func;
	tmp->init_time_ = clock();
	tmp->dif_time_ = time_dif;
	std::lock_guard <std::mutex> lock(lock_);
	end->next_ = tmp;
	end = tmp;
}

gCONVEER_ELEM * gCONVEER::GetNextPointer()
{
	std::lock_guard <std::mutex> lock(lock_);
	return now->next_;
}

void gCONVEER::AddFunc(gCONVEER_ELEM in)
{
	gCONVEER_ELEM * tmp = new gCONVEER_ELEM;
	*tmp = in;
	tmp->next_ = NULL;
	std::lock_guard <std::mutex> lock(lock_);
	end->next_ = tmp;
	end = tmp;
}
