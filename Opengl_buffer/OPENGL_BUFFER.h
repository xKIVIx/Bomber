// this dll for opengl buffer ex


#ifndef OPENGL_BUFFER_HEADER
#define OPENGL_BUFFER_HEADER


#ifdef OPENGL_BUFFER_DLL
#define OPENGL_BUFFER_API _declspec(dllexport)
#else
#define OPENGL_BUFFER_API _declspec(dllimport)
#endif

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "glext.h"

class OPENGL_BUFFER_API OPENGL_BUFFER
{
public:
	OPENGL_BUFFER(byte * error_);
	~OPENGL_BUFFER();
	GLuint * GenBeffers(GLsizei n);
	bool BindBuffer(GLenum type, GLuint buffer_id);
	void DeleteBuffers(GLsizei n, GLuint * buffers);
	void AddBufferData(GLenum type, GLsizeiptrARB size, const void * data, GLenum use_type);
	void GetData(GLenum type, GLintptrARB offset, GLsizeiptrARB size, void * data);
private:
	PFNGLGENBUFFERSARBPROC gen_buffer_ = NULL;
	PFNGLBINDBUFFERARBPROC bind_buffer_ = NULL;
	PFNGLBUFFERDATAARBPROC buffer_data_ = NULL;
	PFNGLISBUFFERARBPROC is_buffer_ = NULL;
	PFNGLDELETEBUFFERSARBPROC delete_buffers_ = NULL;
	PFNGLGETBUFFERSUBDATAARBPROC get_buffer_sub_data_ = NULL;
};
#endif

