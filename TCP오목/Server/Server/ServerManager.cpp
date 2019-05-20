#include "ServerManager.h"
#include "ClientManager.h"
#include "WaitRoomManager.h"
ServerManager * ServerManager::pThis = NULL;

void ServerManager::ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int retval = 0;
	int addrlen = 0;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;

	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGetLastError();
		err_display(err_code);
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
				err_display("err on Accept");
			return;
		}
		ClientManager::GetInstance()->AddClient(client_sock, clientaddr);
		retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		if (retval == SOCKET_ERROR) err_display("WSAAsyncSelect()");

		WaitRoomManager::GetInstance()->SendWaitRoom(client_sock);
	}
	break;
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

		if (!ProcessPacket(wParam, szBuf, retval))
		{
			Sleep(100);
			SendMessage(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
	break;
	case FD_CLOSE:
		PACKET_HEADER packet;
		packet.wIndex = PACKET_INDEX_RETURN;
		packet.wLen = sizeof(PACKET_HEADER);
		send(ClientManager::GetInstance()->GetClient(wParam)->otherPlayersock, (char *)&packet, sizeof(packet), 0);

		WaitRoomManager::GetInstance()->ReturnWaitRoom(wParam);
		ClientManager::GetInstance()->ExitClient(wParam);

		break;
	}
}

bool ServerManager::ProcessPacket(SOCKET sock, char szBuf[BUFSIZE], int retval)
{
	Client * pClient = ClientManager::GetInstance()->GetClient(sock);
	if (retval > 0)
	{
		memcpy(&pClient->buf[pClient->len], szBuf, retval);
		pClient->len += retval;
		retval = 0;
	}

	if (pClient->len < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, &pClient->buf, sizeof(header));

	if (pClient->len < header.wLen)
		return false;

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN:
	{
		WaitRoomManager::GetInstance()->SendWaitRoom(sock);

	}
	break;

	case PACKET_INDEX_TEXT:
	{
		PACKET_TEXT packet;
		memcpy(&packet, szBuf, sizeof(packet));
		send(pClient->otherPlayersock, (char*)&packet, sizeof(packet), 0);
	}
	break;

	case PACKET_INDEX_WAITROOM:
	{
		PACKET_WAITROOM packet;
		memcpy(&packet, szBuf, sizeof(packet));

		SOCKET OtherPlayerSock = WaitRoomManager::GetInstance()->EnterWaitRoom(sock, packet.iRoomNo);
		ClientManager::GetInstance()->SetPair(sock, OtherPlayerSock);
	}

	break;


	case PACKET_INDEX_GAMEREADY:
	{
		send(pClient->otherPlayersock, (char *)&header, sizeof(header), 0);
	}
	break;
	case PACKET_INDEX_GAMESTART:
	{
		send(pClient->otherPlayersock, (char*)&header, sizeof(header), 0);
	}
	break;
	case PACKET_INDEX_PLAYER:
	{
		PACKET_STONE packet;
		memcpy(&packet, szBuf, sizeof(packet));
		send(pClient->otherPlayersock, (char*)&packet, sizeof(packet), 0);
	}
	break;

	case PACKET_INDEX_RETURN:
	{

		send(pClient->otherPlayersock, (char *)&header, sizeof(header), 0);

		WaitRoomManager::GetInstance()->ReturnWaitRoom(sock);
		ClientManager::GetInstance()->ReleasePair(sock);

	}
	break;
	}


	if (pClient->len - header.wLen >= 0)
		memcpy(&pClient->buf, &pClient->buf[pClient->len], pClient->len - header.wLen);
	pClient->len -= header.wLen;

	return true;
}

void ClientManager::Release()
{
	for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapClient.clear();
	SAFE_DELETE(pThis);
}

ServerManager::ServerManager()
{
}


ServerManager::~ServerManager()
{
}
void ServerManager::err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[¿À·ù]%s", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void ServerManager::err_display(char* szMsg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s]%s\n", szMsg, lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void ServerManager::Release()
{
	ClientManager::GetInstance()->Release();
	WaitRoomManager::GetInstance()->Release();
	SAFE_DELETE(pThis);
}
