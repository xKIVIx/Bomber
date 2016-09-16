#define gCONTROL_DLL
#define LOG_ON

#include <time.h>

#include <loger\log_error.h>
#include "gCONTROL.h"

std::vector<vOBJECT> gCONTROL::GetObjectsForRend()
{
	std::vector<vOBJECT> out;
	vOBJECT tmp;
	for (auto iter = objects_.begin(); iter < objects_.end(); iter++)
	{
		if ((*iter).GetRendInfo(&tmp))
			out.push_back(tmp);
	}
	out.push_back(player_one_->GetRendInfo());
	out.push_back(player_two_->GetRendInfo());
	return out;
}

gCONTROL::gCONTROL(HWND h_wnd, unsigned int map_width, unsigned int map_height)
{
	h_wnd_ = h_wnd;
	InitMap(map_width, map_height);
	game_thread = std::thread(&gCONTROL::GameProcess,this);
}

gCONTROL::~gCONTROL()
{
	StopGame();
	game_thread.join();
}

void gCONTROL::Command(char key)
{
	unsigned int x1 = 0, x2 = 0, y1 = 0, y2= 0;
	int power_fire = 0;
	gOBJECT_PERSON * tmp_pers = player_one_;
	tmp_pers->GetPositionIntervals(&x1, &x2, &y1, &y2);
	switch (key)
	{
	case 'w':
	{
		do_list_.AddFunc([tmp_pers]() {
			tmp_pers->SetMoveDirection(up);
		});
		y1++;
		power_fire += (objects_[TransCoord(x1, y1)].Colis() + objects_[TransCoord(x2, y1)].Colis());
		if (power_fire == 0)
			do_list_.AddFunc([tmp_pers]() {tmp_pers->Move(); });
		break;
	}
	case 's':
	{
		do_list_.AddFunc([tmp_pers]() {
			tmp_pers->SetMoveDirection(down);
		});
		y2--;
		power_fire += (objects_[TransCoord(x1, y2)].Colis() + objects_[TransCoord(x2, y2)].Colis());
		if (power_fire == 0)
			do_list_.AddFunc([tmp_pers]() {tmp_pers->Move(); });
		break;
	}
	case 'a':
	{
		do_list_.AddFunc([tmp_pers]() {
			tmp_pers->SetMoveDirection(left);
		});
		x2--;
		power_fire += (objects_[TransCoord(x2, y1)].Colis() + objects_[TransCoord(x2, y2)].Colis());
		if (power_fire == 0)
			do_list_.AddFunc([tmp_pers]() {tmp_pers->Move(); });
		break;
	}
	case 'd':
	{
		do_list_.AddFunc([tmp_pers]() {
			tmp_pers->SetMoveDirection(right);
		});
		x1++;
		power_fire += (objects_[TransCoord(x1, y1)].Colis() + objects_[TransCoord(x1, y2)].Colis());
		if (power_fire == 0)
			do_list_.AddFunc([tmp_pers]() {tmp_pers->Move(); });
		break;
	}
	case ' ':
	{
		break;
	}
	default:
		break;
	}
}

void gCONTROL::InitMap(unsigned int map_width, unsigned int map_height)
{
	lock_objects_.lock();
	//set vector for store objects
	objects_.resize(map_height*map_width);
	map_height_ = map_height;
	map_width_ = map_width;
	//adding walls
	for (unsigned int y_i = 0; y_i < map_height; y_i++)
	{
		for (unsigned int x_i = 0; x_i < map_width; x_i++)
		{
			if ((y_i == 0)||
				(y_i == (map_height-1))||
				(x_i==0)||
				(x_i==(map_width -1)))
				objects_[TransCoord(x_i, y_i)].SetNewObject((gOBJECT*) new gOBJECT_WALL(TYPE_WALLS::iron, x_i, y_i));
			else
				objects_[TransCoord(x_i, y_i)].SetNewObject((gOBJECT*) new gOBJECT_WALL(TYPE_WALLS::brick, x_i, y_i));
		}
	}
	//add start pos
	//first
	objects_[TransCoord(1, 1)].Delete();
	objects_[TransCoord(1, 2)].Delete();
	objects_[TransCoord(2, 1)].Delete();
	//second
	objects_[TransCoord(map_width - 2, map_height - 2)].Delete();
	objects_[TransCoord(map_width - 2, map_height - 3)].Delete();
	objects_[TransCoord(map_width - 3, map_height - 2)].Delete();
	player_one_ = new gOBJECT_PERSON(1, 1);
	player_two_ = new gOBJECT_PERSON(map_width-2, map_height - 2);
	lock_objects_.unlock();
}

void gCONTROL::StopGame()
{
	lock_stop_stat_.lock();
	stop = 1;
	lock_stop_stat_.unlock();
}

unsigned int gCONTROL::TransCoord(unsigned int x, unsigned int y)
{
	return y*map_width_ + x;
}

void gCONTROL::GameProcess()
{
	while (!CheckStopStat())
	{
		do_list_.Do();
	}
}

void gCONTROL::Boom(unsigned int x, unsigned int y)
{
	int power_boom[4] = {4};
	for (unsigned int i = 0; i < power_boom; i++)
	{
		
	}
}

bool gCONTROL::CheckStopStat()
{
	std::lock_guard<std::mutex> lock(lock_stop_stat_);
	return stop;
}

gCONTROL::CELL::CELL()
{
	SetNewObject(NULL);
}

gCONTROL::CELL::CELL(gOBJECT * new_object)
{
	SetNewObject(new_object);
}

gCONTROL::CELL::CELL(CELL & copy)
{
	copy.lock_.lock();
	lock_.lock();
	gOBJECT * tmp;
	tmp = copy.object_;
	copy.object_ = object_;
	object_ = tmp;
	copy.lock_.unlock();
	lock_.unlock();
}

gCONTROL::CELL::~CELL()
{
	Delete();
}

bool gCONTROL::CELL::GetRendInfo(vOBJECT * out)
{
	std::lock_guard <std::mutex> lock(lock_);
	if (object_ != NULL)
	{
		*out = object_->GetRendInfo();
		return 1;
	}
	else
	{
		return 0;
	}
}

void gCONTROL::CELL::Delete()
{
	lock_.lock();
	if (object_ != NULL)
	{
		delete object_;
		object_ = NULL;
	}
	lock_.unlock();
}

void gCONTROL::CELL::SetNewObject(gOBJECT * new_object)
{
	lock_.lock();
	object_ = new_object;
	lock_.unlock();
}

bool gCONTROL::CELL::Damage(int * power_bomb)
{
	*power_bomb--;
	if (object_ == NULL)
		return true;
	object_->Destroy(power_bomb);
	if (*power_bomb >= 0)
		return true;
	return false;
}

int gCONTROL::CELL::Colis()
{
	int back_damage = 0;
	if (object_ != NULL)
	{
		lock_.lock();
		object_->Destroy(&back_damage);
		lock_.unlock();
	}
	return back_damage;
}
