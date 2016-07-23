
#ifndef OPENGL_BUFFER_HEADER
#define OPENGL_BUFFER_HEADER
class OPENGL_BUFFER
{
public:
	OPENGL_BUFFER(byte * error);
	~OPENGL_BUFFER();
	GLuint * GenBeffers(GLsizei n);
	bool BindBuffer(GLenum type, GLuint buffer_id);
	void DeleteBuffers(GLsizei n, GLuint * buffers);
	void AddBufferData(GLenum type, GLsizeiptrARB size, const void * data, GLenum use_type);
	void GetData(GLenum type, GLintptrARB offset, GLsizeiptrARB size, void * data);
private:
	PFNGLGENBUFFERSARBPROC gen_buffer = NULL;
	PFNGLBINDBUFFERARBPROC bind_buffer = NULL;
	PFNGLBUFFERDATAARBPROC buffer_data = NULL;
	PFNGLISBUFFERARBPROC is_buffer = NULL;
	PFNGLDELETEBUFFERSARBPROC delete_buffers = NULL;
	PFNGLGETBUFFERSUBDATAARBPROC get_buffer_sub_data = NULL;
};
#endif

