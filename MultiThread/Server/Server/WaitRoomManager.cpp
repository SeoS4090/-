#include "WaitRoomManager.h"
#include "ClientManager.h"
#include "Defines.h"
#include <random>
WaitRoomManager::WaitRoomManager()
{
	InitializeCriticalSection(&cs);
}


WaitRoomManager::~WaitRoomManager()
{
	DeleteCriticalSection(&cs);
}

PACKET_WAITROOMS WaitRoomManager::SendWaitRoom()
{
	PACKET_WAITROOMS packet;
	packet.header.wIndex = PACKET_INDEX_LOADWAITROOM;
	packet.iFullLen = 0;
	packet.iLastRoomNo = -1;

	EnterCriticalSection(&cs);
	for (auto iter = m_mapWaitRoom.begin(); iter != m_mapWaitRoom.end(); iter++)
	{
		if (iter->second->iPlayerCount < WAITROOM_PLAYER_MAX && !iter->second->bGameStart)
		{
			packet.iFull[packet.iFullLen] = iter->second->RoomNo;
			packet.iFullLen++;
		}
		packet.iLastRoomNo = iter->second->RoomNo;
	}
	LeaveCriticalSection(&cs);

	packet.iLastRoomNo++;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int) + sizeof(int)*packet.iFullLen + sizeof(int);

	return packet;
}	

void WaitRoomManager::EnterWaitRoom(SOCKET sock, int RoomNo)
{
	ClientManager::GetInstance()->SetWaitRoom(sock, true);
	WaitRoom * pInfo;
	EnterCriticalSection(&cs);

	if (m_mapWaitRoom[RoomNo] != NULL)
		 pInfo = m_mapWaitRoom[RoomNo];
	else
	{
		m_mapWaitRoom.erase(RoomNo);

		WaitRoom * pNew = new WaitRoom();
		pNew->RoomNo = RoomNo;
		pNew->iPlayerCount = 0;
		memcpy(pNew->PlayerID[pNew->iPlayerCount], ClientManager::GetInstance()->GetID(sock).c_str(), ClientManager::GetInstance()->GetID(sock).size());
		pNew->Player[pNew->iPlayerCount++] = sock;
		for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
		{
			pNew->iPlayer_Ready[i] = true;
		}
		pNew->iPlayer_Ready[0] = false;

		pNew->bGameStart = false;

		m_mapWaitRoom.insert(make_pair(RoomNo, pNew));

		printf("%d 번방 생성 \n", m_mapWaitRoom[RoomNo]->RoomNo);
		LeaveCriticalSection(&cs);

		return;

	}
	pInfo->iPlayerCount++;

	LeaveCriticalSection(&cs);



	for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
	{
		if (pInfo->Player[i] == 0)
		{
			string id = ClientManager::GetInstance()->GetID(sock);

			memcpy(pInfo->PlayerID[i], id.c_str(), id.size());
			pInfo->Player[i] = sock;
			pInfo->iPlayer_Ready[i] = false;

			break;
		}
	}
}

bool WaitRoomManager::PlayerReady(SOCKET sock, int RoomNo)
{

	EnterCriticalSection(&cs);
	WaitRoom * pInfo = m_mapWaitRoom[RoomNo];
	for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
	{
		if (pInfo->PlayerID[i] == ClientManager::GetInstance()->GetID(sock))
			pInfo->iPlayer_Ready[i] = true;
	}

	LeaveCriticalSection(&cs);

	if (pInfo->iPlayerCount < 2)
		return false;

	for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
		if(!pInfo->iPlayer_Ready[i])
			return false;
	
	pInfo->bGameStart = true;
	PACKET_GAMESTART packet;
	packet.header.wIndex = PACKET_INDEX_GAMESTART;
	packet.PlayerCount = pInfo->iPlayerCount;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int) + sizeof(int);
	packet.Rand_Seed = GetTickCount() % 100;

	srand(packet.Rand_Seed);

	SendPacket(RoomNo, (char *)&packet, packet.header.wLen);
	return true;
}

void WaitRoomManager::ExitRoom(SOCKET sock)
{
	EnterCriticalSection(&cs);
	SOCKETINFO * pSockInfo = ClientManager::GetInstance()->GetSocketInfo(sock);

	if (!pSockInfo->bWaitRoom)
		return;

	pSockInfo->bWaitRoom = false;

	PACKET_LOGIN packet;
	packet.header.wIndex = PACKET_INDEX_EXIT;
	
	int RoomNo = -1;

	for (auto iter = m_mapWaitRoom.begin(); iter != m_mapWaitRoom.end(); iter++)
	{
		for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
		{
			if ((*iter).second->Player[i] == sock)
			{
				memcpy(packet.ID, (*iter).second->PlayerID[i], strlen((*iter).second->PlayerID[i]));
				packet.validID = strlen((*iter).second->PlayerID[i]);
				packet.ID[packet.validID] = 0;
				packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int) + sizeof(packet.ID);

				RoomNo = (*iter).first;

				(*iter).second->Player[i] = 0;
				ZeroMemory((*iter).second->PlayerID[i], MAX_ID_SIZE);
				(*iter).second->iPlayerCount--;
				if ((*iter).second->iPlayerCount == 0)
				{
					WaitRoom * pDel = m_mapWaitRoom[RoomNo];
					SAFE_DELETE(pDel);
					m_mapWaitRoom.erase(m_mapWaitRoom.find(RoomNo));
					return;
				}

			}
		}

	}

	if (!m_mapWaitRoom[RoomNo]->bGameStart)
		SendPlayerInfo(RoomNo);
	else
	{
		SendPacket(RoomNo, (char *)&packet, packet.header.wLen);
	}
	LeaveCriticalSection(&cs);

}

void WaitRoomManager::SendPacket(int RoomNo, char * packet, int PacketSize, SOCKET Except)
{
	EnterCriticalSection(&cs);
	WaitRoom * pInfo = m_mapWaitRoom[RoomNo];
	LeaveCriticalSection(&cs);

	for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
	{
		if (pInfo->Player[i] != 0 && pInfo->Player[i] != Except)
		{
			send(pInfo->Player[i], packet, PacketSize, 0);
		}
	}
}

void WaitRoomManager::SendPlayerInfo(int RoomNo)
{
	EnterCriticalSection(&cs);
	WaitRoom * pInfo = m_mapWaitRoom[RoomNo];
	LeaveCriticalSection(&cs);


	if (pInfo == NULL)
		return;

	PACKET_PLAYERINFO packet;
	packet.header.wIndex = PACKET_INDEX_PLAYERINFO;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int) + sizeof(packet.Id);
	packet.PlayerCount = 0;

	for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
	{
		if (pInfo->Player[i] == 0)
		{
			pInfo->PlayerID[i][0] = 0;
			continue;
		}
		
		packet.PlayerCount++;
		
		memcpy(packet.Id, pInfo->PlayerID, sizeof(packet.Id));
	}

	for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
	{
		if (pInfo->Player[i] == 0)
			continue;
		send(pInfo->Player[i], (char *)&packet, packet.header.wLen, 0);
	}
}

void WaitRoomManager::SetWaitRoomGameStart(int RoomNo)
{
	EnterCriticalSection(&cs);
	WaitRoom * pInfo = m_mapWaitRoom[RoomNo];
	LeaveCriticalSection(&cs);
	pInfo->bGameStart = false;
}
