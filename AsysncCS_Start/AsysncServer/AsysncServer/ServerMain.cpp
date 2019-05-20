#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <map>
#include "..\..\Common\PACKET_HEADER.h"
using namespace std;

#define BUFSIZE 512
#define WM_SOCKET (WM_USER+1)

int g_iIndex = 0;

class Client
{
public:
	SOCKET sock;
	int index;
	int x;
	int y;
};
map<SOCKET, Client*> g_mapClient;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessPacket(SOCKET sock, char* szBuf, int retval);
void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
void err_display(int errcode);
void err_display(char* szMsg);

int main(int argc, char* argv[])
{
	int retval;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = NULL;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "WSAAsyncSelectServer";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&WndClass))
		return -1;

	HWND hWnd = CreateWindow("WSAAsyncSelectServer", "TCP 서버", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, NULL, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	srand(GetTickCount());

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "err on socket" << endl;
		return -1;
	}

	retval = WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on WSAAsyncSelect" << endl;
		return -1;
	}

	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		cout << "err on bind" << endl;
		return -1;
	}

	//listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on listen" << endl;
		return -1;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	closesocket(listen_sock);
	WSACleanup();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SOCKET:
		ProcessSocketMessage(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;


	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		err_display(WSAGETSELECTERROR(lParam));
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			int err_code = WSAGetLastError();
			if (err_code != WSAEWOULDBLOCK)
			{
				err_display("err on accept");
			}
			return;
		}
		printf("[TCP 서버] 클라이언트 접속 : IP 주소 = %s , 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		Client* pNew = new Client();
		pNew->index = g_iIndex++;
		pNew->sock = client_sock;
		pNew->x = rand() % 500;
		pNew->y = rand() % 500;
		g_mapClient.insert(make_pair(client_sock, pNew));

		{
			PACKET_LOGIN_RET packet;
			packet.header.wIndex = PACKET_INDEX_LOGIN_RET;
			packet.header.wLen = sizeof(packet);
			packet.wIndex = pNew->index;
			send(client_sock, (char*)&packet, packet.header.wLen, 0);
		}

		Sleep(500);

		{
			PACKET_PLAYER_INFO packet;
			packet.header.wIndex = PACKET_INDEX_PLAYER_INFO;
			packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(WORD) + g_mapClient.size() * sizeof(PLAYER);
			packet.wLen = g_mapClient.size();
			int i = 0;
			for (auto iter = g_mapClient.begin(); iter != g_mapClient.end(); iter++)
			{
				packet.info[i].wIndex = iter->second->index;
				packet.info[i].x = iter->second->x;
				packet.info[i].y = iter->second->y;
				i++;
			}

			for (auto iter = g_mapClient.begin(); iter != g_mapClient.end(); iter++)
			{
				send(iter->first, (char*)&packet, packet.header.wLen, 0);
			}
		}
		

		retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		if (retval == SOCKET_ERROR)
		{
			cout << "err on WSAAsyncSelect!!" << endl;
		}
	}
	break;
	case FD_READ:
		{
			char szBuf[BUFSIZE];

			retval = recv(wParam, szBuf, BUFSIZ , 0);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					cout << "err on recv!!" << endl;
				}
			}

			ProcessPacket(wParam, szBuf, retval);
		}
		

		break;
	case FD_CLOSE:
		{

		}
		break;
	}
}


void ProcessPacket(SOCKET sock , char* szBuf, int retval)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_PLAYER_POS:
	{
		PACKET_PLAYER_POS packet;
		memcpy(&packet, szBuf, sizeof(packet));

		for (auto iter = g_mapClient.begin(); iter != g_mapClient.end(); iter++)
		{
			if (iter->first == sock)
				continue;

			send(iter->first, (char*)&packet, sizeof(packet), 0);
		}
	}
	break;
	}
}

void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류]%s", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(char* szMsg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s]%s\n", szMsg, lpMsgBuf);
	LocalFree(lpMsgBuf);
}