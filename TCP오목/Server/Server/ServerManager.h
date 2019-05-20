#pragma once
#include "Defines.h"
#include "../../Common/PACKET_HEADER.h"
#include <Windows.h>

class ServerManager
{
private:
	static ServerManager * pThis;
public:
	static ServerManager * GetInstance()
	{
		if (pThis == NULL)
			pThis = new ServerManager();
		return pThis;
	}
	void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ProcessPacket(SOCKET sock, char bufsize[BUFSIZE], int retval);
	ServerManager();
	~ServerManager();
	void err_display(int errcode);
	void err_display(char * szMsg);
	void Release();
};

