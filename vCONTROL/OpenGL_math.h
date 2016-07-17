#pragma once
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
		f_vector();
		f_vector(float x_in, float y_in, float z_in);
		f_vector(i_vector i_vector_in, UINT work_width, UINT work_height, UINT work_depth = 0);
	};
	typedef f_vector f_vertex;
	struct rgb
	{
		byte r, g, b;
		rgb();
		rgb(byte r_in, byte g_in, byte b_in);
	};
	struct rgba
	{
		byte r, g, b, a;
		rgba();
		rgba(byte r_in, byte g_in, byte b_in, byte a_in);
		rgba operator = (rgb in);
	};
	struct tex_coord
	{
		float u, v;
		tex_coord();
		tex_coord(float u_in, float v_in);
	};
}