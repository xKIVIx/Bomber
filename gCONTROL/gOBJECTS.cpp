#include "gOBJECTS.h"
gOBJECT_WALL::gOBJECT_WALL(TYPE_WALLS type, unsigned int x_pos, unsigned int y_pos) : gOBJECT(x_pos,y_pos)
{
	rend_info_.sprite_now_ = 0;
	rend_info_.vertex_id_ = 0;
	rend_info_.id_sprite_mass_ = 0;
	if (type == TYPE_WALLS::iron)
	{
		def = 1000;	
		rend_info_.texture_id_ = 4;
	}
	else
	{
		def = 1;
		rend_info_.texture_id_ = 1;
	}
}

void gOBJECT_WALL::Destroy(int * power_fire)
{
		*power_fire -= def;
}

bool gOBJECT_WALL::Colis()
{
	return true;
}

gOBJECT::gOBJECT(unsigned int x_pos, unsigned int y_pos)
{
	rend_info_.x_pos_ = x_pos * 2;
	rend_info_.y_pos_ = y_pos * 2;
	rend_info_.z_pos_ = 0.5f;
}

vOBJECT gOBJECT::GetRendInfo()
{
	return rend_info_;
}

gOBJECT_PERSON::gOBJECT_PERSON(unsigned int x_pos, unsigned int y_pos) : gOBJECT(x_pos, y_pos)
{
	speed_ = 1;
	rend_info_.sprite_now_ = 0;
	rend_info_.vertex_id_ = 0;
	rend_info_.id_sprite_mass_ = 2;
	rend_info_.texture_id_ = 14;
}

void gOBJECT_PERSON::Move()
{
	std::lock_guard <std::mutex> lock(lock_);
	if (rend_info_.sprite_now_ >=2)
		rend_info_.sprite_now_ = 0;
	else
		rend_info_.sprite_now_ ++;
	switch (move_direction_)
	{
	case MOVE_DIRECTION::stop: 
	{
		break;
	};
	case MOVE_DIRECTION::up: 
	{
		rend_info_.dir_angl_ = 0;
		rend_info_.y_pos_ += speed_;
		break;
	}
	case MOVE_DIRECTION::down:
	{
		rend_info_.dir_angl_ = 180;
		rend_info_.y_pos_ -= speed_;
		break;
	}
	case MOVE_DIRECTION::right:
	{
		rend_info_.dir_angl_ = 270;
		rend_info_.x_pos_ += speed_;
		break;
	}
	case MOVE_DIRECTION::left:
	{
		rend_info_.dir_angl_ = 90;
		rend_info_.x_pos_ -= speed_;
		break;
	}
	default:
	{
		break;
	}
	}
}

void gOBJECT_PERSON::SetMoveDirection(MOVE_DIRECTION dir)
{
	std::lock_guard <std::mutex> lock(lock_);
	move_direction_ = dir;
}

void gOBJECT_PERSON::GetPositionIntervals(unsigned int * x1, unsigned int * x2, unsigned int * y1, unsigned int * y2)
{
	std::lock_guard <std::mutex> lock(lock_);
	//get x interval
	*x1 = unsigned int(rend_info_.x_pos_/2);
	if (float(*x1) < (rend_info_.x_pos_/2))
		*x2 = *x1 + 1;
	else
		*x2 = *x1;
	//get y interval
	*y1 = unsigned int(rend_info_.y_pos_/2);
	if (float(*y1) < (rend_info_.y_pos_/2))
		*y2 = *y1 + 1;
	else
		*y2 = *y1;
}

bool gOBJECT_PERSON::IsLive()
{
	std::lock_guard <std::mutex> lock(lock_);
	return live_;
}

vOBJECT gOBJECT_PERSON::GetRendInfo()
{
	std::lock_guard <std::mutex> lock(lock_);
	return rend_info_;
}

void gOBJECT_PERSON::Destroy(int * power_fire)
{
	std::lock_guard <std::mutex> lock(lock_);
	live_ = 0;
}

bool gOBJECT_PERSON::Colis()
{
	return true;
}

unsigned int gOBJECT_PERSON::GetBombTimer()
{
	return bomb_timer_;
}

void gOBJECT_FIRE::Destroy(int * power_fire)
{
}

bool gOBJECT_FIRE::Colis()
{
	return false;
}

gOBJECT_FIRE::gOBJECT_FIRE(unsigned int x_pos, unsigned int y_pos) : gOBJECT (x_pos,y_pos)
{
	rend_info_.sprite_now_ = 0;
	rend_info_.vertex_id_ = 0;
	rend_info_.id_sprite_mass_ = 0;
	rend_info_.texture_id_ = 3;
}

gOBJECT_BOMB::gOBJECT_BOMB(unsigned int power_bomb, unsigned int x_pos, unsigned int y_pos) : gOBJECT(x_pos, y_pos)
{
	power_bomb_ = power_bomb;
	rend_info_.sprite_now_ = 0;
	rend_info_.vertex_id_ = 0;
	rend_info_.id_sprite_mass_ = 0;
	rend_info_.texture_id_ = 3;
	rend_info_.texture_id_ = 0;
}

void gOBJECT_BOMB::Destroy(int * power_fire)
{
	*power_fire -= 1000;
}

bool gOBJECT_BOMB::Colis()
{
	return true;
}

unsigned int gOBJECT_BOMB::GetPowerBomb()
{
	return power_bomb_;
}
