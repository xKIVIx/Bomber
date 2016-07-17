#pragma once
#include <Main\includs.h>
#include "OpenGl.h"
#include "glext.h"
#include "OpenGL_math.h"
class OBJECT_FOR_REND
{
private:
	unsigned int vertex_buffer_id = 0, texture_buffer_id = 0, texture_coord_buffer_id = 0, count_vert = 0;
	OPENGL_MATH::f_vector world_position;
public:
	void Draw(OPENGL_BUFFER * opengl_buffer);
	void Delete(OPENGL_BUFFER * opengl_buffer);
	OBJECT_FOR_REND(float * vertex, 
		float * texture_coord,
		unsigned int count_vertex,  
		unsigned char * pixels, 
		unsigned int texture_width, 
		unsigned int texture_height, 
		OPENGL_BUFFER * opengl_buffer);
	~OBJECT_FOR_REND();
	void New(float * vertex,
		float * texture_coord,
		unsigned int count_vertex_in,
		unsigned char * pixels,
		unsigned int texture_width,
		unsigned int texture_height,
		OPENGL_BUFFER * opengl_buffer);
};