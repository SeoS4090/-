#include "ServerManager.h"
#include "WaitRoomManager.h"

#define SERVERPORT	9000
#define BUFSIZE		512


int ServerManager::Init()
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
	
	//socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	
}

bool ServerManager::Accept(SOCKET& client_sock, SOCKADDR_IN& clientaddr, int& addrlen)
{
	//accept()
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		printf("accept()\n");
		return true;
	}
	return false;
}


void ServerManager::Release()
{
	WSACleanup();
}

void ServerManager::err_quit(char *msg)		//에러가 나서 죽어야 할 상황
{
	//void 포인터 받아오기
	LPVOID lpMsgBuf;
	//에러 메세지 출력															* 최근에 있었던 에러 코드 ,  랭귀지채우기								실제 값
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);
	//메세지 박스 띄우기
	MessageBox(NULL, (LPSTR)lpMsgBuf, msg, MB_ICONERROR);
	//DELETE
	LocalFree(lpMsgBuf);
	exit(1);
}

void ServerManager::err_display(char *msg)		//클라이언트 쪽에서 문제가 생겨서 그 클라이언트를 자름
{
	//에러 내용만 출력
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
bool ServerManager::ProcessPacket(SOCKETINFO * ptr)
{
	if (ptr->recvbytes < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, ptr->buf, sizeof(PACKET_HEADER));

	if (ptr->recvbytes < header.wLen)
		return false;

	switch (header.wIndex)
	{
		case PACKET_INDEX_LOGIN:
		{
			PACKET_LOGIN packet;
			memcpy(&packet, ptr->buf, sizeof(packet));

			if (ClientManager::GetInstance()->SetID(ptr->sock, packet.ID))
			{
				printf("%d : %s 으로 접속\n", ptr->sock, packet.ID);
				PACKET_WAITROOMS packet = WaitRoomManager::GetInstance()->SendWaitRoom();
				send(ptr->sock, (char *)&packet, packet.header.wLen, 0);
				break;
			}
			else if (ClientManager::GetInstance()->GetID(ptr->sock) == packet.ID)
			{
				WaitRoomManager::GetInstance()->ExitRoom(ptr->sock);
				PACKET_WAITROOMS packet = WaitRoomManager::GetInstance()->SendWaitRoom();
				ClientManager::GetInstance()->SendPacket(packet);
			}
			else
			{
				packet.header.wIndex = PACKET_INDEX_LOGINRT;
				send(ptr->sock, (char*)&packet, packet.header.wLen, 0);
			}
			break;
		}
		case PACKET_INDEX_WAITROOM:
		{
			PACKET_WAITROOM packet;
			memcpy(&packet, ptr->buf, header.wLen);
			send(ptr->sock, (char *)&packet, header.wLen, 0);
			WaitRoomManager::GetInstance()->ExitRoom(ptr->sock);
			WaitRoomManager::GetInstance()->EnterWaitRoom(ptr->sock, packet.RoomNo);
			WaitRoomManager::GetInstance()->SendPlayerInfo(packet.RoomNo);
			PACKET_WAITROOMS wtpacket = WaitRoomManager::GetInstance()->SendWaitRoom();
			ClientManager::GetInstance()->SendPacket(wtpacket);
			break;
		}
		case PACKET_INDEX_TEXT:
		{
			PACKET_TEXT packet;
			memcpy(&packet, ptr->buf, header.wLen);
			WaitRoomManager::GetInstance()->SendPacket(packet.RoomNo, (char *)&packet, header.wLen);
			break;
		}
		case PACKET_INDEX_PLAYER:
		{
			PACKET_PLAYERMOVE packet;
			memcpy(&packet, ptr->buf, header.wLen);
			WaitRoomManager::GetInstance()->SendPacket(packet.RoomNO, (char *)&packet, header.wLen);
			break;
		}
		case PACKET_INDEX_BUBBLE:
		{
			PACKET_BUBBLE packet;
 			memcpy(&packet, ptr->buf, header.wLen);
			WaitRoomManager::GetInstance()->SendPacket(packet.RoomNo, (char *)&packet, header.wLen, ptr->sock);
			break;
		}

		case PACKET_INDEX_PLAYERSTATE:
		{
			PACKET_PLAYERSTATE packet;
			memcpy(&packet, ptr->buf, header.wLen);
			WaitRoomManager::GetInstance()->SendPacket(packet.RoomNo, (char *)&packet, header.wLen);
			break;
		}
		case PACKET_INDEX_GAMEREADY:
		{
			PACKET_GAMEREADY packet;
			memcpy(&packet, ptr->buf, header.wLen);
			if (WaitRoomManager::GetInstance()->PlayerReady(ptr->sock, packet.RoomNo))
			{
				PACKET_WAITROOMS wtpacket = WaitRoomManager::GetInstance()->SendWaitRoom();
				ClientManager::GetInstance()->SendPacket(wtpacket);
			}
			break;
		}
		case PACKET_INDEX_DIE:
		{
			printf("DIE\n");
			PACKET_PLAYERSTATE packet;
			memcpy(&packet, ptr->buf, header.wLen);
			WaitRoomManager::GetInstance()->SetWaitRoomGameStart(packet.RoomNo);
			WaitRoomManager::GetInstance()->SendPacket(packet.RoomNo, (char *)&packet, header.wLen, ptr->sock);

			break;
		}

		case PACKET_INDEX_EXIT:
		{
 			PACKET_WAITROOM packet;
			memcpy(&packet, ptr->buf, header.wLen);
			WaitRoomManager::GetInstance()->ExitRoom(ptr->sock);
			ClientManager::GetInstance()->ExitClient(ptr->sock);
			return true;
		}
	}

	memcpy(&ptr->buf[0], &ptr->buf[header.wLen], sizeof(ptr->buf) - header.wLen);
	ptr->recvbytes -= header.wLen;


	if (ptr->recvbytes >= sizeof(PACKET_HEADER))
		ProcessPacket(ptr);
}

ServerManager::ServerManager()
{
}


ServerManager::~ServerManager()
{
}
