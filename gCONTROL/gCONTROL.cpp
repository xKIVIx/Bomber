#define gCONTROL_DLL
#define LOG_ON

#include <time.h>

#include <loger\log_error.h>
#include "gCONTROL.h"

std::vector<vOBJECT> gCONTROL::GetObjectsForRend()
{
	std::vector<vOBJECT> out;
	lock_objects_.lock();
	for (unsigned int i = 0; i < objects_.size(); i++)
	{
		if (objects_[i] != NULL)
			out.push_back(objects_[i]->GetRendInfo());
	}
	lock_objects_.unlock();
	return out;
}

gCONTROL::gCONTROL(HWND h_wnd)
{
	h_wnd_ = h_wnd;
}

gCONTROL::~gCONTROL()
{
	lock_objects_.lock();
	for (unsigned int i = 0; i < objects_.size(); i++)
	{
		if (objects_[i] != NULL)
		{
			delete objects_[i];
			objects_[i] = NULL;
		}
	}
	lock_objects_.unlock();
}

void gCONTROL::InitMap(unsigned int map_width, unsigned int map_height)
{
	lock_objects_.lock();
	srand(time(0));
	objects_.resize(map_height*map_width);
	for (unsigned int i_height = 0; i_height < map_height; i_height++)
	{
		objects_[i_height*map_width] = (gOBJECT*)new gOBJECT_WALL(TYPE_WALL::iron, float(1), float(i_height*2+1), 0.0f);
		for (unsigned int i_width = 1; i_width < (map_width - 1 ); i_width++)
		{
			if ((i_height == 0) || (i_height == (map_height - 1)))
				objects_[i_height*map_width + i_width] = (gOBJECT*)new gOBJECT_WALL(TYPE_WALL::iron, float(i_width*2+1), float(i_height*2+1), 0.0f);
			else
				objects_[i_height*map_width + i_width] = (gOBJECT*)new gOBJECT_WALL(TYPE_WALL::brick, float(i_width*2+1), float(i_height*2+1), 0.0f);
		}
		objects_[i_height*map_width + map_width - 1] = (gOBJECT*)new gOBJECT_WALL(TYPE_WALL::iron, float(map_width*2 - 1), float(i_height*2+1), 0.0f);
	}
	//set fisrt player
	objects_[map_width + 1] = (gOBJECT*)new gOBJECT_PERSON(float(3), float(3), 0.0f);
	objects_[map_width + 2] = NULL;
	objects_[map_width*2 + 1] = NULL;
	//set second player
	objects_[map_width*(map_height-2) + map_width -  2] = (gOBJECT*)new  gOBJECT_PERSON(float((map_width-1)*2 - 1), float((map_height - 1) * 2 - 1), 0.0f);
	objects_[map_width*(map_height - 2) + map_width - 3] = NULL;
	objects_[map_width*(map_height - 3) + map_width - 2] = NULL;
	lock_objects_.unlock();
}
