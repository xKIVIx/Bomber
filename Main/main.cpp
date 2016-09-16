#define LOG_ON
#include <Windows.h>
#include <vCONTROL\vCONTROL.h>
#include <gCONTROL\gCONTROL.h>
#include <loger\log_error.h>
#include "names.h"
#include <thread>
vCONTROL * v_control;
gCONTROL * g_control;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		v_control->Rend();
		break;
	}
	case WM_SIZE:
	{
		v_control->ResizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case KEY_A:
		{
			g_control->Command('a');
			break;
		}
		case KEY_W:
		{
			g_control->Command('w');
			break;
		}
		case KEY_S:
		{
			g_control->Command('s');
			break;
		}
		case KEY_D:
		{
			g_control->Command('d');
			break;
		}
		case 0x1b:
		{
			LogSend(LOG_INFO, "main", "Close program");
			PostQuitMessage(0);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_DESTROY:
	{
		LogSend(LOG_INFO, "main", "Close program");
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
//reg window class
bool RegWindowClass(HINSTANCE hInstance)
{
	LogSend(LOG_INFO, "main", "Reg window class");
	WNDCLASSEX window;
	window.cbSize = sizeof(WNDCLASSEX);
	window.style = CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc = WndProc;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hInstance = hInstance;
	window.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window.lpszMenuName = NULL;
	window.lpszClassName = GAME_NAME;
	window.hIconSm = LoadIcon(window.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	return bool(RegisterClassEx(&window));
}
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	if (!RegWindowClass(hInstance))
	{
		MessageBox(NULL, L"Error create window!", L"Error registr window`s class", NULL);
		LogSend(LOG_CRITICAL_ERROR, "main", "Error registr window`s class");
		return 1;
	}
	//window`s description
	HWND h_wnd;
	//create window
	h_wnd = CreateWindow(GAME_NAME, GAME_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,window_width, window_height, NULL, NULL, hInstance, NULL);
	//chek create
	if (!h_wnd)
	{
		MessageBox(NULL, L"Error create window!", L"Error window`s desctription", NULL);
		LogSend(LOG_CRITICAL_ERROR, "main", "Error window`s desctription");
		return 2;
	}
	v_control = new vCONTROL (h_wnd);
	g_control = new gCONTROL (h_wnd,10,10);
	v_control->SetScaleCof(unsigned int(10), unsigned int(10));
	v_control->SetSourceObjects([]() {
		return g_control->GetObjectsForRend();
	});
	//show window
	SetWindowLong(h_wnd, GWL_STYLE, WS_POPUP);
	SetWindowLong(h_wnd, GWL_EXSTYLE, WS_EX_TOPMOST);
	ShowWindow(h_wnd, SW_NORMAL);
	//update window
	UpdateWindow(h_wnd);
	//get message
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	delete v_control;
	return (int)msg.wParam;
}