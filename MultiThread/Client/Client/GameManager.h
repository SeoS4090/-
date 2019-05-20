#pragma once
#include <Windows.h>
#include "SingleTone.h"
#include "BitMap.h"
#include "GlobalDefine.h"
#include "character.h"
#include <map>


enum GAME_STATE {
	LOGIN,
	LOGINRT,
	WAITROOM,
	GAME_READY,
	GAME,
	WIN,
	LOSE
};

class GameManager : public SingleTone<GameManager>
{
private:
	HWND				m_hWnd;
	SOCKET				m_ServerSock;
	char				m_Buf[BUFSIZE];
	int					g_iLen;
	int					m_iWinCx;
	int					m_iWinCy;
	int					m_iState;
	char				m_StrBuf[BUFSIZE];
	string				m_sID;
	int					m_iRoomNo;
	int					m_iMessageLine;
	int					m_iWaitingRoomPlayer;
	string				m_sPlayerId[WAITROOM_PLAYER_MAX];
	character*			m_Player;
	float				m_fDuringTime;
	int					m_iBubbleCount;
	bool				m_bInputKor;

public:
	void				Init(HWND hWnd, int cx, int cy);
	void				Draw();
	bool				SetState(int state);
	bool				Update();
	void				Typing(WPARAM wParam);
	string				GetID();
	SOCKET				GetServerSock();
	void				SetRoomNo(int RoomNo);
	void				ProcessSocketMessage(HWND hWnd, int iMessage, WPARAM wParam, LPARAM lParam, SOCKET g_Sock);
	bool				ProcessPacket(char * szBuf, int retval);
	void				Release();
	int					GetRoomNo();
						GameManager();
						~GameManager();
};

