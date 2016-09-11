// this dll for load recurce for rendering

#pragma once
#ifndef vRECURCE_HEAD

#include <string>

#define vRESOURCE_HEAD

#ifdef vRESOURCE_DLL
#define vRESOURCE_API _declspec(dllexport)
#else
#define vRESOURCE_API _declspec(dllimport)
#endif

unsigned int vRESOURCE_API InitTexCoord(float * tmp, void * opengl);
void vRESOURCE_API SelectTextureCoord (unsigned int id, void * opengl);
class vRESOURCE_API vRESOURCE
{
public:
	virtual unsigned int LoadResourceFromFile(std::wstring name,unsigned int window_width, unsigned int window_height) = 0;
	virtual void Select() = 0;
	 ~vRESOURCE();
protected:
	unsigned int LoadTexture(std::fstream * file);
	unsigned int texture_, count_vertex_;
};
class vRESOURCE_API vRESOURCE_ARRAY :vRESOURCE
{
public:
	virtual unsigned int LoadResourceFromFile(std::wstring name, unsigned int window_width, unsigned int window_height);
	virtual void Select();
	~vRESOURCE_ARRAY();
private:
	float * vertex_;
};
class vRESOURCE_API vRESOURCE_BUFFER :vRESOURCE
{
public:
	virtual unsigned int LoadResourceFromFile(std::wstring name, unsigned int window_width, unsigned int window_height);
	virtual void Select();
	void SetOpenglBufferEx(OPENGL_BUFFER * opengl_buffer);
	~vRESOURCE_BUFFER();
private:
	unsigned int vertex_;
	OPENGL_BUFFER * opengl_buffer_;
};
#endif // !vRECURCE_HEAD
