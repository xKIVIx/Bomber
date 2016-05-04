#define LOG_ON
#define MENU_GET_DLL
#include "vMENUS.h"


//loding screen
void LoadScreen()
{
	glBegin(GL_POLYGON);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(1.0f, 0.9f);
	glVertex2f(0.8f, 0.9f);
	glVertex2f(0.8f, 1.0f);
	glEnd();
}
void MainMenu()
{
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glEnd();
}

bool MENU_GET_API MENU_API::GetMenu(int type)
{
	switch (type)
	{
	case STATE_LOADING:
		LoadScreen();
		return 0;
		break;
	case STATE_MAIN_MENU:
		MainMenu();
		return 0;
		break;
	default:
		return 1;
		break;
	}
}