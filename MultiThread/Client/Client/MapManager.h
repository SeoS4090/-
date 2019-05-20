#pragma once
#include "GlobalDefine.h"
#include "POINT.h"
#include "SingleTone.h"
#include "BitMap.h"
#include <queue>


enum Type
{
	EMPTY,
	BLOCK,
	BUBBLE,
	PLAYER,
	SPEED,
	POWER,
	BUBBLE_COUNT,
	BOOM_CENTER, BOOM_UP,BOOM_UP_EX, BOOM_DOWN, BOOM_DOWN_EX, BOOM_LEFT, BOOM_LEFT_EX, BOOM_RIGHT, BOOM_RIGHT_EX
};

struct PLAYERINFO
{
	string			Id;
	HEngine::POINTF	DrawPt;
	HEngine::POINT	pt;
	queue<HEngine::POINT>	queue_pt;
	int				State;
	bool			m_bDie;
	int				m_iSpeed;
	float			m_fEtime;
};

class MapManager:public SingleTone<MapManager>
{
private:
	int Test;
	float					m_fTime;
	int						m_iBubbleMap[MAPSIZE_X][MAPSIZE_Y];
	int						m_iBubbleMap_Origin[MAPSIZE_X][MAPSIZE_Y];

	vector<PLAYERINFO * >	m_vecPlayerPt;
	HEngine::BitMap*		m_Charactor[END];
	POINT					Init_pt[8];
public:
	void		Init(PACKET_GAMESTART packet);
	bool		AddType(HEngine::POINT pt, Type Object);
	void		BoomBlock(HEngine::POINT pt);
	Type		GetType(HEngine::POINT pt);
	void		MovePlayer(HEngine::POINT Before, HEngine::POINT After, string id, int Speed);
	void		AddPlayer(HEngine::POINT pt, string id);
	void		SetPlayerState(string id, DIRECT state);
	void		MovePlayer(HEngine::POINT Before, HEngine::POINT After);
	bool		Update();
	bool		CheckWin();
	void		ClearBoom(HEngine::POINT pt, int Power);
	void		BoomBubble(HEngine::POINT pt, int Power);
	void		Draw();
	void		Release();
	POINT		GetInitPt(int i);
	MapManager();
	~MapManager();
};

