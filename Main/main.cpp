#define LOG_ON

#include "includs.h"
#include "names.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");


	switch (message)
	{
	case WM_PAINT:
		break;
	case WM_DESTROY:
		LogSend(LOG_INFO, "main", "Закрытие приложения");
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
//регестрируем класс окна
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
	//регестрируем класс окна
	if (!RegWindowClass(hInstance))
	{
		MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка регестрации класса окна", NULL);
		LogSend(LOG_CRITICAL_ERROR, "main", "Ошибка регестрации класса окна");
		return 1;
	}
	//дискриптор окна
	HWND h_wnd;
	//создаем окно
	h_wnd = CreateWindow(GAME_NAME, GAME_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,window_width, window_height, NULL, NULL, hInstance, NULL);
	//проверяем создано ли окно
	if (!h_wnd)
	{
		MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка дискриптора окна", NULL);
		LogSend(LOG_CRITICAL_ERROR, "main", "Ошибка создания окна");
		return 2;
	}
	bool error = 0;
	vCONTROL v_control(h_wnd, &error);
	//отображаем окно
	ShowWindow(h_wnd, nCmdShow);
	//обновляем окно
	UpdateWindow(h_wnd);
	v_control.StartRendering();
	//получение сообщений
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}