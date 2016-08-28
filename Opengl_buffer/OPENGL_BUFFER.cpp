#define OPENGL_BUFFER_DLL

#include "OPENGL_BUFFER.h"

inline OPENGL_BUFFER::OPENGL_BUFFER(byte * error_)
{
	const char * extensions = (const char *)glGetString(GL_EXTENSIONS);
	if (!extensions)
	{
		*error_ = 1;
	}
	else
	{
		if (strstr(extensions, "GL_ARB_vertex_buffer_object") == NULL)
		{
			*error_ = 2;
		}
		else
		{
			gen_buffer_ = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
			bind_buffer_ = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
			buffer_data_ = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
			is_buffer_ = (PFNGLISBUFFERARBPROC)wglGetProcAddress("glIsBufferARB");
			delete_buffers_ = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
			get_buffer_sub_data_ = (PFNGLGETBUFFERSUBDATAARBPROC)wglGetProcAddress("glGetBufferSubDataARB");
			*error_ = 0;
		}
	}
}

inline OPENGL_BUFFER::~OPENGL_BUFFER()
{
}

inline GLuint * OPENGL_BUFFER::GenBeffers(GLsizei n)
{
	GLuint * out = new GLuint[n];
	gen_buffer_(n, out);
	return out;
}

inline bool OPENGL_BUFFER::BindBuffer(GLenum type, GLuint buffer_id)
{
	bind_buffer_(type, buffer_id);
	if (is_buffer_(buffer_id) == GL_TRUE)
	{

		return 1;
	}
	else
		return 0;
}

inline void OPENGL_BUFFER::DeleteBuffers(GLsizei n, GLuint * buffers)
{
	delete_buffers_(n, buffers);
}

inline void OPENGL_BUFFER::AddBufferData(GLenum type, GLsizeiptrARB size, const void * data, GLenum use_type)
{
	buffer_data_(type, size, data, use_type);
}

inline void OPENGL_BUFFER::GetData(GLenum type, GLintptrARB offset, GLsizeiptrARB size, void * data)
{
	get_buffer_sub_data_(type, offset, size, data);
}
