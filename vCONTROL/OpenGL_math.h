#pragma once
#ifndef _OPENGL_MATH_HEADER
#define _OPENGL_MATH_HEADER
#include <Windows.h>
#include <math.h>
namespace OPENGL_MATH
{
	struct i_vector
	{
		int x, y, z;
		i_vector();
		i_vector(int x_in, int y_in, int z_in);
	};
	typedef i_vector i_vertex;
	struct f_vector
	{
		float x = 0.0f, y = 0.0f, z = 0.0f;
		f_vector()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		f_vector(float x_in, float y_in, float z_in);
		f_vector(i_vector i_vector_in, UINT work_width, UINT work_height, UINT work_depth = 0);
	};
	typedef f_vector f_vertex;
	struct rgb
	{
		byte r, g, b;
		rgb()
		{
			r = 0;
			g = 0;
			b = 0;
		};
		rgb(byte r_in, byte g_in, byte b_in);
	};
	struct rgba
	{
		byte r, g, b, a;
		rgba()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 0;
		}
		rgba(byte r_in, byte g_in, byte b_in, byte a_in);
		rgba operator = (rgb in);
	};
	struct tex_coord
	{
		float u, v;
		tex_coord()
		{
			u = 0.0f;
			v = 0.0f;
		}
		tex_coord(float u_in, float v_in);
	};
	f_vector NormalizeVector(f_vector vect);
}
#endif
