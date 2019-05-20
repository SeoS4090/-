#include "ClientManager.h"
#include "../../Common/PACKET_HEADER.h"
#include "WaitRoomManager.h"

ClientManager* ClientManager::pThis = NULL;


bool ClientManager::AddClient(SOCKET sock, SOCKADDR_IN clientaddr)
{
	Client* pNew = new Client();
	pNew->sock = sock;
	pNew->index = g_iIndex++;
	pNew->len = 0;
	m_mapClient.insert(make_pair(pNew->sock, pNew));
	printf("[TCP 서버] 클라이언트 접속 : IP 주소 = %s , 포트번호 = %d,  소켓번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), pNew->sock);

	return true;
}

bool ClientManager::SetPair(SOCKET sock1, SOCKET sock2)
{
	if (sock2 != sock1)
	{
		m_mapClient[sock2]->otherPlayersock = sock1;
		m_mapClient[sock1]->otherPlayersock = sock2;

		PACKET_HEADER packet;

		packet.wIndex = PACKET_INDEX_WAITROOM;
		packet.wLen = sizeof(PACKET_HEADER);

		send(sock2, (char *)&packet, sizeof(packet), 0);
		send(sock1, (char *)&packet, sizeof(packet), 0);
	}

	for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
	{
		WaitRoomManager::GetInstance()->SendWaitRoom(iter->first);
	}

	return false;
}

bool ClientManager::ReleasePair(SOCKET sock)
{
	if (m_mapClient[sock]->otherPlayersock != 0)
		m_mapClient[m_mapClient[sock]->otherPlayersock]->otherPlayersock = 0;
	m_mapClient[sock]->otherPlayersock = 0;

	for (auto iter = m_mapClient.begin(); iter != m_mapClient.end(); iter++)
	{
		WaitRoomManager::GetInstance()->SendWaitRoom(iter->first);
	}
	return true;
}


Client * ClientManager::GetClient(SOCKET sock)
{
	return m_mapClient[sock];
}

bool ClientManager::ExitClient(SOCKET sock)
{
	printf("%d 클라이언트 종료\n", m_mapClient[sock]->sock);
	if (m_mapClient[sock]->otherPlayersock != 0)
		m_mapClient[m_mapClient[sock]->otherPlayersock]->otherPlayersock = 0;

	delete m_mapClient[sock];
	m_mapClient.erase(sock);


	return true;
}

ClientManager::ClientManager()
{
}


ClientManager::~ClientManager()
{
}
