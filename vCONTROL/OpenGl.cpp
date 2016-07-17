#include <Main\includs.h>
#include "glext.h"
#include "OpenGl.h"

 OPENGL_BUFFER::OPENGL_BUFFER(byte * er)
{
	const char * extensions = (const char *)glGetString(GL_EXTENSIONS);
	if (!extensions)
	{
		::MessageBox(NULL, L"OpenGL error", L"Error", NULL);
		*er = 1;
	}
	else
	{
		if (strstr(extensions, "GL_ARB_vertex_buffer_object") != NULL)
			*er = 0;
		else
		{
			*er = 2;
			return;
		}
		gen_buffer = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		bind_buffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		buffer_data = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
		is_buffer = (PFNGLISBUFFERARBPROC)wglGetProcAddress("glIsBufferARB");
		delete_buffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
		get_buffer_sub_data = (PFNGLGETBUFFERSUBDATAARBPROC)wglGetProcAddress("glGetBufferSubDataARB");
	}
}

 OPENGL_BUFFER::~OPENGL_BUFFER()
 {
 }

 GLuint * OPENGL_BUFFER::GenBeffers(GLsizei n)
{
	GLuint * out = new GLuint[n];
	gen_buffer(n, out);
	return out;
}

 bool OPENGL_BUFFER::BindBuffer(GLenum type, GLuint buffer_id)
{
	bind_buffer(type, buffer_id);
	if (is_buffer(buffer_id) == GL_TRUE)
	{

		return 1;
	}
	else
		return 0;
}

 void OPENGL_BUFFER::DeleteBuffers(GLsizei n, GLuint * buffers)
{
	delete_buffers(n, buffers);
}

 void OPENGL_BUFFER::AddBufferData(GLenum type, GLsizeiptrARB size, const void * data, GLenum use_type)
{
	buffer_data(type, size, data, use_type);
}

 void OPENGL_BUFFER::GetData(GLenum type, GLintptrARB offset, GLsizeiptrARB size, void * data)
{
	get_buffer_sub_data(type, offset, size, data);
}
