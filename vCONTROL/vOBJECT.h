#pragma once
#ifndef _vOBJECT_HEADER
#define _vOBJECT_HEADER

#include <mutex>
#include "OpenGL_math.h"

struct vOBJECT
{
	unsigned int recurce_id_;
	OPENGL_MATH::f_vector view_direction_, position_;

};
#endif