#pragma once
#include <Windows.h>
#include <vector>
#include "../../Common/PACKET_HEADER.h"
#define BUFSIZE 512
#define MAPSIZE 13

using namespace std;

enum GAME_STATE
{
	LOGIN,
	WAIT_ROOM,
	GAME_READY,
	GAME_START,
	WIN,
	LOSE
};

class GameManager
{
private:
	HDC m_hdc;
	int m_Map[MAPSIZE][MAPSIZE]; // m_Map[HEIGHT][WIDTH]
	char m_Buf[BUFSIZE];
	int g_iLen, g_iIndex;
	std::string				m_sMessage[256];
	char str[256];
	int len;
	int						m_iMessageLine;
	int						m_iState;
	SOCKET					ServerSock;
	bool					m_bTrun;
	bool					m_bTeam;
	static GameManager *	pThis;
	int						m_iWaitRoom;
public:
	static GameManager* GetInstance()
	{
		if (pThis == NULL)
			pThis = new GameManager();
		return pThis;
	}
	void Init(HWND hWnd);
	void SetTurn();
	void Message(WPARAM wParam);
	//void Draw(HDC hdc);
	//void Draw();
	void Draw(HDC hdc);
	void GameRestart();
	int GetState()
	{
		return m_iState;
	}
	SOCKET GetServerSock()
	{
		return ServerSock;
	}
	void SetState(GAME_STATE state);
	void SetWaitRoom(int RoomNo);
	
	int GetWaitRoom ()
	{
		return m_iWaitRoom;
	}
	bool AddStone(int x, int y);
	void Release();
	void ClearMap();
	GameManager();
	~GameManager();
	bool CheckWin(bool Team);
	void ProcessSocketMessage(HWND hWnd, int iMessage, WPARAM wParam, LPARAM lParam, SOCKET g_Sock);
	bool ProcessPacket(char * szBuf, int retval);

};

