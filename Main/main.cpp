#define LOG_ON

#include "includs.h"
#include "names.h"
vCONTROL * _v_control;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		_v_control->Rend();
		break;
	case WM_SIZE:
		_v_control->ResizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_DESTROY:
		_v_control->~vCONTROL();
		LogSend(LOG_INFO, "main", "Закрытие приложения");
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
//reg window class
bool RegWindowClass(HINSTANCE hInstance)
{
	LogSend(LOG_INFO, "main", "Регестрация класса окна");
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
	return RegisterClassEx(&window);
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
	bool error = 0;
	_v_control = new vCONTROL(h_wnd);
	//show window
	ShowWindow(h_wnd, nCmdShow);
	//update window
	UpdateWindow(h_wnd);
	//get message
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}