#pragma once
#pragma comment (lib, "ws2_32.lib")
#include<Windows.h>
#include <iostream>
#include <string>
using namespace std;

enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN,
	PACKET_INDEX_LOADWAITROOM,
	PACKET_INDEX_WAITROOM,
	PACKET_INDEX_TEXT,
	PACKET_INDEX_GAMEREADY,
	PACKET_INDEX_GAMESTART,
	PACKET_INDEX_PLAYER,
	PACKET_INDEX_EXIT,
	PACKET_INDEX_RETURN
};

#pragma pack(1)
struct PACKET_HEADER
{
	WORD wIndex;	//PACKET_INDEX 상태
	WORD wLen;		//뒤에 들어가는 바이트으 길이
};
struct PACKET_WAITROOM
{
	PACKET_HEADER header;
	int iRoomNo;
};
struct PACKET_WAITROOMS	
{
	PACKET_HEADER header;
	int iFullLen;
	int iFull[256];
};

struct PACKET_TEXT
{
	PACKET_HEADER header;
	char wStr[256];
};
struct PACKET_LOGIN_RET
{
	PACKET_HEADER header;	//패킷 헤더
	WORD wIndex;			//몇번째 클라인가
};

struct PACKET_STONE
{
	PACKET_HEADER header;	//패킷 헤더
	WORD x;
	WORD y;
};
#pragma pack()