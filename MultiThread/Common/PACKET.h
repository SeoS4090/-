#pragma once
#pragma comment (lib, "ws2_32.lib")
#include<Windows.h>
#include <iostream>
#include <string>
#define WAITROOM_PLAYER_MAX	8
#define MAX_ID_SIZE			8
#define MAPSIZE_X				15
#define MAPSIZE_Y				13


using namespace std;

enum PACKET_INDEX
{
	PACKET_INDEX_LOGIN,
	PACKET_INDEX_LOGINRT,
	PACKET_INDEX_WAITROOM,
	PACKET_INDEX_LOADWAITROOM,
	PACKET_INDEX_PLAYERINFO,
	PACKET_INDEX_TEXT,
	PACKET_INDEX_GAMEREADY,
	PACKET_INDEX_GAMESTART,
	PACKET_INDEX_PLAYER,
	PACKET_INDEX_PLAYERSTATE,
	PACKET_INDEX_DIE,
	PACKET_INDEX_BUBBLE,
	PACKET_INDEX_EXIT
};

#pragma pack(1)

struct PACKET_HEADER
{
	WORD			wIndex;	//PACKET_INDEX ����
	WORD			wLen;		//�ڿ� ���� ����Ʈ�� ����
};

struct PACKET_LOGIN
{
	PACKET_HEADER	header;
	char			ID[MAX_ID_SIZE + 1];
	int				validID;
};

struct PACKET_WAITROOM //���� ����
{
	PACKET_HEADER	header;
	int				RoomNo;
};

struct PACKET_WAITROOMS //���� ������
{
	PACKET_HEADER	header;
	int				iFullLen; //������� ��ȿ�Ѱ�
	int				iLastRoomNo;
	int				iFull[256];
};

struct PACKET_TEXT //��ȭ
{
	PACKET_HEADER	header;
	char			wStr[256];
	int				Count;
	int				RoomNo;
};

struct PACKET_GAMEREADY
{
	PACKET_HEADER header;	
	int RoomNo;
};

struct PACKET_GAMESTART
{
	PACKET_HEADER header;
	int PlayerCount; // �÷��̾� ����
	int Rand_Seed;
};

struct PACKET_PLAYERMOVE
{
	PACKET_HEADER header;
	POINT PlayerPt;
	POINT BeforePt;
	int RoomNO;
	int Speed;
	char Id[MAX_ID_SIZE + 1];
};

struct PACKET_PLAYERSTATE
{	
	PACKET_HEADER header;
	int state;
	int RoomNo;
	char Id[MAX_ID_SIZE + 1];
};

struct PACKET_BUBBLE
{
	PACKET_HEADER header;
	POINT pt;
	int Power;
	int RoomNo;	
};

struct PACKET_PLAYERINFO
{
	PACKET_HEADER	header;
	int				PlayerCount;
	char			Id[WAITROOM_PLAYER_MAX][MAX_ID_SIZE];

};

#pragma pack()