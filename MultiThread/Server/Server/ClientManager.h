#pragma once
#include <map>
#include "Defines.h"
#include "SingleTone.h"

struct SOCKETINFO
{
	OVERLAPPED	overlapped;
	SOCKET		sock;
	char		buf[BUFSIZE + 1];
	int			recvbytes;
	WSABUF		wsabuf;
	string		ID;
	bool		bWaitRoom;
};

class ClientManager : public SingleTone<ClientManager>
{
private:
	CRITICAL_SECTION cs;
	map<SOCKET, SOCKETINFO *> m_mapClient;
public:
	
	void		addClient(SOCKET sock, SOCKADDR_IN clientaddr);
	void		ExitClient(SOCKET sock);
	string		GetID(SOCKET sock);
	bool		SetID(SOCKET sock, string ID);

	SOCKETINFO * GetSocketInfo(SOCKET Sock);
	void		SetWaitRoom(SOCKET sock, bool bWaitRoom);


	void		SendPacket(PACKET_WAITROOMS packet);
				ClientManager();
				~ClientManager();
};

