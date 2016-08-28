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
		i_vector()
		{
			x = 0;
			y = 0;
			z = 0;
		}
		i_vector(int x_in, int y_in, int z_in)
		{
			x = x_in;
			y = y_in;
			z = z_in;
		}
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
		f_vector(i_vector i_vector_in, UINT work_width, UINT work_height, UINT work_depth = 0)
		{
			x = float(i_vector_in.x - 150) / float(work_width)*2.0f - 1.0f;
			y = (float(i_vector_in.y) / float(work_height)*2.0f - 1.0f)*(-1.0f);
			if (work_depth)
				z = float(i_vector_in.z) / float(work_depth) * (-1.0f);
		}
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
		rgb(byte r_in, byte g_in, byte b_in)
		{
			r = r_in;
			g = g_in;
			b = b_in;
		}
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
		rgba(byte r_in, byte g_in, byte b_in, byte a_in)
		{
			r = r_in;
			g = g_in;
			b = b_in;
			a = a_in;
		}
		rgba operator = (rgb in)
		{
			this->a = 255;
			this->r = in.r;
			this->g = in.g;
			this->b = in.b;
			return rgba(in.r, in.g, in.b, 255);
		}
	};
	struct tex_coord
	{
		float u, v;
		tex_coord()
		{
			u = 0.0f;
			v = 0.0f;
		}
		tex_coord(float u_in, float v_in)
		{
			u = u_in;
			v = v_in;
		}
	};

}
#endif
