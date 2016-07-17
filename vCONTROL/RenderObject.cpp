#include "RenderObject.h"
void OBJECT_FOR_REND::Draw(OPENGL_BUFFER * opengl_buffer)
{
	opengl_buffer->BindBuffer(GL_ARRAY_BUFFER_ARB, vertex_buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, (char*)NULL);
	opengl_buffer->BindBuffer(GL_ARRAY_BUFFER_ARB, texture_coord_buffer_id);
	glTexCoordPointer(2, GL_FLOAT, 0, (char*)NULL);
	glBindTexture(GL_TEXTURE_2D, texture_buffer_id);
	glDrawArrays(GL_TRIANGLE_FAN, 0, count_vert);
}

void OBJECT_FOR_REND::Delete(OPENGL_BUFFER * opengl_buffer)
{
	opengl_buffer->DeleteBuffers(1, &vertex_buffer_id);
	glDeleteTextures(1, &texture_buffer_id);
	opengl_buffer->DeleteBuffers(1, &texture_coord_buffer_id);
	vertex_buffer_id = 0;
	texture_buffer_id = 0;
	texture_coord_buffer_id = 0;
}

OBJECT_FOR_REND::OBJECT_FOR_REND(float * vertex, float * texture_coord, unsigned int count_vertex_in, unsigned char * pixels, unsigned int texture_width, unsigned int texture_height, OPENGL_BUFFER * opengl_buffer)
{
	New(vertex, texture_coord, count_vertex_in, pixels, texture_width, texture_height, opengl_buffer);
}

OBJECT_FOR_REND::~OBJECT_FOR_REND()
{
}

void OBJECT_FOR_REND::New(float * vertex, float * texture_coord, unsigned int count_vertex_in, unsigned char * pixels, unsigned int texture_width, unsigned int texture_height, OPENGL_BUFFER * opengl_buffer)
{
	if (vertex_buffer_id)
		Delete(opengl_buffer);
	// gen ids for buffers
	vertex_buffer_id = *opengl_buffer->GenBeffers(1);
	texture_coord_buffer_id = *opengl_buffer->GenBeffers(1);
	glGenTextures(1, &texture_buffer_id);
	//load vertex
	opengl_buffer->BindBuffer(GL_ARRAY_BUFFER_ARB, vertex_buffer_id);
	opengl_buffer->AddBufferData(GL_ARRAY_BUFFER_ARB, count_vertex_in * 3 * sizeof(float), vertex, GL_DYNAMIC_DRAW);
	//load texture coord
	opengl_buffer->BindBuffer(GL_ARRAY_BUFFER_ARB, texture_coord_buffer_id);
	opengl_buffer->AddBufferData(GL_ARRAY_BUFFER_ARB, count_vertex_in * 2 * sizeof(float), texture_coord, GL_DYNAMIC_DRAW);
	//load texture
	glBindTexture(GL_TEXTURE_2D, texture_buffer_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}
