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
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
//������������ ����� ����
bool RegWindowClass(HINSTANCE hInstance)
{
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
	//������������ ����� ����
	if (!RegWindowClass(hInstance))
	{
		MessageBox(NULL, L"������ �������� ����!", L"������ ����������� ������ ����", NULL);
		return 1;
	}
	//���������� ����
	HWND h_wnd;
	//������� ����
	h_wnd = CreateWindow(GAME_NAME, GAME_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,window_width, window_height, NULL, NULL, hInstance, NULL);
	//��������� ������� �� ����
	if (!h_wnd)
	{
		MessageBox(NULL, L"������ �������� ����!", L"������ ����������� ����", NULL);
		return 2;
	}
	//���������� ����
	ShowWindow(h_wnd, nCmdShow);
	//��������� ����
	UpdateWindow(h_wnd);
	//��������� ���������
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}