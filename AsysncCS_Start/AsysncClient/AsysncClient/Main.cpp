#include <WinSock2.h>
#include <windows.h>
#include <map>
#include "..\..\Common\PACKET_HEADER.h"
using namespace std;

#define WM_SOCKET (WM_USER+1)
#define BUFSIZE 512

void ProcessPacket(char* szBuf, int retval);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void ProcessSocketMessage(HWND hWnd, int iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
char g_szClassName[256] = "Hello World!!";
SOCKET g_Sock;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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
	if (g_Sock == INVALID_SOCKET)
	{
		//cout << "err on socket" << endl;
		return -1;
	}


	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int retval = connect(g_Sock, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		//cout << "err on bind" << endl;
		return -1;
	}


	retval = WSAAsyncSelect(g_Sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		//cout << "err on WSAAsyncSelect" << endl;
		return -1;
	}


	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	closesocket(g_Sock);
	WSACleanup();

	return (int)Message.wParam;
}

class Player
{
public:
	int x;
	int y;
};

map<int , Player*> g_mapPlayer;
int g_iIndex = 0;

void SendPos()
{
	PACKET_PLAYER_POS packet;
	packet.header.wIndex = PACKET_INDEX_PLAYER_POS;
	packet.header.wLen = sizeof(packet);
	packet.info.wIndex = g_iIndex;
	packet.info.x = g_mapPlayer[g_iIndex]->x;
	packet.info.y = g_mapPlayer[g_iIndex]->y;
	send(g_Sock, (char*)&packet, sizeof(packet), 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		{
		/*
			srand(GetTickCount());
			Player* pNew = new Player();
			pNew->x = rand() % 800;
			pNew->y = rand() % 600;
			g_mapPlayer.insert(make_pair(0, pNew));

			pNew = new Player();
			pNew->x = rand() % 800;
			pNew->y = rand() % 600;
			g_mapPlayer.insert(make_pair(1, pNew));
			*/
		}
		
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			g_mapPlayer[g_iIndex]->x -= 8;
			SendPos();
			break;
		case VK_RIGHT:
			g_mapPlayer[g_iIndex]->x += 8;
			SendPos();
			break;
		case VK_UP:
			g_mapPlayer[g_iIndex]->y -= 8;
			SendPos();
			break;
		case VK_DOWN:
			g_mapPlayer[g_iIndex]->y += 8;
			SendPos();
			break;
		}

		InvalidateRect(hWnd, NULL, true);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for(auto iter = g_mapPlayer.begin() ; iter != g_mapPlayer.end() ; iter++)
		{
			char szPrint[128];
			wsprintf(szPrint, "%d", iter->first);
			TextOut(hdc, iter->second->x, iter->second->y, szPrint, strlen(szPrint));

		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SOCKET:
		ProcessSocketMessage(hWnd, iMessage, wParam, lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		for (auto iter = g_mapPlayer.begin(); iter != g_mapPlayer.end(); iter++)
		{
			delete iter->second;
		}
		g_mapPlayer.clear();
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void ProcessSocketMessage(HWND hWnd,int iMessage,WPARAM wParam,LPARAM lParam)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;


	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		//err_display(WSAGETSELECTERROR(lParam));
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{

	case FD_READ:
	{
		char szBuf[BUFSIZE];

		retval = recv(wParam, szBuf, BUFSIZ, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//cout << "err on recv!!" << endl;
			}
		}

		ProcessPacket(szBuf , retval);
	}


	break;
	case FD_CLOSE:
	{

	}
	break;
	}
}

void ProcessPacket(char* szBuf,int retval)
{
	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN_RET:
		{
			PACKET_LOGIN_RET packet;
			memcpy(&packet, szBuf, sizeof(packet));
			g_iIndex = packet.wIndex;
		}
		break;
	case PACKET_INDEX_PLAYER_INFO:
		{
			PACKET_PLAYER_INFO packet;
			memcpy(&packet, szBuf, header.wLen);
			for (auto iter = g_mapPlayer.begin(); iter != g_mapPlayer.end(); iter++)
			{
				delete iter->second;
			}
			g_mapPlayer.clear();

			for (int i = 0; i < packet.wLen; i++)
			{
				Player* pNew = new Player();
				pNew->x = packet.info[i].x;
				pNew->y = packet.info[i].y;
				g_mapPlayer.insert(make_pair(packet.info[i].wIndex, pNew));
			}
		}
		break;
	case PACKET_INDEX_PLAYER_POS:
		{
			PACKET_PLAYER_POS packet;
			memcpy(&packet, szBuf, header.wLen);

			g_mapPlayer[packet.info.wIndex]->x = packet.info.x;
			g_mapPlayer[packet.info.wIndex]->y = packet.info.y;
		}
		break;
	}
}