#include "GUI.h"

mCONTROL::mCONTROL(HWND h_wnd)
{
	h_wnd_ = h_wnd;
	SetMain();
}

mCONTROL::~mCONTROL()
{
	delete g_control;
}

bool mCONTROL::CheckStopState()
{
	std::lock_guard <std::mutex> lock(lock_stop_stat_);
	return stop;
}

void mCONTROL::Stop()
{
	std::lock_guard <std::mutex> lock(lock_stop_stat_);
	stop = 1;
}

void mCONTROL::DoCommand()
{
	char tmp;
	while (!CheckStopState())
	{
		lock_do_list_.lock();
		if (do_list_.size() > 0)
		{
			tmp = do_list_.front();
			do_list_.pop_front();
			lock_do_list_.unlock();
			Comand(tmp);
		}
		lock_do_list_.unlock();
	}
}

void mCONTROL::AddCom(char key)
{
	std::lock_guard <std::mutex> lock(lock_do_list_);
	do_list_.push_back(key);
}

void mCONTROL::Comand(char key)
{
	switch (now_state_)
	{
	case GAME:
	{
		if (key == 'e')
		{
			SetSecondMenu();
		}
		else
			g_control->Command(0,key);
		return;
	}
	case MAIN:
	{
		if (key == 'w')
		{
			std::lock_guard <std::mutex> lock_(lock_objects_);
			menu_objects_[now_menu_select_].Unselect();
			do
			{
				now_menu_select_--;
				if (now_menu_select_ < 0)
					now_menu_select_ = menu_objects_.size() - 1;
			} while (!menu_objects_[now_menu_select_].Select());
			return;
		}
		if (key == 's')
		{
			std::lock_guard <std::mutex> lock_(lock_objects_);
			menu_objects_[now_menu_select_].Unselect();
			do
			{
				now_menu_select_++;
				if (now_menu_select_ >= menu_objects_.size())
					now_menu_select_ = 0;
			} while (!menu_objects_[now_menu_select_].Select());
			return;
		}
		if (key == ' ')
		{
				menu_objects_[now_menu_select_].Use();
				return;
		}
		if (key == 'e')
		{
			Close();
		}
		return;
	}
	case SEC_MENU:
	{
		switch (key)
		{
		case 'e':
		{
			SetGame(NULL);
		}
		case 'd':
		{
			std::lock_guard <std::mutex> lock_(lock_objects_);
			menu_objects_[now_menu_select_].Unselect();
			do
			{
				now_menu_select_++;
				if (now_menu_select_ >= menu_objects_.size())
					now_menu_select_ = 0;
			} while (!menu_objects_[now_menu_select_].Select());
			return;
		}
		case 'a':
		{
			std::lock_guard <std::mutex> lock_(lock_objects_);
			menu_objects_[now_menu_select_].Unselect();
			do
			{
				now_menu_select_--;
				if (now_menu_select_ < 0)
					now_menu_select_ = menu_objects_.size() - 1;
			} while (!menu_objects_[now_menu_select_].Select());
			return;
		}
		defalut: 
		{
			return;
		}
		}
	}
	case WIN:
	{
		SetMain();
	}
	case LOSE:
	{
		SetMain();
	}
	}
}

std::vector<vOBJECT> mCONTROL::GetRendInfo()
{
	std::vector <vOBJECT> out;
	std::lock_guard <std::mutex> lock_(lock_objects_);
	if ((now_state_ == GAME) || (now_state_ == SEC_MENU))
		out = g_control->GetObjectsForRend();
	for (auto i = menu_objects_.begin(); i < menu_objects_.end(); i++)
		out.push_back(i->GetRendInfo());
	return out;
}

mCONTROL::MENU_OBJECT::MENU_OBJECT()
{
}

mCONTROL::MENU_OBJECT::MENU_OBJECT(float x_pos,
	float y_pos,
	float z_pos,
	float x_size,
	float y_size,
	unsigned texture_id,
	std::function <void()> what_do)
{
	// fill rend info
	rend_info_.game_object = 0;
	rend_info_.id_sprite_mass_ = 1;
	rend_info_.sprite_now_ = 0;
	rend_info_.texture_id_ = texture_id;
	rend_info_.x_pos_ = x_pos;
	rend_info_.y_pos_ = y_pos;
	rend_info_.z_pos_ = z_pos;
	rend_info_.scale_cof_x = x_size;
	rend_info_.scale_cof_y = y_size;
	// other data
	usable_ = 1;
	func_ = what_do;
}

mCONTROL::MENU_OBJECT::MENU_OBJECT(float x_pos, float y_pos, float z_pos, float x_size, float y_size, unsigned texture_id)
{
	// fill rend info
	rend_info_.game_object = 0;
	rend_info_.id_sprite_mass_ = 0;
	rend_info_.sprite_now_ = 0;
	rend_info_.texture_id_ = texture_id;
	rend_info_.x_pos_ = x_pos;
	rend_info_.y_pos_ = y_pos;
	rend_info_.z_pos_ = z_pos;
	rend_info_.scale_cof_x = x_size;
	rend_info_.scale_cof_y = y_size;
	// other data
	usable_ = 0;
}

vOBJECT mCONTROL::MENU_OBJECT::GetRendInfo()
{
	return rend_info_;
}

bool mCONTROL::MENU_OBJECT::Select()
{
	if (usable_)
	{
		rend_info_.sprite_now_ = 1;
		return 1;
	}
	return 0;
}

void mCONTROL::MENU_OBJECT::Unselect()
{
	rend_info_.sprite_now_ = 0;
}

void mCONTROL::MENU_OBJECT::Use()
{
	if (usable_)
		func_();
}

void mCONTROL::SetMain()
{
	std::vector <MENU_OBJECT> tmp;
	// host
	tmp.push_back(MENU_OBJECT(0.5, 1.6, 0.0, 0.3, 0.2, 7, [this]() {this->SetGame(NULL); }));
	// client
	tmp.push_back(MENU_OBJECT(0.5, 1.2, 0.0, 0.3, 0.2, 5, [this]() {this->SetGame("localhost"); }));
	// exit
	tmp.push_back(MENU_OBJECT(0.5, 0.8, 0.9, 0.3, 0.2, 6, [this]() {this->Close(); }));
	std::lock_guard <std::mutex> lock(lock_objects_);
	if (g_control != NULL)
		delete g_control;
	now_state_ = MAIN;
	menu_objects_.swap(tmp);
}

void mCONTROL::SetSecondMenu()
{
	std::vector <MENU_OBJECT> tmp;
	// is exit?
	tmp.push_back(MENU_OBJECT (0.3,0.6,0,0.3,0.3,8));
	// no
	tmp.push_back(MENU_OBJECT(0.3, 0.3, 0, 0.3, 0.3, 10, [this]() {this->SetGame(NULL); }));
	// yes
	tmp.push_back(MENU_OBJECT(0.6, 0.3, 0, 0.2, 0.2, 13, [this]() {this->Close(); }));
	std::lock_guard <std::mutex> lock(lock_objects_);
	now_state_ = SEC_MENU;
	menu_objects_.swap(tmp);

}

void mCONTROL::SetGame(char * host_name)
{
	std::vector <MENU_OBJECT> tmp;
	if (g_control == NULL)
	{
		SetLoad();
		g_control = new gCONTROL(h_wnd_, 21, 21, host_name);
	}
	std::lock_guard <std::mutex> lock(lock_objects_);
	now_state_ = GAME;
	menu_objects_.swap(tmp);
}

void mCONTROL::SetLoad()
{
	std::vector <MENU_OBJECT> tmp;
	std::lock_guard <std::mutex> lock(lock_objects_);
	tmp.push_back(MENU_OBJECT(1.8, 0.2, 0.0, 0.2, 0.2, 11));
	now_state_ = LOAD;
	menu_objects_.swap(tmp);
}

void mCONTROL::SetWin()
{

}

void mCONTROL::SetLose()
{
}

void mCONTROL::Close()
{
	SendMessage(h_wnd_, WM_DESTROY, NULL, NULL);
}
