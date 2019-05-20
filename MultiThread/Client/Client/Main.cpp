#pragma comment (lib, "ws2_32")
#include <WinSock2.h>
#include <windows.h>
#include "GameManager.h"
#include <crtdbg.h>

using namespace std;


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
char g_szClassName[256] = "Crazy Acade Client";
SOCKET g_Sock;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(161);
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	g_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_Sock == INVALID_SOCKET) return 1;


	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	int retval = connect(g_Sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) return 1;


	retval = WSAAsyncSelect(g_Sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR) return 1;

	GameManager::GetInstance()->Init(hWnd, 800, 600);

	while (true)
	{
		/// 메시지큐에 메시지가 있으면 메시지 처리
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			if (GameManager::GetInstance()->Update())
			{
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}
		}
	}
	GameManager::GetInstance()->Release();
	GameManager::GetInstance()->DestroyInstance();
	closesocket(g_Sock);
	WSACleanup();

	return (int)Message.wParam;
}
HWND g_Button;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CHAR:
		GameManager::GetInstance()->Typing(wParam);
		return 0;

	case WM_SOCKET:
		GameManager::GetInstance()->ProcessSocketMessage(hWnd, iMessage, wParam, lParam, g_Sock);
		return 0;

	case WM_DESTROY:
		
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}