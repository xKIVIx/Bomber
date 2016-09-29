#define gCONTROL_DLL
#define LOG_ON
#define MOVE_CD 100
#include <time.h>

#include <loger\log_error.h>
#include "gCONTROL.h"

std::vector<vOBJECT> gCONTROL::GetObjectsForRend()
{
	std::vector<vOBJECT> out;
	vOBJECT tmp;
	lock_objects_.lock();
	for (auto iter = objects_.begin(); iter < objects_.end(); iter++)
	{
		if ((*iter).GetRendInfo(&tmp))
			out.push_back(tmp);
	}
	out.push_back(players[0]->GetRendInfo());
	out.push_back(players[1]->GetRendInfo());
	lock_objects_.unlock();
	return out;
}

gCONTROL::gCONTROL(HWND h_wnd, unsigned int map_width, unsigned int map_height, char * host_name)
{
	h_wnd_ = h_wnd;
	InitMap(map_width, map_height);
	if (host_name == NULL)
	{
		g_net = new gNET(SERVER, "localhost");
		gOBJECT_PERSON*tmp = players[0];
		players[0] = players[1];
		players[1] = tmp;
	}
	else
	{
		g_net = new gNET(CLIENT, host_name);
	}
	GetComSecondPlayer(1);
	do_list_.AddFunc([this]() {this->CheckInFire(); });
	game_thread = std::thread(&gCONTROL::GameProcess,this);
}

gCONTROL::~gCONTROL()
{
	StopGame();
	game_thread.join();
	delete g_net;
	for (short int i = 0; i < 2; i++)
		delete players[i];
}

void gCONTROL::Command(short int player_id, char key)
{
	unsigned int x1 = 0, x2 = 0, y1 = 0, y2= 0;
	gOBJECT_PERSON * tmp_pers = players[player_id];
	tmp_pers->GetPositionIntervals(&x1, &x2, &y1, &y2);
	switch (key)
	{
	case 'w':
	{
		if ((player_id == 0) && ((clock() - move_cd_) < MOVE_CD))
			return;
		if (player_id == 0)
			move_cd_ = clock();
		if (player_id != 1)
			g_net->SendCommand('w');
		do_list_.AddFunc([tmp_pers]() {
			tmp_pers->SetMoveDirection(up);
		});
		y1++;
		if ((!objects_[TransCoord(x1, y1)].Colis()) && (!objects_[TransCoord(x2, y1)].Colis()))
		{
			do_list_.AddFunc([tmp_pers]() {tmp_pers->Move(); });
			do_list_.AddFunc(MOVE_CD/2,[tmp_pers]() {tmp_pers->Move(); });

		}
		break;
	}
	case 's':
	{
		if ((player_id == 0) && ((clock() - move_cd_) < MOVE_CD))
			return;
		if (player_id == 0)
			move_cd_ = clock();
		if (player_id != 1)
			g_net->SendCommand('s');
		do_list_.AddFunc([tmp_pers]() {
			tmp_pers->SetMoveDirection(down);
		});
		y2--;
		if ((!objects_[TransCoord(x1, y2)].Colis()) && (!objects_[TransCoord(x2, y2)].Colis()))
		{
			do_list_.AddFunc([tmp_pers]() {tmp_pers->Move(); });
			do_list_.AddFunc(MOVE_CD / 2, [tmp_pers]() {tmp_pers->Move(); });

		}
		break;
	}
	case 'a':
	{
		if ((player_id == 0) && ((clock() - move_cd_) < MOVE_CD))
			return;
		if (player_id == 0)
			move_cd_ = clock();
		if (player_id != 1)
			g_net->SendCommand('a');
		do_list_.AddFunc([tmp_pers]() {
			tmp_pers->SetMoveDirection(left);
		});
		x2--;
		if ((!objects_[TransCoord(x2, y2)].Colis()) && (!objects_[TransCoord(x2, y1)].Colis()))
		{
			do_list_.AddFunc([tmp_pers]() {tmp_pers->Move(); });
			do_list_.AddFunc(MOVE_CD / 2, [tmp_pers]() {tmp_pers->Move(); });

		}
		break;
	}
	case 'd':
	{
		if ((player_id == 0) && ((clock() - move_cd_) < MOVE_CD))
			return;
		if (player_id == 0)
			move_cd_ = clock();
		if (player_id != 1)
			g_net->SendCommand('d');
		do_list_.AddFunc([tmp_pers]() {
			tmp_pers->SetMoveDirection(right);
		});
		x1++;
		if ((!objects_[TransCoord(x1, y1)].Colis()) && (!objects_[TransCoord(x1, y2)].Colis()))
		{
			do_list_.AddFunc([tmp_pers]() {tmp_pers->Move(); });
			do_list_.AddFunc(MOVE_CD, [tmp_pers]() {tmp_pers->Move(); });
		}
		break;
	}
	case ' ':
	{
		if (player_id != 1)
			g_net->SendCommand(' ');
		objects_[TransCoord(x1, y1)].SetNewObject((gOBJECT*) new gOBJECT_BOMB(5,x1, y1));
		do_list_.AddFunc(tmp_pers->GetBombTimer(),[x1,y1,this]() {
			this->Boom(x1, y1);
		});
		break;
	}
	case 'p':
	{
		if (player_id == 0)
			g_net->SendCommand('o');
		SendMessage(h_wnd_, WM_WIN, NULL, NULL);
		StopGame();
	}
	case 'o':
	{
		if (player_id == 0)
			g_net->SendCommand('p');
		SendMessage(h_wnd_, WM_LOSE, NULL, NULL);
		StopGame();
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
	// save map sizes
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
			{
				if ((x_i % 2) && (y_i % 2))
					objects_[TransCoord(x_i, y_i)].SetNewObject((gOBJECT*) new gOBJECT_WALL(TYPE_WALLS::iron, x_i, y_i));
				else
					objects_[TransCoord(x_i, y_i)].SetNewObject((gOBJECT*) new gOBJECT_WALL(TYPE_WALLS::brick, x_i, y_i));
			}
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
	players[0] = new gOBJECT_PERSON(1, 1);
	players[1] = new gOBJECT_PERSON(map_width - 2, map_width - 2);
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
	// work processe
	while (!CheckStopStat())
	{
		do_list_.Do();
	}
}

void gCONTROL::Boom(unsigned int x, unsigned int y)
{
	Boom(x, y, objects_[TransCoord(x, y)].GetBombPower());
}

void gCONTROL::Boom(unsigned int x, unsigned int y, unsigned power)
{
	// time for delete fire
	const unsigned int fire_time = 300;
	// fire power for lines
	int power_boom[4];
	for (int i = 0; i < 4; i++)
		power_boom[i] = power;
	//add fire on lines
	if (power_boom[0] != 0)
	{
		objects_[TransCoord(x, y)].SetNewObject((gOBJECT *)new gOBJECT_FIRE(x, y));
		CELL * tmp = &objects_[TransCoord(x, y)];
		do_list_.AddFunc(fire_time, [tmp]() {tmp->Delete(); });
		for (int i = 1; i < power_boom[0]; i++)
		{
			if (objects_[TransCoord(x + i, y)].Damage(&power_boom[0]))
			{
				objects_[TransCoord(x + i, y)].SetNewObject((gOBJECT *)new gOBJECT_FIRE(x + i, y));
				CELL * tmp = &objects_[TransCoord(x + i, y)];
				do_list_.AddFunc(fire_time, [tmp]() {tmp->Delete(); });
			}
		}
		for (int i = 1; i < power_boom[1]; i++)
		{
			if (objects_[TransCoord(x - i, y)].Damage(&power_boom[1]))
			{
				objects_[TransCoord(x - i, y)].SetNewObject((gOBJECT *)new gOBJECT_FIRE(x - i, y));
				CELL * tmp = &objects_[TransCoord(x - i, y)];
				do_list_.AddFunc(fire_time, [tmp]() {tmp->Delete(); });
			}
		}
		for (int i = 1; i < power_boom[2]; i++)
		{
			if (objects_[TransCoord(x, y + i)].Damage(&power_boom[2]))
			{
				objects_[TransCoord(x, y + i)].SetNewObject((gOBJECT *)new gOBJECT_FIRE(x, y + i));
				CELL * tmp = &objects_[TransCoord(x, y + i)];
				do_list_.AddFunc(fire_time, [tmp]() {tmp->Delete(); });
			}
		}
		for (int i = 1; i < power_boom[3]; i++)
		{
			if (objects_[TransCoord(x, y - i)].Damage(&power_boom[3]))
			{
				objects_[TransCoord(x, y - i)].SetNewObject((gOBJECT *)new gOBJECT_FIRE(x, y - i));
				CELL * tmp = &objects_[TransCoord(x, y - i)];
				do_list_.AddFunc(fire_time, [tmp]() {tmp->Delete(); });
			}
		}
	}
}

void gCONTROL::GetComSecondPlayer(short id_second_player)
{
	// buffer for commands
	char * commands;
	unsigned int count_com;
	// chek for disconnect
	if (g_net->GetCommand(&commands, &count_com))
	{
		Command(0, 'p');
		return;
	}
	// chek for empy buffer
	if (commands != NULL)
	{
		// add commands
		for (unsigned int i = 0; i < count_com; i++)
			Command(id_second_player, commands[i]);
		delete[]commands;
	}
	// add this function in do list
	do_list_.AddFunc([this,id_second_player]() {
		GetComSecondPlayer(id_second_player);
	});
}

void gCONTROL::CheckInFire()
{
	unsigned x1=0, y1=0, x2=0, y2 = 0;
	players[0]->GetPositionIntervals(&x1, &x2, &y1, &y2);
	if (objects_[TransCoord(x1, y1)].IsFire() || objects_[TransCoord(x2, y2)].IsFire())
	{
		Command(0, 'o');
		return;
	}
	do_list_.AddFunc([this]() {this->CheckInFire(); });
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

bool gCONTROL::CELL::IsFire()
{
	if (object_ != NULL)
	{
		int power_fire = 1;
		object_->Destroy(&power_fire);
		if (power_fire == 1)
			return true;
	}
	return false;
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
	if (object_ == NULL)
		return true;
	object_->Destroy(power_bomb);
	if (*power_bomb >= 0)
		return true;
	return false;
}

bool gCONTROL::CELL::Colis()
{
	std::lock_guard <std::mutex> lock(lock_);
	if (object_ != NULL)
		return object_->Colis();
	else
		return false;
}

unsigned int gCONTROL::CELL::GetBombPower()
{
	if (object_ != NULL)
		return ((gOBJECT_BOMB *)object_)->GetPowerBomb();
	else
		return 0;
}
