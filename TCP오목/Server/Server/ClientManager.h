#pragma once
#include "Defines.h"
#include <map>


class Client
{
public:
	SOCKET sock;
	SOCKET otherPlayersock;
	int index;
	char buf[BUFSIZE];
	int len;
};


class ClientManager
{
private:
	static ClientManager * pThis;
	map<SOCKET, Client*> m_mapClient;
	int g_iIndex = 0;

public:
	static ClientManager* GetInstance()
	{
		if (pThis == NULL)
			pThis = new ClientManager();
		return pThis;
	}
	
	bool AddClient(SOCKET sock, SOCKADDR_IN clientaddr);
	bool SetPair(SOCKET sock1, SOCKET sock2);
	bool ReleasePair(SOCKET sock);
	Client * GetClient(SOCKET sock);
	bool ExitClient(SOCKET sock);
	void Release();
	ClientManager();
	~ClientManager();
};

