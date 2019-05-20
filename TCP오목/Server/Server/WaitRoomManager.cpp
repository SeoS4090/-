#include "WaitRoomManager.h"
#include "../../Common/PACKET_HEADER.h"

WaitRoomManager * WaitRoomManager::pThis = NULL;



SOCKET WaitRoomManager::EnterWaitRoom(SOCKET sock, int RoomNo)
{
	for (auto iter = m_ListWaitRoom.begin(); iter != m_ListWaitRoom.end(); iter++)
	{
		if ((*iter)->RoomNo == RoomNo)
		{
			(*iter)->Player[1] = sock;
			return (*iter)->Player[0];
		}
	}

	WaitRoom * pNew = new WaitRoom();
	pNew->RoomNo = m_iLastRoomNo++;
	pNew->Player[0] = sock;
	m_ListWaitRoom.push_back(pNew);
	printf("%d 번방 생성 \n", pNew->RoomNo);
	return sock;
}

bool WaitRoomManager::ReturnWaitRoom(SOCKET sock)
{
	for (auto iter = m_ListWaitRoom.begin(); iter != m_ListWaitRoom.end(); iter++)
	{
		if ((*iter)->Player[0] == sock || (*iter)->Player[1] == sock)
		{
			if ((*iter)->Player[0] == sock)
				(*iter)->Player[0] = (*iter)->Player[1];
			(*iter)->Player[1] = 0;

			if ((*iter)->Player[0] == 0)
			{
				WaitRoom * pdel = *iter;
				m_ListWaitRoom.erase((iter));
				delete pdel;
				break;
			}
		}
	}
	return false;
}
void WaitRoomManager::SendWaitRoom(SOCKET sock)
{
	PACKET_WAITROOMS packet;
	packet.header.wIndex = PACKET_INDEX_LOADWAITROOM;
	packet.iFullLen = 0;

	for (auto iter = m_ListWaitRoom.begin(); iter != m_ListWaitRoom.end(); iter++)
	{
		if ((*iter)->Player[1] == sock || (*iter)->Player[0] == sock)
			return;

		if ((*iter)->Player[1] == 0)
		{
			packet.iFull[packet.iFullLen] = (*iter)->RoomNo;
			packet.iFullLen++;
		}
	}
	
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int) + sizeof(int)*packet.iFullLen;
	
	send(sock, (char *)&packet, packet.header.wLen, 0);
}
PACKET_WAITROOMS WaitRoomManager::SendWaitRoom()
{
	PACKET_WAITROOMS packet;
	packet.header.wIndex = PACKET_INDEX_LOADWAITROOM;
	packet.iFullLen = 0;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int) + sizeof(int)*packet.iFullLen;

	for (auto iter = m_ListWaitRoom.begin(); iter != m_ListWaitRoom.end(); iter++)
	{
		if ((*iter)->Player[1] != 0)
		{
			packet.iFull[packet.iFullLen] = (*iter)->RoomNo;
			packet.iFullLen++;
		}
	}
	return packet;
}

void WaitRoomManager::Release()
{
	for (auto iter = m_ListWaitRoom.begin(); iter != m_ListWaitRoom.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_ListWaitRoom.clear();
	SAFE_DELETE(pThis);
}


WaitRoomManager::WaitRoomManager()
{
	m_iLastRoomNo = 0;
}


WaitRoomManager::~WaitRoomManager()
{
}
