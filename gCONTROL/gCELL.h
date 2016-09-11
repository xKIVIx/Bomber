#pragma once
#include <mutex>
#include "gOBJECT.h"
class gCELL
{
public:
	gCELL(unsigned int x, unsigned int y);
	~gCELL();
	void SwapObject(gCELL * source);
	void SetObject(gOBJECT * new_object);
	gOBJECT * GetObject();
private:
	std::mutex lock_cell;
	float x_pos_, y_pos_, z_pos_;
	gOBJECT * object_;
	void DeleteObject();
};
