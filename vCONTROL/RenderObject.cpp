#include "RenderObject.h"
void OBJECT_FOR_REND::Draw(OPENGL_BUFFER * opengl_buffer)
{
	if (vertex_buffer_id&&texture_buffer_id&&texture_coord_buffer_id)
	{
		opengl_buffer->BindBuffer(GL_ARRAY_BUFFER_ARB, vertex_buffer_id);
		glVertexPointer(3, GL_FLOAT, 0, (char*)NULL);
		opengl_buffer->BindBuffer(GL_ARRAY_BUFFER_ARB, texture_coord_buffer_id);
		glTexCoordPointer(2, GL_FLOAT, count_vert*sizeof(OPENGL_MATH::tex_coord)*sprite, (char*)NULL);
		glBindTexture(GL_TEXTURE_2D, texture_buffer_id);
		glDrawArrays(GL_TRIANGLE_FAN, 0, count_vert);
	}
}
void OBJECT_FOR_REND::SetWorldPosition(OPENGL_MATH::f_vector new_pos)
{
	world_position = new_pos;
}

void OBJECT_FOR_REND::SetViewDirection(OPENGL_MATH::f_vector new_direction)
{
	view_direction = new_direction;
}

void OBJECT_FOR_REND::SelectSpriteImage(UINT sprite_id)
{
	sprite = sprite_id;
}


OBJECT_FOR_REND::OBJECT_FOR_REND(UINT vertex_buffer, UINT tex_coord_buffer, UINT count_vertex,UINT tex_buffer)
{
	New(vertex_buffer,
		tex_coord_buffer,
		count_vertex,
		tex_buffer);
}

void OBJECT_FOR_REND::New(UINT vertex_buffer, UINT tex_coord_buffer,UINT count_vertex ,UINT tex_buffer)
{
	vertex_buffer_id = vertex_buffer;
	texture_coord_buffer_id = tex_coord_buffer;
	count_vert = count_vertex;
	texture_buffer_id = tex_buffer;
}
