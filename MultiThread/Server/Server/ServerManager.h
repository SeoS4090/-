#pragma once
#include "Defines.h"
#include "SingleTone.h"
#include "ClientManager.h"


class ServerManager : public SingleTone<ServerManager>
{
private:
	SOCKET				listen_sock;
	HANDLE				hcp;
	int					retval;


public:
	int					Init();
	bool				Accept(SOCKET & client_sock, SOCKADDR_IN & clientaddr, int & addrlen);
	void				Release();
	
	void				err_quit(char * msg);
	void				err_display(char * msg);
	bool				ProcessPacket(SOCKETINFO * ptr);
	ServerManager();
	~ServerManager();
};

