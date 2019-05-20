#include<Windows.h>
#include "ToolManager.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Tank Map Tool");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	//WndClass�� �⺻ ������ȯ���� ����� ����ü��. �ɹ������� �ؿ��� ����.
	WndClass.cbClsExtra = 0; //���࿵��
	WndClass.cbWndExtra = 0; //���࿵�� (�Ű�x)
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//Ŀ��
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//������ ���
	WndClass.hInstance = hInstance; //(���α׷� �ڵ鰪(��ȣ)���)
	WndClass.lpfnWndProc = WndProc;	//���μ��� �Լ� ȣ��
	WndClass.lpszClassName = lpszClass;	//Ŭ���� �̸�
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);  //������� WidClass�� ���

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, MAP_WIDTH*2 + 500, MAP_HEIGHT*2 + 60,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hEdit;
	RECT rt = { 0,0,MAP_WIDTH * 2 + 20,MAP_HEIGHT * 2 + 20 };
	switch (iMessage)
	{
	case WM_CREATE:
		
		hdc = GetDC(hWnd);
		ToolManager::GetInstance()->Init(hWnd, hdc, g_hInst);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_MOUSEMOVE:
		ToolManager::GetInstance()->Update(lParam);
		InvalidateRect(hWnd, &rt, FALSE);
		return 0;
	case WM_LBUTTONDOWN:
		ToolManager::GetInstance()->LBDown(lParam);
		return 0;
	case WM_RBUTTONDOWN:
		ToolManager::GetInstance()->RBDown(lParam);
		InvalidateRect(hWnd, &rt, FALSE);
		return 0;
	case WM_COMMAND:
		ToolManager::GetInstance()->Command(hWnd, wParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		ToolManager::GetInstance()->Draw(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}