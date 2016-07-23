
#include "OpenGL_math.h"


inline OPENGL_MATH::f_vector::f_vector(float x_in, float y_in, float z_in)
{
	x = x_in;
	y = y_in;
	z = z_in;
}

inline OPENGL_MATH::f_vector::f_vector(i_vector i_vector_in, UINT work_width, UINT work_height, UINT work_depth)
{
	x = float(i_vector_in.x - 150) / float(work_width)*2.0f - 1.0f;
	y = (float(i_vector_in.y) / float(work_height)*2.0f - 1.0f)*(-1.0f);
	if (work_depth)
		z = float(i_vector_in.z) / float(work_depth) * (-1.0f);
}

inline OPENGL_MATH::rgb::rgb(byte r_in, byte g_in, byte b_in)
{
	r = r_in;
	g = g_in;
	b = b_in;
}

inline OPENGL_MATH::rgba::rgba(byte r_in, byte g_in, byte b_in, byte a_in)
{
	r = r_in;
	g = g_in;
	b = b_in;
}

inline OPENGL_MATH::rgba OPENGL_MATH::rgba::operator=(rgb in)
{
	this->a = 255;
	this->r = in.r;
	this->g = in.g;
	this->b = in.b;
	return rgba(in.r, in.g, in.b, 255);
}

inline OPENGL_MATH::tex_coord::tex_coord(float u_in, float v_in)
{
	u = u_in;
	v = v_in;
}

inline OPENGL_MATH::i_vector::i_vector()
{
	x = 0;
	y = 0;
	z = 0;
}

inline OPENGL_MATH::i_vector::i_vector(int x_in, int y_in, int z_in)
{
	x = x_in;
	y = y_in;
	z = z_in;
}

inline OPENGL_MATH::f_vector OPENGL_MATH::NormalizeVector(f_vector vect)
{
	float size = sqrtf(vect.x*vect.x + vect.y*vect.y + vect.z*vect.z);
	vect.x = vect.x / size;
	vect.y = vect.y / size;
	vect.z = vect.z / size;
	return vect;
}
