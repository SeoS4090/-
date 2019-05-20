#include "ClientManager.h"
#include "Defines.h"

void ClientManager::addClient(SOCKET sock, SOCKADDR_IN clientaddr)
{
	SOCKETINFO* pNew = new SOCKETINFO();
	ZeroMemory(&pNew->overlapped, sizeof(pNew->overlapped));
	pNew->sock = sock;
	pNew->wsabuf.buf = pNew->buf;
	pNew->wsabuf.len = BUFSIZE;
	pNew->bWaitRoom = false;
	pNew->ID = "";
	
	EnterCriticalSection(&cs);
	m_mapClient.insert(make_pair(pNew->sock, pNew));
	LeaveCriticalSection(&cs);

	PACKET_HEADER packet;
	packet.wIndex = PACKET_INDEX_LOGIN;
	packet.wLen = sizeof(PACKET_HEADER);
	send(sock, (char *)&packet, sizeof(packet), 0);
}


void ClientManager::ExitClient(SOCKET sock)
{
	
	EnterCriticalSection(&cs);

	delete(m_mapClient[sock]);
	m_mapClient.erase(sock);

	LeaveCriticalSection(&cs);
}

string ClientManager::GetID(SOCKET sock)
{
	EnterCriticalSection(&cs);
	SOCKETINFO * pInfo = m_mapClient[sock];
	LeaveCriticalSection(&cs);

	return pInfo->ID;
}

bool ClientManager::SetID(SOCKET sock, string ID)
{
	EnterCriticalSection(&cs);
	SOCKETINFO * pInfo = m_mapClient[sock];
	
	for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
	{
		if (iter->second->ID == ID)
		{
			LeaveCriticalSection(&cs);
			return false;
		}
	}
	LeaveCriticalSection(&cs);

	pInfo->ID = ID;
	return true;
}
SOCKETINFO* ClientManager::GetSocketInfo(SOCKET sock)
{
	EnterCriticalSection(&cs);
	SOCKETINFO * pInfo = m_mapClient[sock];
	LeaveCriticalSection(&cs);

	return pInfo;
}
void ClientManager::SetWaitRoom(SOCKET sock, bool bWaitRoom)
{
	EnterCriticalSection(&cs);
	SOCKETINFO * pInfo = m_mapClient[sock];
	LeaveCriticalSection(&cs);

	pInfo->bWaitRoom = bWaitRoom;
}
void ClientManager::SendPacket(PACKET_WAITROOMS packet)
{
	EnterCriticalSection(&cs);
	for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
	{
		if(!(*iter).second->bWaitRoom)
			send((*iter).first, (char *)&packet, packet.header.wLen, 0);
	}
	LeaveCriticalSection(&cs);

}

ClientManager::ClientManager()
{
	InitializeCriticalSection(&cs);
}


ClientManager::~ClientManager()
{
	DeleteCriticalSection(&cs);
}
