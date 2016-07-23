#pragma once
#ifndef RENDER_OBJECT_HEADER
#define RENDER_OBJECT_HEADER
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "glext.h"
#include "OPENGL_BUFFER.h"
#include "OpenGL_math.h"
class OBJECT_FOR_REND
{
private:
	UINT vertex_buffer_id = 0,
		texture_buffer_id = 0,
		texture_coord_buffer_id = 0,
		count_vert = 0,
		sprite = 0;
	OPENGL_MATH::f_vector world_position, view_direction;
public:
	void Draw(OPENGL_BUFFER * opengl_buffer);
	void SetWorldPosition(OPENGL_MATH::f_vector new_pos);
	void SetViewDirection(OPENGL_MATH::f_vector new_direction);
	void SelectSpriteImage(UINT sprite_id);
	OBJECT_FOR_REND(UINT vertex_buffer, UINT tex_coord_buffer, UINT count_vertex, UINT tex_buffer);
	void New(UINT vertex_buffer, UINT tex_coord_buffer, UINT count_vertex, UINT tex_buffer);
};
#endif // !RENDER_OBJECT_HEADER
