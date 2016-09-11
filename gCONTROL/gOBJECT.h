#pragma once
#ifndef _gOBJECT_HEADER
#define _gOBJECT_HEADER

#include <vCONTROL\vOBJECT.h>
#include <mutex>
class gOBJECT
{
public:
	//chek destroing object
	 bool Destroy(unsigned int * power_fire);
	//chek colis object
	virtual bool Colis(gOBJECT * contact_object) = 0;
	vOBJECT GetRendInfo();
	void SetNewCoord(float x, float y, float z);
protected:
	bool ghost = 0;
	unsigned short int def;
	vOBJECT render_info_;
	std::mutex lock_object_;
};
enum TYPE_WALL
{
	iron,
	brick
};
class gOBJECT_WALL :gOBJECT
{
public:
	gOBJECT_WALL(TYPE_WALL type_wall, float x_pos, float y_pos, float z_pos);
	virtual bool Colis(gOBJECT * contact_object);

};
class gOBJECT_PERSON :gOBJECT
{
public:
	gOBJECT_PERSON(float x_pos, float y_pos, float z_pos);
	virtual bool  Colis(gOBJECT * contact_object);

};
#endif