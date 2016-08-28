
#define vRESOURCE_DLL

#include <fstream>
#include <Windows.h>
#include <gl\GL.h>
#include <Opengl_buffer\OPENGL_BUFFER.h>
#include "vRESOURCE.h"
unsigned int vRESOURCE_ARRAY::LoadResourceFromFile(std::wstring name, unsigned int window_width, unsigned int window_height)
{
	// open file
	std::fstream file(name, std::ios::binary | std::ios::in);
	// read recurce id 
	unsigned int recurce_id;
	file.read((char *)&recurce_id, sizeof(int));
	// read count vertex
	file.read((char *)&count_vertex_, sizeof(int));
	// set memory for vertex
	vertex_ = new float[count_vertex_];
	// read vertex 
	file.read((char*)vertex_, count_vertex_*sizeof(float));
	// load texture in buffer
	LoadTexture(&file);
	return recurce_id;
}

void vRESOURCE_ARRAY::Select()
{
	glVertexPointer(3, GL_FLOAT, 0, vertex_);
	glBindTexture(GL_TEXTURE_2D, texture_);
}

vRESOURCE_ARRAY::~vRESOURCE_ARRAY()
{
	delete [] vertex_;
}

unsigned int vRESOURCE_BUFFER::LoadResourceFromFile(std::wstring name, unsigned int window_width, unsigned int window_height)
{
	if (opengl_buffer_ != 0)
	{
		// open file
		std::fstream file (name, std::ios::binary | std::ios::in);
		// read recurce id 
		unsigned int recurce_id;
		file.read((char *)&recurce_id, sizeof(int));
		// read count vertex
		file.read((char *)&count_vertex_, sizeof(int));
		// set memory for vertex
		float * tmp_vertex = new float[count_vertex_*3];
		// read vertex 
		for (int i = 0; i < count_vertex_*3;i++)
			file.read((char*)&(tmp_vertex[i]),sizeof(float));
		// load vertex in buffer
		vertex_ = *opengl_buffer_->GenBeffers(1);
		opengl_buffer_->BindBuffer(GL_ARRAY_BUFFER_ARB, vertex_);
		opengl_buffer_->AddBufferData(GL_ARRAY_BUFFER_ARB, count_vertex_*sizeof(float) * 3, tmp_vertex, GL_STATIC_DRAW_ARB);
		// load texture in buffer
		LoadTexture(&file);
		file.close();
		return recurce_id;
	}
	else
		return 0;
}

void vRESOURCE_BUFFER::Select()
{
	opengl_buffer_->BindBuffer(GL_ARRAY_BUFFER_ARB,vertex_);
	glVertexPointer(3, GL_FLOAT, 0, (char*)NULL);
	glBindTexture(GL_TEXTURE_2D, texture_);
}

void vRESOURCE_BUFFER::SetOpenglBufferEx(OPENGL_BUFFER * opengl_buffer)
{
	opengl_buffer_ = opengl_buffer;
}

vRESOURCE_BUFFER::~vRESOURCE_BUFFER()
{
	opengl_buffer_->DeleteBuffers(1, &vertex_);
}

vRESOURCE::~vRESOURCE()
{
	glDeleteTextures(1, &texture_);
}

unsigned int vRESOURCE::LoadTexture(std::fstream * file)
{
	// read count sprites
	unsigned int count_sprites = 0;
	file->read((char*)&count_sprites, sizeof(int));
	// read size texture
	unsigned int texture_size[2];
	file->read((char*)texture_size, 2 * sizeof(int));
	// read texture
	unsigned char * tmp_texture = new unsigned char[texture_size[0] * texture_size[1]*4];
	file->read((char*)tmp_texture, 4 * texture_size[0] * texture_size[1] * sizeof(unsigned char));
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_2D, texture_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, texture_size[0], texture_size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp_texture);
	return count_sprites;
}
