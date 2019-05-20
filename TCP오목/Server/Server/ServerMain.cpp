#include <WinSock2.h>
#include <crtdbg.h>
#include "ServerManager.h"
#include "Defines.h"

using namespace std;

#define BUFSIZE		512
#define WM_SOCKET	(WM_USER+1)
#define SERVERPORT	9000
int g_iIndex = 0;
int g_iRoomNo = 0;
SOCKET WaitPlayer = NULL;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int main(int argc, char* argv[])
{
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(154);

	int retval;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = NULL;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "TCPServer";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&WndClass))
		return -1;

	HWND hWnd = CreateWindow(WndClass.lpszClassName, "TCP 오목 서버", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		300, 100, NULL, (HMENU)NULL, NULL, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)	ServerManager::GetInstance()->err_display("listen()");

	retval = WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR) ServerManager::GetInstance()->err_display("WSAAsyncSelect()");

	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) ServerManager::GetInstance()->err_display("Bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) ServerManager::GetInstance()->err_display("listen()");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	closesocket(listen_sock);
	WSACleanup();

	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return 0;
	case WM_SOCKET:
		ServerManager::GetInstance()->ProcessSocketMessage(hWnd, uMsg, wParam, lParam);
		return 0;

	case WM_DESTROY:
		ServerManager::GetInstance()->Release();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}