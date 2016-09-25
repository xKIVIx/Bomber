
#define vRESOURCE_DLL
#define MAX_COUNT_SPRITES 6
#define COUNT_BUFERS_FOR_SPRITES 21
#include <fstream>
#include <Windows.h>
#include <gl\GL.h>
#include <Opengl_buffer\OPENGL_BUFFER.h>
#include "vRESOURCE.h"
vRESOURCE_ARRAY::vRESOURCE_ARRAY()
{
	InitTexCoord();
}
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

bool vRESOURCE_ARRAY::Select(vOBJECT info)
{
	if (!(
		(info.sprite_now_ <= info.id_sprite_mass_) &&
		(info.id_sprite_mass_ < texture_coords_.size()) &&
		(info.texture_id_ < textures_.size())))
		return 0;
	glVertexPointer(3, GL_FLOAT, 0, vertex_);
	glTexCoordPointer(2, GL_FLOAT, 8 * info.sprite_now_, texture_coords_[info.id_sprite_mass_][info.sprite_now_]);
	glBindTexture(GL_TEXTURE_2D, textures_[info.texture_id_]);
	return 1;
}

vRESOURCE_ARRAY::~vRESOURCE_ARRAY()
{
	delete [] vertex_;
	for (auto i = texture_coords_.begin(); i < texture_coords_.end();i++)
		for (auto k = i->begin(); k < i->end(); k++)
			delete (*k._Ptr);
}

void vRESOURCE_ARRAY::InitTexCoord()
{
	std::vector <float *> tmp_vec;
	float * tmp_coord;
	for (unsigned i = 0; i < MAX_COUNT_SPRITES; i++)
	{
		tmp_vec.clear();
		float cof = 1.0f / float(i + 1);
		for (unsigned int k = 0; k < (i + 1); k++)
		{
			tmp_coord = new float[8];
			//bot left
			tmp_coord[0] = k*cof;
			tmp_coord[1] = 0.0f;
			//top left
			tmp_coord[2] = k*cof;
			tmp_coord[3] = 1.0f;
			//top right
			tmp_coord[4] = (k + 1)*cof;
			tmp_coord[5] = 1.0f;
			//bot tight
			tmp_coord[6] = (k + 1)*cof;
			tmp_coord[7] = 0.0f;
			tmp_vec.push_back(tmp_coord);
		};
		texture_coords_.push_back(tmp_vec);
	}
}

vRESOURCE_BUFFER::vRESOURCE_BUFFER(OPENGL_BUFFER * opengl_buf)
{
	opengl_buffer_ = opengl_buf;
	InitTexCoord();
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

bool vRESOURCE_BUFFER::Select(vOBJECT info)
{
	if (!(
		(info.sprite_now_ <= info.id_sprite_mass_) &&
		(info.id_sprite_mass_ < texture_coords_.size()) &&
		(info.texture_id_ < textures_.size())))
		return 0;
	opengl_buffer_->BindBuffer(GL_ARRAY_BUFFER_ARB,vertex_);
	glVertexPointer(3, GL_FLOAT, 0, (char*)NULL);
	opengl_buffer_->BindBuffer(GL_ARRAY_BUFFER_ARB,texture_coords_[info.id_sprite_mass_][info.sprite_now_]);
	glTexCoordPointer(2, GL_FLOAT,0, (char*)NULL);
	glBindTexture(GL_TEXTURE_2D, textures_[info.texture_id_]);
	return 1;
}

vRESOURCE_BUFFER::~vRESOURCE_BUFFER()
{
	opengl_buffer_->DeleteBuffers(1, &vertex_);
	for (auto i = texture_coords_.begin(); i < texture_coords_.end(); i++)
		for (auto k = i->begin(); k < i->end();k++)
			opengl_buffer_->DeleteBuffers(1,k._Ptr);
}

void vRESOURCE_BUFFER::InitTexCoord()
{
	std::vector <unsigned> out;
	unsigned * tmp_ids = opengl_buffer_->GenBeffers(COUNT_BUFERS_FOR_SPRITES);
	unsigned used_id = 0;
	float tmp_coord [8];
	for (unsigned i = 0; i < MAX_COUNT_SPRITES; i++)
	{
		out.clear();
		float cof = 1.0f / float(i + 1);
		for (unsigned int k = 0; k < (i + 1); k++)
		{
			//bot left
			tmp_coord[0] = k*cof;
			tmp_coord[1] = 0.0f;
			//top left
			tmp_coord[2] = k*cof;
			tmp_coord[3] = 1.0f;
			//top right
			tmp_coord[4] = (k + 1)*cof;
			tmp_coord[5] = 1.0f;
			//bot tight
			tmp_coord[6] = (k + 1)*cof;
			tmp_coord[7] = 0.0f;
			opengl_buffer_->BindBuffer(GL_ARRAY_BUFFER_ARB, tmp_ids[used_id]);
			opengl_buffer_->AddBufferData(GL_ARRAY_BUFFER_ARB, sizeof(float)*8, tmp_coord, GL_STATIC_DRAW_ARB);
			out.push_back(tmp_ids[used_id]);
			used_id++;
		};
		texture_coords_.push_back(out);
	}
	delete[] tmp_ids;
}
vRESOURCE::~vRESOURCE()
{
	for (auto i = textures_.begin(); i < textures_.end();i++)
		glDeleteTextures(1, i._Ptr);
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
	unsigned tmp_id = 0;
	glGenTextures(1, &tmp_id);
	glBindTexture(GL_TEXTURE_2D, tmp_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, texture_size[0], texture_size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp_texture);
	textures_.push_back(tmp_id);
	delete[] tmp_texture;
	return count_sprites;
}