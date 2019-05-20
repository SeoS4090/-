#include <WinSock2.h>
#include <windows.h>
#include "../../Common/PACKET_HEADER.h"
#include "GameManager.h"
#include "UIManager.h"
using namespace std;

#define WM_SOCKET	(WM_USER+1)
#define BUFSIZE		512
#define SERVERPORT	9000
#define SERVERIP	"127.0.0.1"

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
char g_szClassName[256] = "Client";
SOCKET g_Sock;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(150);

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
		800, 800, NULL, (HMENU)NULL, hInstance, NULL);
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

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

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
	case WM_CREATE:
		GameManager::GetInstance()->Init(hWnd);
		return 0;

	case WM_CHAR:
		GameManager::GetInstance()->Message(wParam);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_LBUTTONUP:
		{
			GameManager::GetInstance()->AddStone(LOWORD(lParam), HIWORD(lParam));
			POINT pt = { LOWORD(lParam), HIWORD(lParam) };
			UIManager::GetInstance()->Update(pt);

			if (GameManager::GetInstance()->GetState() == WIN)
			{
				if (MessageBox(hWnd, "Win", "YOU", MB_OK))
				{
					GameManager::GetInstance()->GameRestart();
				}
			}

			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;
	case WM_SOCKET:
		GameManager::GetInstance()->ProcessSocketMessage(hWnd, iMessage, wParam,lParam, g_Sock);
		if (GameManager::GetInstance()->GetState() == LOSE)
		{
			if (MessageBox(hWnd, "LOSE", "YOU", MB_OK))
			{
				GameManager::GetInstance()->GameRestart();
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;	

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->Draw(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		GameManager::GetInstance()->Release();
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
