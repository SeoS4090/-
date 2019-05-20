#pragma once
#include <map>
#include "Defines.h"
#include "SingleTone.h"

struct WaitRoom
{
	int			RoomNo;
	SOCKET		Player[8];
	int			iPlayerCount;
	bool		iPlayer_Ready[WAITROOM_PLAYER_MAX];
	char		PlayerID[WAITROOM_PLAYER_MAX][8];
	bool		bGameStart;
};

class WaitRoomManager : public SingleTone<WaitRoomManager>
{
private:
	CRITICAL_SECTION cs;	
	map<int ,WaitRoom * >	m_mapWaitRoom;
public:
	PACKET_WAITROOMS	SendWaitRoom();

	void				EnterWaitRoom(SOCKET sock, int RoomNo);
	bool				PlayerReady(SOCKET sock, int RoomNO);
	void				ExitRoom(SOCKET sock);
	void				SendPacket(int RoomNo, char * packet, int PacketSize, SOCKET Except = 0);
	//void				SendPacket(int RoomNo, char * packet, int PacketSize);
	void				SendPlayerInfo(int RoomNo);
	void				SetWaitRoomGameStart(int RoomNo);

	WaitRoomManager();
	~WaitRoomManager();
};

