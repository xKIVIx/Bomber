// this dll for load recurce for rendering

#pragma once
#ifndef vRECURCE_HEAD
#include <vector>
#include <string>
#include <vCONTROL\vOBJECT.h>
#define vRESOURCE_HEAD

#ifdef vRESOURCE_DLL
#define vRESOURCE_API _declspec(dllexport)
#else
#define vRESOURCE_API _declspec(dllimport)
#endif


class vRESOURCE_API vRESOURCE
{
public:
	virtual unsigned int LoadResourceFromFile(std::wstring name,unsigned int window_width, unsigned int window_height) = 0;
	virtual bool Select(vOBJECT info) = 0;
	 ~vRESOURCE();
protected:
	unsigned int LoadTexture(std::fstream * file);
	virtual void InitTexCoord() = 0;
	std::vector <unsigned> textures_;
	unsigned count_vertex_;
};
class vRESOURCE_API vRESOURCE_ARRAY :vRESOURCE
{
public:
	vRESOURCE_ARRAY();
	virtual unsigned int LoadResourceFromFile(std::wstring name, unsigned int window_width, unsigned int window_height);
	virtual bool Select(vOBJECT info);
	~vRESOURCE_ARRAY();
private:
	virtual void InitTexCoord();
	std::vector <std::vector<float *>> texture_coords_;
	float * vertex_;
};
class vRESOURCE_API vRESOURCE_BUFFER :vRESOURCE
{
public:
	vRESOURCE_BUFFER(OPENGL_BUFFER * opengl_buffer_);
	virtual unsigned int LoadResourceFromFile(std::wstring name, unsigned int window_width, unsigned int window_height);
	virtual bool Select(vOBJECT info);
	~vRESOURCE_BUFFER();
private:
	virtual void InitTexCoord();
	std::vector  <std::vector<unsigned>> texture_coords_;
	unsigned int vertex_;
	OPENGL_BUFFER * opengl_buffer_;
};
#endif // !vRECURCE_HEAD
