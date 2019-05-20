#pragma once
#include <Windows.h>

enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN_RET,
	PACKET_INDEX_PLAYER_INFO,
	PACKET_INDEX_PLAYER_POS,
};

#pragma pack(1)
struct PACKET_HEADER
{
	WORD wIndex;
	WORD wLen;
};

struct PLAYER
{
	WORD wIndex;
	WORD x;
	WORD y;
};

struct PACKET_LOGIN_RET
{
	PACKET_HEADER header;
	WORD wIndex;
};

struct PACKET_PLAYER_INFO
{
	PACKET_HEADER header;
	WORD wLen;
	PLAYER info[20];
};

struct PACKET_PLAYER_POS
{
	PACKET_HEADER header;
	PLAYER info;
};

#pragma pack()