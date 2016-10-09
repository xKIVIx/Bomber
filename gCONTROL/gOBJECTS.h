// this is intefaces for game objects

#pragma once
#include <mutex>
#include <vCONTROL\vOBJECT.h>
class gOBJECT
{
public:
	gOBJECT(unsigned int x_pos, unsigned int y_pos);
	virtual vOBJECT GetRendInfo();
	virtual void Destroy(int * power_fire)=0;
	virtual bool Colis() = 0;
protected:
	vOBJECT rend_info_;
};
enum TYPE_WALLS
{
	iron,
	brick
};
class gOBJECT_WALL:gOBJECT
{
public:
	gOBJECT_WALL(TYPE_WALLS type, unsigned int x_pos, unsigned int y_pos);
	virtual void Destroy(int * power_fire);
	virtual bool Colis();
private:
	unsigned int def;
};
enum MOVE_DIRECTION
{
	stop,
	up,
	down,
	right,
	left
};
class gOBJECT_PERSON :gOBJECT
{
public:
	gOBJECT_PERSON(unsigned int x_pos, unsigned int y_pos);
	void Move();
	void SetMoveDirection(MOVE_DIRECTION dir);
	// get inteval where object placed
	void GetPositionIntervals(unsigned int * x1,
		unsigned int  * x2,
		unsigned int * y1,
		unsigned int * y2);
	bool IsLive();
	virtual vOBJECT GetRendInfo();
	virtual void Destroy(int * power_fire);
	virtual bool Colis();
	unsigned int GetBombTimer();
private:
	unsigned move_cd_ = 0;
	std::mutex lock_;
	MOVE_DIRECTION move_direction_;
	bool live_ = 1;
	float speed_;
	unsigned int bomb_timer_ = 3000;
};
class gOBJECT_FIRE :gOBJECT
{
public:
	virtual void Destroy(int * power_fire);
	virtual bool Colis();
	gOBJECT_FIRE(unsigned int x_pos, unsigned int y_pos);
};
class gOBJECT_BOMB :gOBJECT
{
public:
	gOBJECT_BOMB(unsigned int power_bomb,unsigned int x_pos, unsigned int y_pos);
	virtual void Destroy(int * power_fire);
	virtual bool Colis();
	unsigned int GetPowerBomb();
private:
	unsigned int power_bomb_;
};