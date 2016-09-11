#define LOG_ON
#define vCONTROL_DLL

#include <stdexcept>
#include <wchar.h>
#include <Opengl_buffer\OPENGL_BUFFER.h>
#include <vRECURCE\vRESOURCE.h>

#include <loger\log_error.h>
#include "vCONTROL.h"
//function init pixel format
bool  InitPixelFormat(HDC h_dc_)
{
	PIXELFORMATDESCRIPTOR pixel_setting = { 0 };
	pixel_setting.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel_setting.nVersion = 1;
	pixel_setting.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
	pixel_setting.iPixelType = PFD_TYPE_RGBA;
	pixel_setting.cColorBits = 32;
	pixel_setting.cDepthBits = 24;

	int pixel_format = ChoosePixelFormat(h_dc_, &pixel_setting);
	if (!pixel_format)
	{
		return 1;
	}
	if (!SetPixelFormat(h_dc_, pixel_format, &pixel_setting))
	{
		return 1;
	}
	return 0;
}
//rendering finction
void vCONTROL::ResizeWindow(int rect_x, int rect_y)
{
	glViewport(0, 0, rect_x, rect_y);
}
void vCONTROL::LoadGameResource()
{
	wchar_t file_path[260];
	WIN32_FIND_DATA file_data;
	HANDLE h_find;
	h_find = FindFirstFile(L"rec/*.rec",&file_data);
	if (h_find == INVALID_HANDLE_VALUE)
	{
		LogSend(LOG_CRITICAL_ERROR, "vCONTROL", "Can`t load resource");
		return;
	}
	unsigned int resource_id = 0;
	do
	{
		vRESOURCE * resource;
		if (opengl_buffer_ != 0)
		{
			resource = (vRESOURCE *) new vRESOURCE_BUFFER;
			((vRESOURCE_BUFFER *)resource)->SetOpenglBufferEx((OPENGL_BUFFER *)opengl_buffer_);
		}
		else
		{
			resource = (vRESOURCE *) new vRESOURCE_ARRAY;
		}
		wcscpy_s(file_path,260, L"rec/");
		wcscat_s(file_path,260, file_data.cFileName);
		resource_id = resource->LoadResourceFromFile(file_path, 0, 0);
		auto insert_error = resource_.insert(std::pair<unsigned int, void *>(resource_id, (void*)resource));
		if (insert_error.second == false)
		{
			LogSend(LOG_CRITICAL_ERROR, "vCONTROL", "Error resource id");
			SendMessage(h_wnd_, WM_CLOSE, NULL, NULL);
			return;
		}

	} while (FindNextFile(h_find, &file_data) != 0);

}
void vCONTROL::Rend()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//get render objects
	object_for_rend_.swap(source_objects_());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//rendering objects
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	((vRESOURCE*)resource_.at(2))->Select();
	SelectTextureCoord(id_texcoord_, opengl_buffer_);
	glTranslatef(0.0f, 0.0f, 0.5f);
	glScalef(2, 2, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glLoadIdentity();
	glTranslatef(-1.0f, -1.0f, 0.0f);
	glScalef(scale_cof_x_, scale_cof_y_, 0);
	for (auto iter = object_for_rend_.begin(); iter < object_for_rend_.end(); iter++)
	{
		glPushMatrix();
		glTranslatef(iter->x_pos_, iter->y_pos_, 0);
		((vRESOURCE*)resource_.at(iter->recurce_id_))->Select();
		SelectTextureCoord(id_texcoord_, opengl_buffer_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glPopMatrix();
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	SwapBuffers(h_dc_);
}
void vCONTROL::SetSourceObjects(std::function<std::vector<vOBJECT>()> source_objects)
{
	source_objects_ = source_objects;
}
void vCONTROL::SetScaleCof(unsigned int map_width, unsigned int map_heigh)
{
	RECT window_size;
	GetClientRect(h_wnd_, &window_size);
	scale_cof_x_ = 1.0f / float(map_width);
	scale_cof_y_ = 1.0f / float(map_heigh);
}
void vCONTROL::SetScaleCof(float cof_x, float cof_y)
{
	scale_cof_x_ = cof_x;
	scale_cof_y_ = cof_y;
}
void vCONTROL::GetScaleCof(float * cof_x, float * cof_y)
{
	*cof_x = scale_cof_x_;
	*cof_y = scale_cof_y_;
}
//set class function
vCONTROL::vCONTROL (HWND h_wnd)
{
	h_wnd_ = h_wnd;
	h_dc_ = GetDC(h_wnd_);
	InitPixelFormat(h_dc_);
	h_rc_ = wglCreateContext(h_dc_);
	if (!h_rc_)
	{
		MessageBox(NULL, L"Error init OpenGL!", L"Error create context", NULL);
		LogSend(LOG_CRITICAL_ERROR,"vCONTROL","Error create context");
		SendMessage(h_wnd_, WM_DESTROY,NULL,NULL);
	}
	if (!wglMakeCurrent(h_dc_, h_rc_))
	{
		MessageBox(NULL, L"Error init OpenGL!", L"Error meke current context", NULL);
		LogSend(LOG_CRITICAL_ERROR, "vCONTROL", "Error meke current context");
		SendMessage(h_wnd_, WM_DESTROY, NULL, NULL);
		return;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glAlphaFunc(GL_GREATER, 0.6f);  // Настройка альфа теста (запрет смешивания)
	glEnable(GL_ALPHA_TEST);
	byte error = 0;
	opengl_buffer_ = new OPENGL_BUFFER(&error);
	if (error == 0)
		LogSend(LOG_INFO, "vCONTROL", "Buffer work");
	else
	{
		LogSend(LOG_ERROR, "vCONTROL", "Buffer don`t work ");
		opengl_buffer_ = 0;
	}
	LoadGameResource();
	float tm[8] = { 0.0,0.0,0.0,1.0,1.0,1.0,1.0,0.0 };
	id_texcoord_ = InitTexCoord(tm, opengl_buffer_);
}
vCONTROL::~vCONTROL()
{
	if (h_rc_)
	{
		wglMakeCurrent(h_dc_, NULL);
		wglDeleteContext(h_rc_);
		h_rc_ = 0;
	}
}
