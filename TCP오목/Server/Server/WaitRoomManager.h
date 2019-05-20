#pragma once
#include "Defines.h"
#include "../../Common/PACKET_HEADER.h"
#include <list>



class WaitRoomManager
{
private:
	struct WaitRoom
	{
		int		RoomNo;
		SOCKET	Player[2];
	};

	static WaitRoomManager * pThis;
	list<WaitRoom *> m_ListWaitRoom;
	int m_iLastRoomNo;
public:
	static WaitRoomManager * GetInstance()
	{
		if (pThis == NULL)
		{
			pThis = new WaitRoomManager();
		}
		return pThis;
	}
	void SendWaitRoom(SOCKET sock);
	SOCKET EnterWaitRoom(SOCKET sock, int RoomNo);
	bool ReturnWaitRoom(SOCKET sock);
	PACKET_WAITROOMS SendWaitRoom();
	void Release();
	WaitRoomManager();
	~WaitRoomManager();
};

